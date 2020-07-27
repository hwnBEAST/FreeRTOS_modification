/**
 * @file use_example.c
 *
 * @brief   Tests for added new FreeRTOS functionality.
 */

/*----------------------------------------------------------------------------*/

#include <ndebug_printf.h>

#include <FreeRTOS.h>
#include <task.h>

#include "tests.h"

#include "tests_timed_task.h"
#include "tests_replicated_task.h"
#include "tests_get_type.h"

/*----------------------------------------------------------------------------*/

/*----------------------------------------------------------------------------*/

void tests_task(void * unused)
{
    test_status_t test_status;
    for(;;)
    {
#if 1
        ndebug_printf("Testing timed tasks...");
        test_status = tests_timed_task();
        ndebug_printf("%s\n", TEST_PASS == test_status ? "OK" : "ERROR");
#endif

#if 1
        ndebug_printf("Testing replicated tasks...");
        test_status = tests_replicated_task();
        ndebug_printf("%s\n", TEST_PASS == test_status ? "OK" : "ERROR");
#endif

#if 1
        ndebug_printf("Testing get task type...");
        test_status = tests_get_type();
        ndebug_printf("%s\n", TEST_PASS == test_status ? "OK" : "ERROR");
#endif
        vTaskDelay(pdMS_TO_TICKS(4 * 1000));
    }
}

/*----------------------------------------------------------------------------*/

/****END OF FILE****/