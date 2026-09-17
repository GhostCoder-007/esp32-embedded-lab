#include "unity.h"
#include "esp_log.h"
void app_main(void)
{
    suiteSetUp();    
    UNITY_BEGIN();
    unity_run_all_tests();  // automatically finds and runs all test cases defined with TEST_CASE()
    UNITY_END();
    // unity_run_menu();    // Alternative to using unity_run_all_tests(). It opens a user interactive menu to select which tests to run.
    suiteTearDown(Unity.TestFailures);
}