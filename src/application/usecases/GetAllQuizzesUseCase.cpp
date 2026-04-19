#include "GetAllQuizzesUseCase.hpp"
#include <ranges>
#include <algorithm>

namespace cowme::application {

GetAllQuizzesUseCase::GetAllQuizzesUseCase(
    std::shared_ptr<core::IQuizRepository> repo)
    : m_quizRepo{std::move(repo)}
{}

std::expected<std::vector<QuizDto>, core::RepoError>
GetAllQuizzesUseCase::execute() const
{
    auto result = m_quizRepo->getAll();
    if (!result) return std::unexpected(result.error());

    auto toDto = [](const core::Quiz& q) -> QuizDto {
        auto questionDtos = q.questions
            | std::views::transform([](const core::Question& question) {
                return QuestionDto{
                    .id = question.id.value,
                    .title = question.title,
                    .codeSnippet = question.codeSnippet,
                    .answerOptions = question.answerOptions,
                    .correctAnswer = question.correctAnswer,
                    .explanation = question.explanation,
                    .difficulty = question.difficulty,
                    .strategyType = question.strategyType,
                    .tags = question.tags,
                    .category = question.category
                };
            });

        return QuizDto{
            .id = q.id.value,
            .name = q.name,
            .description = q.description,
            .questions = {questionDtos.begin(), questionDtos.end()}
        };
    };

    auto dtos = result.value()
        | std::views::transform(toDto);

    return std::vector<QuizDto>{dtos.begin(), dtos.end()};
}

} // namespace cowme::application
