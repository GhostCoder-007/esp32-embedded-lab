#include "led.h"

static const char *TAG = "LED_ON_OFF";

esp_err_t led_init(led_config_t *led_config)
{
    gpio_config_t io_config = { 
    .pin_bit_mask =  (1ULL << led_config->pin),
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE,
    };
    return gpio_config(&io_config);
}

void led_on(led_config_t *led_config)
{
    gpio_set_level(led_config->pin,IO_ON);
    ESP_LOGI(TAG, "LED IS ON");
}

void led_off(led_config_t *led_config)
{
    gpio_set_level(led_config->pin,IO_OFF);
    ESP_LOGI(TAG, "LED IS OFF");
}