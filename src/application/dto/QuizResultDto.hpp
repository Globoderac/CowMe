#pragma once
#include <QString>
#include <vector>
#include <cstdint>

namespace cowme::application {

struct AnswerRecordDto {
    int64_t questionId{0};
    QString givenAnswer;
    bool correct{false};
};

struct QuizResultDto {
    int64_t quizId{0};
    std::vector<AnswerRecordDto> answers;
    int64_t durationSeconds{0};
    int correctCount{0};
    double scorePercent{0.0};
};

} // namespace cowme::application
