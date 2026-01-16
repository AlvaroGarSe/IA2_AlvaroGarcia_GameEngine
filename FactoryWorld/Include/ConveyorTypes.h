#pragma once
#include <cstdint>

using ConveyorId = uint32_t;

static constexpr ConveyorId INVALID_CONVEYOR = 0; // If ConveyorId is 0 there is no conveyor there
static constexpr int SLOT_COUNT = 4; // Capacity of the conveyor

enum class ConveyorMode { AoS, SoA};