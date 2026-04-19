#pragma once
#include "interfaces/IQuestionRepository.hpp"
#include "strategies/AnswerStrategyFactory.hpp"
#include "dto/QuizResultDto.hpp"
#include <expected>
#include <memory>

namespace cowme::application {

struct SubmitAnswerResult {
    bool correct{false};
    QString explanation;
    QString correctAnswer;
};

class SubmitAnswerUseCase {
public:
    explicit SubmitAnswerUseCase(std::shared_ptr<core::IQuestionRepository> repo);
    [[nodiscard]] std::expected<SubmitAnswerResult, core::RepoError> execute(
        int64_t questionId, const QString& givenAnswer) const;

private:
    std::shared_ptr<core::IQuestionRepository> m_questionRepo;
};

} // namespace cowme::application
