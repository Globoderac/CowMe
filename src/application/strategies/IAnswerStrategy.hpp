#pragma once
#include <QString>

namespace cowme::application {

class IAnswerStrategy {
public:
    virtual ~IAnswerStrategy() = default;

    [[nodiscard]] virtual bool evaluate(
        const QString& givenAnswer,
        const QString& correctAnswer) const = 0;

    [[nodiscard]] virtual QString strategyName() const = 0;
};

} // namespace cowme::application
