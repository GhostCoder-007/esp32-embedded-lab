#include "ultrasonic.h"
#include "esp_err.h"
#include "esp_timer.h"
#include <math.h>
#include "freertos/task.h"
#include "portmacro.h"

static void isr_gpio_handler(void *args)
{
    ultrasonic_t *sensor = (ultrasonic_t*)args;
    if (gpio_get_level(sensor->echo_pin))
    {
        // positive edge
        if (sensor->state == ULTRASONIC_WAITING_FOR_RISING)
        {
            sensor->rising_timestamp = esp_timer_get_time();
            sensor->state = ULTRASONIC_WAITING_FOR_FALLING;
        }
    }
    else
    {
        // negative edge
        if (sensor->state == ULTRASONIC_WAITING_FOR_FALLING)
        {
            sensor->falling_timestamp = esp_timer_get_time();
            sensor->state = ULTRASONIC_COMPLETE;

            // vTaskNotifyGiveFromISR() -> notifies that the falling edge is complete and calculation can commence
            BaseType_t pxHigherPriorityTaskWoken = pdFALSE;
            vTaskNotifyGiveFromISR(sensor->measurement_task,&pxHigherPriorityTaskWoken);
        }
    }
}

esp_err_t ultrasonic_init(ultrasonic_t *sensor)
{
    if (sensor == NULL) // check for invalid args in sensor struct (cant be empty)
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

    if ((sensor->echo_pin == sensor->trig_pin) || !(GPIO_IS_VALID_OUTPUT_GPIO(sensor->trig_pin)) || !(GPIO_IS_VALID_GPIO(sensor->echo_pin)))
    {
        return ESP_ERR_INVALID_ARG;
    }

    // esure no measurement is already running

    // Create task handle
    sensor->measurement_task = xTaskGetCurrentTaskHandle();

    // set the ultrasonic state to ULTRASONIC_WAITING_FOR_RISING
    sensor->state = ULTRASONIC_WAITING_FOR_RISING;

    // Generate Trip Pulse
    esp_err_t gpio_err_returned = gpio_set_level(sensor->trig_pin,IO_ON);
    if (gpio_err_returned != ESP_OK)
    {
        return gpio_err_returned;
    }

    // Wait for ISR to complete measurement
    //ulTaskNotifyTake() -> wait so CPU isnt wasted

    //Calculate Distance (Pulse duration stored in distance_cm mem)


    return ESP_OK;
}

