#include "ultrasonic.h"
#include <stdlib.h>

void isr_handler(void *args)
{
    ultrasonic_t *sensor = (ultrasonic_t*)args;
    if (gpio_get_level(sensor->echo_pin))
    {
        // positive edge
    }
    else
    {
        // negative edge
    }
}

esp_err_t ultrasonic_init(ultrasonic_t *sensor)
{
    if (sensor == NULL)
    {
        return ESP_ERR_INVALID_ARG;
    }

    if ((sensor->echo_pin == sensor->trig_pin) || !(GPIO_IS_VALID_OUTPUT_GPIO(sensor->trig_pin)) || !(GPIO_IS_VALID_GPIO(sensor->echo_pin)))
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
        .intr_type = GPIO_INTR_ANYEDGE,
        .pull_up_en = GPIO_PULLUP_DISABLE,
        .pull_down_en = GPIO_PULLDOWN_DISABLE,
    };

    esp_err_t echo_err_returned = gpio_config(&echo_config);
    if (echo_err_returned != ESP_OK)
    {
        return echo_err_returned;
    }

    esp_err_t handler_returned = gpio_isr_handler_add(sensor->echo_pin,isr_handler,sensor);
    if (handler_returned != ESP_OK)
    {
        return handler_returned;
    }

    return ESP_OK;
}

