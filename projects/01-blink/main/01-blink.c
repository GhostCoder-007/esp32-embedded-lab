#include "freertos/projdefs.h"
#include "led.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "portmacro.h"

void app_main(void)
{
    TickType_t Delay = pdMS_TO_TICKS(5000);
    if (led_init() == ESP_OK)
    {
        led_on();
        vTaskDelay(Delay);
        led_off();
    }
}
