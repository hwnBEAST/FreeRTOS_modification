/**
 * @file example.c
 *
 * @brief   Example of usage of added FreeRTOS functionality.
 *
 * @note    Written according to BARR-C:2018 coding standard
 *          Exceptions:
 *              - 3.2 a, c  - Eclipse formatter doesn't support
 *              - 6.3 b iv. - ERR_CHECK() macro has return keyword
 *              - 7.1 m     - Boolean begins with is, e.g. is_example
 */

#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

#include <ndebug_printf.h>

#include <stm32f4xx_hal.h>

#include <FreeRTOS.h>
#include <task.h>
#include <timers.h>

#include "example.h"
#include "tests.h"

#define PRIORITY_TESTS 1

#define LED_GREEN_Pin GPIO_PIN_12
#define LED_GREEN_GPIO_Port GPIOD
#define LED_ORANGE_Pin GPIO_PIN_13
#define LED_ORANGE_GPIO_Port GPIOD
#define LED_RED_Pin GPIO_PIN_14
#define LED_RED_GPIO_Port GPIOD
#define LED_BLUE_Pin GPIO_PIN_15
#define LED_BLUE_GPIO_Port GPIOD

#define BTN_BLUE_Pin GPIO_PIN_0
#define BTN_BLUE_GPIO_Port GPIOA

#define NUM_OF_DIGITS 10
#define NUM_OF_SIGNS 8

/* The task functions prototype */
void vTask1 (void *pvParameters);
void vTask2 (void *pvParameters);
void vTask3 (void *pvParameters);
void vTask4 (void *pvParameters);

/* Called when error happens with compare values */
uint8_t cmp_val_err_cb (CompareValue_t * pxCompareValues, uint8_t ucLen);

void timer_cb_fnc (TimerHandle_t xTimer);

#ifndef NDEBUG
/* Used for run time stats of heap */
static const uint8_t freeRTOSMemoryScheme = 4;
#endif

uint32_t compare_fail = 0;

void example_run (void)
{
#if TEST_MODE == 1

    xTaskCreate(tests_task,
                "Tests",
                configMINIMAL_STACK_SIZE,
                NULL,
                PRIORITY_TESTS,
                NULL);

    /* Give control to FreeRTOS */
    vTaskStartScheduler();
    for(;;);
#else
    xTaskCreate(vTask2, "T2", configMINIMAL_STACK_SIZE, NULL, 1, NULL);
    xTaskCreateReplicated(vTask3, "T3", configMINIMAL_STACK_SIZE, NULL, 1, NULL,
    taskREPLICATED_NO_RECOVERY, cmp_val_err_cb);
    xTaskCreateReplicated(vTask4, "T4", configMINIMAL_STACK_SIZE, NULL, 1, NULL,
    taskREPLICATED_RECOVERY, cmp_val_err_cb);

    /* Give control to FreeRTOS */
    vTaskStartScheduler();
    for(;;);
#endif
#   ifndef NDEBUG
    (void)freeRTOSMemoryScheme;
#   endif
}

void vTask1 (void *pvParameters)
{
    for (;;)
    {
        ndebug_printf("Task type of task %s is %d\n", pcTaskGetName(NULL),
                eTaskGetType(NULL));
        HAL_GPIO_TogglePin(LED_RED_GPIO_Port, LED_RED_Pin);
        vTaskDelay(pdMS_TO_TICKS(2 * 1000));
    }
}

void vTask2 (void *pvParameters)
{
    for (;;)
    {
        ndebug_printf("Task type of task %s is %d\n", pcTaskGetName(NULL),
                eTaskGetType(NULL));
        HAL_GPIO_TogglePin(LED_BLUE_GPIO_Port, LED_BLUE_Pin);
        vTaskDelay(pdMS_TO_TICKS(1 * 1000));
    }
}

void vTask3 (void *pvParameters)
{
    for (;;)
    {
        ndebug_printf("Task type of task %s is %d\n", pcTaskGetName(NULL),
                eTaskGetType(NULL));
        HAL_GPIO_TogglePin(LED_GREEN_GPIO_Port, LED_GREEN_Pin);
        vTaskDelay(pdMS_TO_TICKS(1 * 1000));
        if (HAL_GPIO_ReadPin(BTN_BLUE_GPIO_Port, BTN_BLUE_Pin) == pdTRUE)
        {
            vTaskDelete(NULL);
        }
        compare_fail++;
        vTaskSyncAndCompare(&(compare_fail));
    }
}

void vTask4 (void *pvParameters)
{
    for (;;)
    {
        ndebug_printf("Task type of task %s is %d\n", pcTaskGetName(NULL),
                eTaskGetType(NULL));
        HAL_GPIO_TogglePin(LED_ORANGE_GPIO_Port, LED_ORANGE_Pin);

        if (HAL_GPIO_ReadPin(BTN_BLUE_GPIO_Port, BTN_BLUE_Pin) == pdTRUE)
        {
            vTaskDelete(NULL);
        }
        compare_fail++;
        vTaskSyncAndCompare(&compare_fail);
    }
}

uint8_t cmp_val_err_cb (CompareValue_t * pxCompareValues, uint8_t ucLen)
{
    uint8_t ucIsTaskDeleteNeeded = pdTRUE;

    ndebug_printf("Compare values don't match:");

    for (uint32_t iii = 0; iii < ucLen; iii++)
    {
        ndebug_printf(" %lu", pxCompareValues[iii]);
    }
    ndebug_printf("\n");

    return ucIsTaskDeleteNeeded;
}

void timer_cb_fnc (TimerHandle_t xTimer)
{
    bool is_same_name = !strncmp(pcTimerGetName(xTimer), "WorstTimeTimer",
            strlen("WorstTimeTimer"));
    TaskHandle_t ph_owner_task = pvTimerGetTimerID(xTimer);

    configASSERT(is_same_name != 0);

#   ifndef NDEBUG
    {
        ndebug_printf("Timer %s overflowed. ", pcTimerGetName(xTimer));
        ndebug_printf("Task %s overflowed.\n", pcTaskGetName(ph_owner_task));
    }
#   endif

#   if 0
    {
        /* Delete the task that has overflown
         * NOTE: deletes the timer also */
        vTaskDelete( ph_owner_task );
    }
#   endif

#   if 1
    {
        taskENTER_CRITICAL();
        for (;;)
        {
            /* Do nothing as we just block the processor */
        }
    }
#   endif
}

/****END OF FILE****/