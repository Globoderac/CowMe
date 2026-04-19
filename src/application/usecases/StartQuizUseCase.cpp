#include "StartQuizUseCase.hpp"
#include <ranges>

namespace cowme::application {

StartQuizUseCase::StartQuizUseCase(
    std::shared_ptr<core::IQuizRepository> repo)
    : m_quizRepo{std::move(repo)}
{}

std::expected<QuizDto, core::RepoError>
StartQuizUseCase::execute(int64_t quizId) const
{
    auto result = m_quizRepo->getById(core::QuizId{quizId});
    if (!result) return std::unexpected(result.error());

    const auto& quiz = result.value();
    auto questionDtos = quiz.questions
        | std::views::transform([](const core::Question& q) {
            return QuestionDto{
                .id = q.id.value,
                .title = q.title,
                .codeSnippet = q.codeSnippet,
                .answerOptions = q.answerOptions,
                .correctAnswer = {},
                .explanation = {},
                .difficulty = q.difficulty,
                .strategyType = q.strategyType,
                .tags = q.tags,
                .category = q.category
            };
        });

    return QuizDto{
        .id = quiz.id.value,
        .name = quiz.name,
        .description = quiz.description,
        .questions = {questionDtos.begin(), questionDtos.end()}
    };
}

} // namespace cowme::application
