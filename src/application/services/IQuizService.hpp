#pragma once
#include "dto/QuizDto.hpp"
#include "dto/QuizResultDto.hpp"
#include <expected>
#include <vector>
#include <QString>
#include "interfaces/IQuestionRepository.hpp"

namespace cowme::application {

class IQuizService {
public:
    virtual ~IQuizService() = default;

    virtual std::expected<QuizDto, core::RepoError> startQuiz(int64_t quizId) = 0;
    virtual std::expected<bool, core::RepoError> submitAnswer(
        int64_t questionId, const QString& answer) = 0;
    virtual std::expected<QuizResultDto, core::RepoError> finishQuiz() = 0;
    virtual std::expected<std::vector<QuizDto>, core::RepoError> getAllQuizzes() = 0;
};

} // namespace cowme::application
