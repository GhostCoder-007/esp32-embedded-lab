#include "led.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "driver/gpio.h"
#define IO_PIN GPIO_NUM_2
void app_main(void)
{
    TickType_t delay_ticks = pdMS_TO_TICKS(1000);
    if (led_init(IO_PIN) == ESP_OK)
    {
        while (true){
            led_on(IO_PIN);
            vTaskDelay(delay_ticks);
            led_off(IO_PIN);
            vTaskDelay(delay_ticks);
        }
    }
}
