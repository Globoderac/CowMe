#pragma once
#include <QString>
#include <QStringList>
#include "enums/Difficulty.hpp"
#include "enums/AnswerStrategy.hpp"

namespace cowme::application {

struct QuestionDto {
    int64_t id{0};
    QString title;
    QString codeSnippet;
    QStringList answerOptions;
    QString correctAnswer;
    QString explanation;
    core::Difficulty difficulty{core::Difficulty::Easy};
    core::AnswerStrategyType strategyType{core::AnswerStrategyType::ComboSelection};
    QStringList tags;
    QString category;
};

} // namespace cowme::application
