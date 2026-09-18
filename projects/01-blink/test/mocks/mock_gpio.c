#include "esp_err.h"
#include "esp_log.h"
#include "mock_gpio.h"

static gpio_dev_t last_gpio_call;

void gpio_mock_reset(void)
{
    last_gpio_call.gpio_num = 0;
    last_gpio_call.level = 0;
}

esp_err_t __wrap_gpio_set_level(int gpio_num, uint32_t level)
{
    last_gpio_call.gpio_num = gpio_num;
    last_gpio_call.level = level;
    ESP_LOGW("Test", "MOCK GPIO SET LEVEL USED");
    return ESP_OK;
}

gpio_dev_t gpio_mock_get_last_call(void)
{
    return last_gpio_call;
}