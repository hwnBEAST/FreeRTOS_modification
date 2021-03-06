/**
 * @file tests_timed_task.c
 *
 * @brief   Tests for added timed task.
 */

#include "tests_timed_task.h"

#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>

#include <stdbool.h>

#include "ndebug_printf.h"

/******************************************************************************/

#undef TEST_TASK_PRIORITY
#define TEST_TASK_PRIORITY 1

#undef TIMED_TIMEOUT
#define TIMED_TIMEOUT 500


/******************************************************************************/

test_status_t test_slower_than_oflow_timeout();
test_status_t test_faster_than_oflow_timeout();
void task_slower_oflow(void * unused);
void task_faster_oflow(void * unused);
void slower_oflow_timeout_cb(WorstTimeTimerHandle_t h_timer);
void faster_oflow_timeout_cb(WorstTimeTimerHandle_t h_timer);

test_status_t test_slower_than_orun_timeout();
test_status_t test_faster_than_orun_timeout();
void task_slower_orun(void * unused);
void task_faster_orun(void * unused);
void slower_orun_timeout_cb(WorstTimeTimerHandle_t h_timer);
void faster_orun_timeout_cb(WorstTimeTimerHandle_t h_timer);

/******************************************************************************/

static volatile bool g_is_success;

/******************************************************************************/

void tests_timed_task(void)
{
    ndebug_printf("    Test slower than overflow timeout...%s\n",
                  test_slower_than_oflow_timeout() == TEST_PASS?
                  "OK": "FAIL");

    ndebug_printf("    Test faster than overflow timeout...%s\n",
                  test_faster_than_oflow_timeout() == TEST_PASS?
                  "OK": "FAIL");

    ndebug_printf("    Test slower than overrun timeout...%s\n",
                  test_slower_than_orun_timeout() == TEST_PASS?
                  "OK": "FAIL");

    ndebug_printf("    Test faster than overrun timeout...%s\n",
                  test_faster_than_orun_timeout() == TEST_PASS?
                  "OK": "FAIL");
}

/******************************************************************************/

test_status_t test_slower_than_oflow_timeout()
{
    TaskHandle_t h_timed = NULL;

    g_is_success = false;

    xTaskCreateTimed(task_slower_oflow,
                     "slower",
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     TEST_TASK_PRIORITY,
                     &h_timed,
                     0,
                     NULL,
                     TIMED_TIMEOUT,
                     slower_oflow_timeout_cb);

    vTaskDelay(TIMED_TIMEOUT + 50);

    if(h_timed)
    {
        vTaskDelete(h_timed);
    }

    return g_is_success ? TEST_PASS : TEST_FAIL;
}

void task_slower_oflow(void * unused)
{
    vTaskDelay(portMAX_DELAY);
}

void slower_oflow_timeout_cb(WorstTimeTimerHandle_t h_timer)
{
    g_is_success = true;
}

/******************************************************************************/

test_status_t test_faster_than_oflow_timeout()
{
    TaskHandle_t h_timed = NULL;

    g_is_success = true;

    xTaskCreateTimed(task_faster_oflow,
                     "faster",
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     TEST_TASK_PRIORITY,
                     &h_timed,
                     0,
                     NULL,
                     TIMED_TIMEOUT,
                     faster_oflow_timeout_cb
                    );

    vTaskDelay(TIMED_TIMEOUT * 4);

    if(h_timed)
    {
        vTaskDelete(h_timed);
    }

    return g_is_success ? TEST_PASS : TEST_FAIL;
}

void task_faster_oflow(void * unused)
{
    while(true)
    {
        vTaskDelay(TIMED_TIMEOUT - 50);
        vTaskTimedReset(NULL);
    }
}

void faster_oflow_timeout_cb(WorstTimeTimerHandle_t h_timer)
{
    g_is_success = false;
}

/******************************************************************************/

test_status_t test_slower_than_orun_timeout()
{
    TaskHandle_t h_timed = NULL;

    g_is_success = false;

    xTaskCreateTimed(task_slower_orun,
                     "faster",
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     TEST_TASK_PRIORITY,
                     &h_timed,
                     TIMED_TIMEOUT,
                     slower_orun_timeout_cb,
                     0,
                     NULL
                    );

    vTaskDelay(TIMED_TIMEOUT + 50);

    if(h_timed)
    {
        vTaskDelete(h_timed);
    }

    return g_is_success ? TEST_PASS : TEST_FAIL;
}

void task_slower_orun(void * unused)
{
    while(true)
    {
        HAL_Delay(TIMED_TIMEOUT * 2);
        vTaskDelay(portMAX_DELAY);
    }
}
void slower_orun_timeout_cb(WorstTimeTimerHandle_t h_timer)
{
    g_is_success = true;
}

/******************************************************************************/

test_status_t test_faster_than_orun_timeout()
{
    TaskHandle_t h_timed = NULL;

    g_is_success = true;

    xTaskCreateTimed(task_slower_orun,
                     "faster",
                     configMINIMAL_STACK_SIZE,
                     NULL,
                     TEST_TASK_PRIORITY,
                     &h_timed,
                     TIMED_TIMEOUT,
                     slower_orun_timeout_cb,
                     0,
                     NULL
                    );

    vTaskDelay(TIMED_TIMEOUT * 4);

    if(h_timed)
    {
        vTaskDelete(h_timed);
    }

    return g_is_success ? TEST_PASS : TEST_FAIL;
}

void task_faster_orun(void * unused)
{
    while(true)
    {
        HAL_Delay(TIMED_TIMEOUT - 50);
        vTaskTimedReset(NULL);
    }
}
void faster_orun_timeout_cb(WorstTimeTimerHandle_t h_timer)
{
    g_is_success = false;
}
/******************************************************************************/
/****END OF FILE****/
