#include "CreateQuizUseCase.hpp"
#include <ranges>

namespace cowme::application {

CreateQuizUseCase::CreateQuizUseCase(
    std::shared_ptr<core::IQuizRepository> repo)
    : m_quizRepo{std::move(repo)}
{}

std::expected<int64_t, core::RepoError>
CreateQuizUseCase::execute(const QuizDto& dto) const
{
    auto questions = dto.questions
        | std::views::transform([](const QuestionDto& qd) {
            return core::Question{
                .id = core::QuestionId{qd.id},
                .title = qd.title,
                .codeSnippet = qd.codeSnippet,
                .answerOptions = qd.answerOptions,
                .correctAnswer = qd.correctAnswer,
                .explanation = qd.explanation,
                .difficulty = qd.difficulty,
                .strategyType = qd.strategyType,
                .tags = qd.tags,
                .category = qd.category
            };
        });

    core::Quiz quiz{
        .id = core::QuizId{0},
        .name = dto.name,
        .description = dto.description,
        .questions = {questions.begin(), questions.end()}
    };

    auto result = m_quizRepo->add(quiz);
    if (!result) return std::unexpected(result.error());
    return result.value().value;
}

} // namespace cowme::application
