#pragma once
#include "IAnswerStrategy.hpp"

namespace cowme::application {

class ComboSelectionStrategy final : public IAnswerStrategy {
public:
    [[nodiscard]] bool evaluate(
        const QString& givenAnswer,
        const QString& correctAnswer) const override
    {
        return givenAnswer.trimmed().compare(
            correctAnswer.trimmed(), Qt::CaseInsensitive) == 0;
    }

    [[nodiscard]] QString strategyName() const override {
        return QStringLiteral("ComboSelection");
    }
};

} // namespace cowme::application
