#pragma once
#include <cstdint>
#include <compare>

namespace cowme::core
{
struct QuestionId
{
	int64_t value{ 0 };
	auto operator<=>(const QuestionId&) const = default;
	[[nodiscard]] bool isValid() const noexcept { return value > 0; }
};

struct QuizId
{
	int64_t value{ 0 };
	auto operator<=>(const QuizId&) const = default;
	[[nodiscard]] bool isValid() const noexcept { return value > 0; }
};
}