#pragma once
#include "value_objects/QuestionId.hpp"
#include <QString>
#include <vector>
#include <chrono>
#include <algorithm>

namespace cowme::core {

struct AnswerRecord {
    QuestionId questionId;
    QString givenAnswer;
    bool correct{false};
};

struct QuizResult {
    QuizId quizId;
    std::vector<AnswerRecord> answers;
    std::chrono::seconds duration{0};

    [[nodiscard]] int correctCount() const noexcept {
        return static_cast<int>(
            std::ranges::count_if(answers, &AnswerRecord::correct));
    }

    [[nodiscard]] double scorePercent() const noexcept {
        if (answers.empty()) return 0.0;
        return 100.0 * correctCount() / static_cast<double>(answers.size());
    }
};

} // namespace cowme::core