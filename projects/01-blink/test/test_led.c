#include "soc/gpio_num.h"
#include "unity.h"
#include "led.h"
#include "mock_gpio.h"


led_config_t led = {.pin = GPIO_NUM_2 };
const char* TAG = "TEST";


void suiteSetUp(void){
    ESP_LOGW(TAG, "########## Running UNIT TESTS for Blink LED ##########");
}

int suiteTearDown(int num_failures){
    ESP_LOGW(TAG, "########## Completed all UNIT TESTS for Blink LED ##########");
    return num_failures;
}

void setUp(void) {
    ESP_LOGW(TAG, "-----> Running setUP()");
    gpio_mock_reset();
}

void tearDown(void) {
    ESP_LOGW(TAG, "-----> Running tearDown()");
}

TEST_CASE("Initialize", "[init]")
{
    TEST_ASSERT_EQUAL(ESP_OK, led_init(&led));       // Should return ESP_OK if already initialized.
}

TEST_CASE("CHECK IO PIN", "[led_on]")
{
    led_on(&led);
    gpio_dev_t lastc = gpio_mock_get_last_call();
    TEST_ASSERT_EQUAL(GPIO_NUM_2, lastc.gpio_num);   
}

TEST_CASE("LED ON", "[led_on]")
{
    
    led_on(&led);
    gpio_dev_t lastc = gpio_mock_get_last_call();
    TEST_ASSERT_EQUAL(IO_ON, lastc.level);       
}

TEST_CASE("LED OFF", "[led_off]]")
{
    led_off(&led);
    gpio_dev_t lastc = gpio_mock_get_last_call();
    TEST_ASSERT_EQUAL(IO_OFF, lastc.level);       
}
