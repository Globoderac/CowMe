#pragma once
#include "IAnswerStrategy.hpp"
#include "ComboSelectionStrategy.hpp"
#include "TextInputStrategy.hpp"
#include "CodeOutputMatchStrategy.hpp"
#include "enums/AnswerStrategy.hpp"
#include <memory>
#include <stdexcept>

namespace cowme::application {

class AnswerStrategyFactory {
public:
    [[nodiscard]] static std::unique_ptr<IAnswerStrategy> create(
        core::AnswerStrategyType type)
    {
        switch (type) {
        case core::AnswerStrategyType::ComboSelection:
            return std::make_unique<ComboSelectionStrategy>();
        case core::AnswerStrategyType::TextInput:
            return std::make_unique<TextInputStrategy>();
        case core::AnswerStrategyType::CodeOutputMatch:
            return std::make_unique<CodeOutputMatchStrategy>();
        }
        throw std::invalid_argument("Unknown AnswerStrategyType");
    }
};

} // namespace cowme::application
