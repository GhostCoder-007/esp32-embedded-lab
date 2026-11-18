#include <stdio.h>
#include "ultrasonic.h"
#include "driver/gpio.h"

#define ECHO_PIN GPIO_NUM_22
#define TRIG_PIN GPIO_NUM_23 

void app_main(void)
{
    esp_err_t install_err = gpio_install_isr_service(0);
    if (install_err == ESP_ERR_INVALID_STATE || install_err == ESP_OK )
    {
        static ultrasonic_t sensor1 = {
        .echo_pin = ECHO_PIN,
        .trig_pin = TRIG_PIN
        };
        esp_err_t init_err = ultrasonic_init(&sensor1);

        if (init_err != ESP_OK)
        {
            printf("Ultrasonic initialization failed: %s\n",esp_err_to_name(init_err));
        }
        float distance_calc; //cm
        ultrasonic_measure(&sensor1,&distance_calc);
        printf("Measured Distance: %f cm\n",esp_err_to_name(init_err));
    }
    else
    {
        printf("Ultrasonic Service Install failed: %s\n",esp_err_to_name(install_err));
    }
}