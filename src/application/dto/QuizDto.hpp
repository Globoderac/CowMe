#pragma once
#include "QuestionDto.hpp"
#include <QString>
#include <vector>

namespace cowme::application {

struct QuizDto {
    int64_t id{0};
    QString name;
    QString description;
    std::vector<QuestionDto> questions;

    [[nodiscard]] size_t questionCount() const noexcept {
        return questions.size();
    }
};

} // namespace cowme::application
