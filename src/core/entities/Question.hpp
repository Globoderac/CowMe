#pragma once
#include "value_objects/QuestionId.hpp"
#include "enums/Difficulty.hpp"
#include "enums/AnswerStrategy.hpp"
#include <QString>
#include <QStringList>
#include <vector>
#include <optional>

namespace cowme::core {

struct Question {
    QuestionId id;
    QString title;
    QString codeSnippet;
    QStringList answerOptions;
    QString correctAnswer;
    QString explanation;
    Difficulty difficulty{Difficulty::Easy};
    AnswerStrategyType strategyType{AnswerStrategyType::ComboSelection};
    QStringList tags;
    QString category;

    [[nodiscard]] bool isValid() const noexcept {
        return !title.isEmpty() && !correctAnswer.isEmpty();
    }
};

} // namespace cowme::core