#include "SubmitAnswerUseCase.hpp"

namespace cowme::application {

SubmitAnswerUseCase::SubmitAnswerUseCase(
    std::shared_ptr<core::IQuestionRepository> repo)
    : m_questionRepo{std::move(repo)}
{}

std::expected<SubmitAnswerResult, core::RepoError>
SubmitAnswerUseCase::execute(int64_t questionId, const QString& givenAnswer) const
{
    auto result = m_questionRepo->getById(core::QuestionId{questionId});
    if (!result) return std::unexpected(result.error());

    const auto& question = result.value();

    auto strategy = AnswerStrategyFactory::create(question.strategyType);
    bool correct = strategy->evaluate(givenAnswer, question.correctAnswer);

    return SubmitAnswerResult{
        .correct = correct,
        .explanation = question.explanation,
        .correctAnswer = question.correctAnswer
    };
}

} // namespace cowme::application
