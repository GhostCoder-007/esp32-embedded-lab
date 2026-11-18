#include <stdio.h>
#include "ultrasonic.h"
#include "driver/gpio.h"

#define ECHO_PIN GPIO_NUM_22
#define TRIG_PIN GPIO_NUM_23 

void app_main(void)
{
    //Install the GPIO ISR service
    esp_err_t install_err = gpio_install_isr_service(0);
    if (install_err == ESP_ERR_INVALID_STATE || install_err == ESP_OK ) //if service is already running or started successfuly
    {
        // Initialise the ultrasonic sensor.
        static ultrasonic_t sensor1 = {
        .echo_pin = ECHO_PIN,
        .trig_pin = TRIG_PIN,
        .mux = portMUX_INITIALIZER_UNLOCKED
        };
        esp_err_t init_err = ultrasonic_init(&sensor1);

        if (init_err != ESP_OK)
        {
            printf("Ultrasonic initialization failed: %s\n",esp_err_to_name(init_err));
        }
        else
        {
            TickType_t delay_ticks = pdMS_TO_TICKS(2000);
            while (true){
                float distance_calc; //cm
                esp_err_t measure_err = ultrasonic_measure(&sensor1,&distance_calc);
                if (measure_err != ESP_OK)
                {
                    printf("Ultrasonic Measurement failed: %s\n",esp_err_to_name(measure_err));
                }
                else
                {
                    printf("Measured Distance: %f cm\n",distance_calc);
                }
                vTaskDelay(delay_ticks);
            }
        }
    }
    else
    {
        printf("Ultrasonic Service Install failed: %s\n",esp_err_to_name(install_err));
    }
}