#include "ultrasonic.h"
#include "esp_err.h"
#include "esp_timer.h"
#include <math.h>
#include "freertos/projdefs.h"
#include "freertos/task.h"
#include "portmacro.h"
#include "esp_rom_sys.h"

static void isr_gpio_handler(void *args)
{
    ultrasonic_t *sensor = (ultrasonic_t*)args;
    if (gpio_get_level(sensor->echo_pin))
    {
        
        // positive edge
        taskENTER_CRITICAL_ISR(&sensor->mux);
        if (sensor->state == ULTRASONIC_WAITING_FOR_RISING)
        {
            sensor->rising_timestamp = esp_timer_get_time();
            sensor->state = ULTRASONIC_WAITING_FOR_FALLING;
        }
        taskEXIT_CRITICAL_ISR(&sensor->mux);
    }
    else
    {
        // negative edge
        bool ready_to_notify_falling = false;
        taskENTER_CRITICAL_ISR(&sensor->mux);
        if (sensor->state == ULTRASONIC_WAITING_FOR_FALLING)
        {
            sensor->falling_timestamp = esp_timer_get_time();
            sensor->state = ULTRASONIC_COMPLETE;
            ready_to_notify_falling = true;
        }
        TaskHandle_t currentTask = sensor->measurement_task;

        taskEXIT_CRITICAL_ISR(&sensor->mux);

        if (ready_to_notify_falling)
        {
            // vTaskNotifyGiveFromISR() -> notifies that the falling edge is complete and calculation can commence
            BaseType_t pxHigherPriorityTaskWoken = pdFALSE;

            if (currentTask != NULL)
            {   
                vTaskNotifyGiveFromISR(currentTask,&pxHigherPriorityTaskWoken); //unblocks measurement task
                portYIELD_FROM_ISR(pxHigherPriorityTaskWoken); // allows context switch to happen immediately to a higher priority task if necessary
            }
        }
    }
}

esp_err_t ultrasonic_init(ultrasonic_t *sensor)
{
    if (sensor == NULL) // make sure pointer location is valid.
    {
        return ESP_ERR_INVALID_ARG;
    }

    if ((sensor->echo_pin == sensor->trig_pin) || !(GPIO_IS_VALID_OUTPUT_GPIO(sensor->trig_pin)) || !(GPIO_IS_VALID_GPIO(sensor->echo_pin)))// make sure the GPIO pins parsed actually exist
    {
        return ESP_ERR_INVALID_ARG;
    }
    
    //Configure Trig as output
    gpio_config_t trig_config = {
        .pin_bit_mask = ( 1ULL << sensor->trig_pin),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };
    
    esp_err_t trig_err_returned = gpio_config(&trig_config);
    if (trig_err_returned != ESP_OK)
    {
        return trig_err_returned;
    }

    //Init Trig to 0 
    esp_err_t gpio_err_returned = gpio_set_level(sensor->trig_pin,IO_OFF);
    if (gpio_err_returned != ESP_OK)
    {
        return gpio_err_returned;
    }
    
    //Configure Echo as input & Interrupt
    gpio_config_t echo_config = {
        .pin_bit_mask = ( 1ULL << sensor->echo_pin),
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_ANYEDGE, // check for both neg and pos edge in GPIO to trigger an interrupt.
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
    };

    esp_err_t echo_err_returned = gpio_config(&echo_config); // configure the gpio using settings above
    if (echo_err_returned != ESP_OK)
    {
        return echo_err_returned;
    }

    sensor->state = ULTRASONIC_IDLE; // initialize the Ultrasonic sensor measurement sequence state to "IDLE"

    sensor->measurement_task = NULL; // initialize the task handle to null

    /*
    Add an ISR handler for above Input GPIO
    @input           gpio_num::sensor->echo_pin    => set the input pin as sensor echo
    @input           isr_handler::isr_gpio_handler => callback function which is executed when added isr is triggered (+ / - edge)
    @input           args*::sensor                 => the context argument used by the callback function (isr_gpio_handler)
    */
    esp_err_t handler_returned = gpio_isr_handler_add(sensor->echo_pin,isr_gpio_handler,sensor); 
    if (handler_returned != ESP_OK)
    {
        return handler_returned;
    }

    return ESP_OK;
}

esp_err_t ultrasonic_measure(ultrasonic_t *sensor, float *distance_cm)
{
    // check if the sensor and output are valid
    if (sensor == NULL || distance_cm == NULL)
    {
        return ESP_ERR_INVALID_ARG;
    }

    // Ensure no measurement is already running
    bool sensor_available = false;

    taskENTER_CRITICAL(&sensor->mux); //prevent resource conflicts - one elemet
    if (sensor->state == ULTRASONIC_IDLE)
    {
        // Create task handle
        sensor->measurement_task = xTaskGetCurrentTaskHandle();

        // set the ultrasonic state to ULTRASONIC_WAITING_FOR_RISING
        sensor->state = ULTRASONIC_WAITING_FOR_RISING;
        sensor_available  = true;
    }
    taskEXIT_CRITICAL(&sensor->mux);

    if (sensor_available)
    {
        //clear any pending notification immediately without blocking
        ulTaskNotifyTake(pdTRUE, 0);

        // Generate Trip Pulse
        // SET Trigger to HIGH
        esp_err_t gpio_on_err_returned = gpio_set_level(sensor->trig_pin,IO_ON);
        if (gpio_on_err_returned != ESP_OK)
        {
            taskENTER_CRITICAL(&sensor->mux);
            sensor->state = ULTRASONIC_IDLE;
            sensor->measurement_task = NULL;
            taskEXIT_CRITICAL(&sensor->mux);
            return gpio_on_err_returned;
        }

        // delay for 10 us and then reset for a 10 µs trigger pulse (TRIG requirement)
        esp_rom_delay_us(10); 

        // SET Trigger to LOW
        esp_err_t gpio_off_err_returned = gpio_set_level(sensor->trig_pin,IO_OFF);

        if (gpio_off_err_returned != ESP_OK)
        {
            taskENTER_CRITICAL(&sensor->mux);
            sensor->state = ULTRASONIC_IDLE;
            sensor->measurement_task = NULL;
            taskEXIT_CRITICAL(&sensor->mux);
            return gpio_off_err_returned;
        }

        /* 
        Wait for ISR to complete measurement - ulTaskNotifyTake() -> wait so CPU isnt wasted
        pdTRUE-> clears notification count when task receives it
        portMAX_DELAY -> block task till notification is received from ISR
        */
        uint32_t ulNotificationValue = ulTaskNotifyTake(pdTRUE, pdMS_TO_TICKS(100)); // timeout of 100ms

        if (ulNotificationValue != 0)
        {
            if (sensor->state == ULTRASONIC_COMPLETE)
            {
                //Calculate Distance (Pulse duration stored in distance_cm mem)
                *distance_cm = ((float)(sensor->falling_timestamp - sensor->rising_timestamp))/58.0f; //used formular
                taskENTER_CRITICAL(&sensor->mux);
                sensor->state = ULTRASONIC_IDLE;
                sensor->measurement_task = NULL;
                taskEXIT_CRITICAL(&sensor->mux);
            }
            else
            {
                taskENTER_CRITICAL(&sensor->mux);
                sensor->state = ULTRASONIC_IDLE;
                sensor->measurement_task = NULL;
                taskEXIT_CRITICAL(&sensor->mux);
                return ESP_ERR_INVALID_STATE;
            }
        }
        else
        {
            taskENTER_CRITICAL(&sensor->mux);
            sensor->state = ULTRASONIC_IDLE;
            sensor->measurement_task = NULL;
            taskEXIT_CRITICAL(&sensor->mux);
            return ESP_ERR_TIMEOUT;
        }
        return ESP_OK;
    }
    else
    {
        return ESP_ERR_INVALID_STATE;
    }
}

