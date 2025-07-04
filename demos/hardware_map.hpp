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

#pragma once

#include <libhal/functional.hpp>
#include <libhal/output_pin.hpp>
#include <libhal/steady_clock.hpp>

struct hardware_map_t
{
  hal::output_pin* led;
  hal::steady_clock* clock;
  hal::callback<void()> reset;
};

// Each application function must implement this function
void
application(hardware_map_t& p_map);

// Platform source files must be implement this function
hardware_map_t
initialize_platform();
