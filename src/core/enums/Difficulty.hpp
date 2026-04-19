#pragma once
#include <cstdint>

namespace cowme::core
{
enum class Difficulty : uint8_t
{
	Easy,
	Medium,
	Hard,
	Expert
};
}