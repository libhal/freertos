// Copyright 2024 Khalil Estell
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

#include <libhal-arm-mcu/dwt_counter.hpp>
#include <libhal-arm-mcu/interrupt.hpp>
#include <libhal-arm-mcu/lpc40/clock.hpp>
#include <libhal-arm-mcu/lpc40/interrupt.hpp>
#include <libhal-arm-mcu/lpc40/output_pin.hpp>
#include <libhal-arm-mcu/system_control.hpp>
#include <libhal-arm-mcu/systick_timer.hpp>
#include <libhal-util/steady_clock.hpp>

#include "../hardware_map.hpp"

extern "C" void
xPortSysTickHandler();
extern "C" void
xPortPendSVHandler();
extern "C" void
vPortSVCHandler();

using namespace hal::literals;
using namespace std::literals;

void
hard_fault_handler()
{
  while (true) {
    continue;
  }
}
void
memory_management_handler()
{
  while (true) {
    continue;
  }
}
void
bus_fault_handler()
{
  while (true) {
    continue;
  }
}
void
usage_fault_handler()
{
  while (true) {
    continue;
  }
}

hal::cortex_m::dwt_counter* global_steady_clock = nullptr;

extern "C"
{
  void vPortSetupTimerInterrupt(void)
  {
    auto cpu_frequency = hal::lpc40::get_frequency(hal::lpc40::peripheral::cpu);
    static hal::cortex_m::systick_timer systick(cpu_frequency);
    try {
      systick.schedule(xPortSysTickHandler, 1ms);
      // Override the handler set by systick to ensure its set correctly.
      // systick schedule may wrap the function call which is problematic.
      // But now its set to the correct period of 1ms
      hal::cortex_m::enable_interrupt(hal::value(hal::cortex_m::irq::systick),
                                      xPortSysTickHandler);
    } catch (...) {
      hal::halt();
    }
  }

  unsigned int _freertos_get_high_resolution_timer_count(void)
  {
    return global_steady_clock->uptime();
  }
  void _freertos_configure_high_resolution_timer(void) {}

  unsigned int idle_percent = 0;

  void vApplicationIdleHook()
  {
    while (true) {
      idle_percent = ulTaskGetIdleRunTimePercent();
      asm volatile("wfi");
    }
  }
}

hardware_map_t
initialize_platform()
{
  // Change the input frequency to match the frequency of the crystal attached
  // to the external OSC pins.
  hal::lpc40::maximum(10.0_MHz);

  auto cpu_frequency = hal::lpc40::get_frequency(hal::lpc40::peripheral::cpu);

  static hal::cortex_m::dwt_counter steady_clock(cpu_frequency);
  static hal::lpc40::output_pin led(1, 10);

  global_steady_clock = &steady_clock;

  hal::lpc40::initialize_interrupts();

  hal::cortex_m::enable_interrupt(hal::value(hal::cortex_m::irq::hard_fault),
                                  hard_fault_handler);
  hal::cortex_m::enable_interrupt(
    hal::value(hal::cortex_m::irq::memory_management_fault),
    memory_management_handler);
  hal::cortex_m::enable_interrupt(hal::value(hal::cortex_m::irq::bus_fault),
                                  bus_fault_handler);
  hal::cortex_m::enable_interrupt(hal::value(hal::cortex_m::irq::usage_fault),
                                  usage_fault_handler);

  hal::cortex_m::enable_interrupt(hal::value(hal::cortex_m::irq::software_call),
                                  vPortSVCHandler);
  hal::cortex_m::enable_interrupt(hal::value(hal::cortex_m::irq::pend_sv),
                                  xPortPendSVHandler);

  return {
    .led = &led,
    .clock = &steady_clock,
    .reset = []() { hal::cortex_m::reset(); },
  };
}
