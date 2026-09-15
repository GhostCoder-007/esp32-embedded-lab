#include "led.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"


void app_main(void)
{
    TickType_t delay_ticks = pdMS_TO_TICKS(1000);
    led_config_t led = {.pin = GPIO_NUM_2};
    if (led_init(&led) == ESP_OK)
    {
        while (true){
            led_on(&led);
            vTaskDelay(delay_ticks);
            led_off(&led);
            vTaskDelay(delay_ticks);
        }
    }
}
