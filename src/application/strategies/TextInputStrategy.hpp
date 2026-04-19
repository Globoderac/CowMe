#pragma once
#include "IAnswerStrategy.hpp"

namespace cowme::application {

class TextInputStrategy final : public IAnswerStrategy {
public:
    [[nodiscard]] bool evaluate(
        const QString& givenAnswer,
        const QString& correctAnswer) const override
    {
        return givenAnswer.trimmed() == correctAnswer.trimmed();
    }

    [[nodiscard]] QString strategyName() const override {
        return QStringLiteral("TextInput");
    }
};

} // namespace cowme::application
