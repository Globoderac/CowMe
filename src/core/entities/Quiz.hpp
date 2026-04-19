#pragma once
#include "Question.hpp"
#include "value_objects/QuestionId.hpp"
#include <QString>
#include <vector>

namespace cowme::core {

struct Quiz {
    QuizId id;
    QString name;
    QString description;
    std::vector<Question> questions;

    [[nodiscard]] size_t questionCount() const noexcept {
        return questions.size();
    }

    [[nodiscard]] bool isEmpty() const noexcept {
        return questions.empty();
    }
};

} // namespace cowme::core
