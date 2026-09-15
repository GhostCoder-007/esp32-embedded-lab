#include "led.h"
#include "esp_log.h"
#include "driver/gpio.h"

static const char *TAG = "LED_ON_OFF";

esp_err_t led_init(int GPIO_PIN_NUMBER)
{
    gpio_config_t io_config = { 
    .pin_bit_mask =  (1ULL << GPIO_PIN_NUMBER),
    .mode = GPIO_MODE_OUTPUT,
    .pull_up_en = GPIO_PULLUP_DISABLE,
    .pull_down_en = GPIO_PULLDOWN_DISABLE,
    .intr_type = GPIO_INTR_DISABLE,
    };
    return gpio_config(&io_config);
    
    
}

void led_on(int GPIO_PIN_NUMBER)
{
    gpio_set_level(GPIO_PIN_NUMBER,IO_ON);
    ESP_LOGI(TAG, "LED IS ON");
}

void led_off(int GPIO_PIN_NUMBER)
{
    gpio_set_level(GPIO_PIN_NUMBER,IO_OFF);
    ESP_LOGI(TAG, "LED IS OFF");
}