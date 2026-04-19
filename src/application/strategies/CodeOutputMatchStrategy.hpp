#pragma once
#include "IAnswerStrategy.hpp"

namespace cowme::application {

class CodeOutputMatchStrategy final : public IAnswerStrategy {
public:
    [[nodiscard]] bool evaluate(
        const QString& givenAnswer,
        const QString& correctAnswer) const override
    {
        auto normalize = [](QString s) {
            return s.replace("\r\n", "\n").trimmed();
        };
        return normalize(givenAnswer) == normalize(correctAnswer);
    }

    [[nodiscard]] QString strategyName() const override {
        return QStringLiteral("CodeOutputMatch");
    }
};

} // namespace cowme::application
