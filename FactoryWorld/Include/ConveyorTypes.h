#pragma once
#include <cstdint>

using ConveyorId = uint32_t;
static constexpr ConveyorId INVALID_CONVEYOR = 0;
static constexpr int SLOT_COUNT = 4;

enum class ConveyorMode { AoS, SoA};