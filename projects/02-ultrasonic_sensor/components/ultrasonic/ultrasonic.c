#include "ultrasonic.h"


esp_err_t ultrasonic_init(gpio_num_t trig_pin, gpio_num_t echo_pin)
{
    //Configure Trig as output
    gpio_config_t trig_config = {
        .pin_bit_mask = ( 1ULL << trig_pin),
        .mode = GPIO_MODE_OUTPUT,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
        .intr_type = GPIO_INTR_DISABLE,
    };

    gpio_config(&trig_config);

    //Init Trig to 0 
    gpio_set_level(trig_pin,IO_OFF);

    //Configure Echo as input & Interrupt
    gpio_config_t echo_config = {
        .pin_bit_mask = ( 1ULL << echo_pin),
        .mode = GPIO_MODE_INPUT,
        .intr_type = GPIO_INTR_ANYEDGE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
    };

    esp_err_t result = gpio_config(&echo_config);

    return result;
}