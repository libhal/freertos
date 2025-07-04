// Copyright 2024 - 2025 Khalil Estell and the libhal contributors
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//      http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.

#include <FreeRTOS.h>
#include <task.h>

constexpr size_t stack_size = 500;
/* Structure that will hold the TCB of the task being created. */
StaticTask_t xTaskBuffer;

/* Buffer that the task being created will use as its stack.  Note this is
an array of StackType_t variables.  The size of StackType_t is dependent on
the RTOS port. */
StackType_t xStack[stack_size];

/* Function that implements the task being created. */
void
vTaskCode(void* pvParameters)
{
  /* The parameter value is expected to be 1 as 1 is passed in the
  pvParameters value in the call to xTaskCreateStatic(). */
  configASSERT((uint32_t)pvParameters == 1UL);

  for (;;) {
    /* Task code goes here. */
  }
}

int
main()
{
  TaskHandle_t xHandle = NULL;

  /* Create the task without using any dynamic memory allocation. */
  xHandle = xTaskCreateStatic(
    vTaskCode,        /* Function that implements the task. */
    "NAME",           /* Text name for the task. */
    stack_size,       /* Number of indexes in the xStack array. */
    (void*)1,         /* Parameter passed into the task. */
    tskIDLE_PRIORITY, /* Priority at which the task is created. */
    xStack,           /* Array to use as the task's stack. */
    &xTaskBuffer);    /* Variable to hold the task's data structure. */

  /* Start the RTOS scheduler, this function should not return as it causes the
  execution context to change from main() to one of the created tasks. */
  vTaskStartScheduler();

  return 0;
}

extern "C"
{
  /* configSUPPORT_STATIC_ALLOCATION is set to 1, so the application must
  provide an implementation of vApplicationGetIdleTaskMemory() to provide the
  memory that is used by the Idle task. */
  void vApplicationGetIdleTaskMemory(StaticTask_t** ppxIdleTaskTCBBuffer,
                                     StackType_t** ppxIdleTaskStackBuffer,
                                     uint32_t* pulIdleTaskStackSize)
  {
    /* If the buffers to be provided to the Idle task are declared inside this
    function then they must be declared static - otherwise they will be
    allocated on the stack and so not exists after this function exits. */
    static StaticTask_t xIdleTaskTCB;
    static StackType_t uxIdleTaskStack[configMINIMAL_STACK_SIZE];

    /* Pass out a pointer to the StaticTask_t structure in which the Idle task's
    state will be stored. */
    *ppxIdleTaskTCBBuffer = &xIdleTaskTCB;

    /* Pass out the array that will be used as the Idle task's stack. */
    *ppxIdleTaskStackBuffer = uxIdleTaskStack;

    /* Pass out the size of the array pointed to by *ppxIdleTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configMINIMAL_STACK_SIZE is specified in words, not bytes. */
    *pulIdleTaskStackSize = configMINIMAL_STACK_SIZE;
  }
  /*-----------------------------------------------------------*/

  /* configSUPPORT_STATIC_ALLOCATION and configUSE_TIMERS are both set to 1, so
  the application must provide an implementation of
  vApplicationGetTimerTaskMemory() to provide the memory that is used by the
  Timer service task. */
  void vApplicationGetTimerTaskMemory(StaticTask_t** ppxTimerTaskTCBBuffer,
                                      StackType_t** ppxTimerTaskStackBuffer,
                                      uint32_t* pulTimerTaskStackSize)
  {
    /* If the buffers to be provided to the Timer task are declared inside this
    function then they must be declared static - otherwise they will be
    allocated on the stack and so not exists after this function exits. */
    static StaticTask_t xTimerTaskTCB;
    static StackType_t uxTimerTaskStack[configTIMER_TASK_STACK_DEPTH];

    /* Pass out a pointer to the StaticTask_t structure in which the Timer
    task's state will be stored. */
    *ppxTimerTaskTCBBuffer = &xTimerTaskTCB;

    /* Pass out the array that will be used as the Timer task's stack. */
    *ppxTimerTaskStackBuffer = uxTimerTaskStack;

    /* Pass out the size of the array pointed to by *ppxTimerTaskStackBuffer.
    Note that, as the array is necessarily of type StackType_t,
    configTIMER_TASK_STACK_DEPTH is specified in words, not bytes. */
    *pulTimerTaskStackSize = configTIMER_TASK_STACK_DEPTH;
  }

  unsigned int _freertos_get_high_resolution_timer_count(void)
  {
    return 0;
  }
  void _freertos_configure_high_resolution_timer(void) {}
}