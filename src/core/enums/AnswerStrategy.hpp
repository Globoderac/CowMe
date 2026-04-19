#pragma once
#include <cstdint>

namespace cowme::core
{

enum class AnswerStrategyType : uint8_t
{
	ComboSelection,
	TextInput,
	CodeOutputMatch
};

} // namespace cowme::core