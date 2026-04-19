#pragma once
#include "interfaces/IQuizRepository.hpp"
#include "dto/QuizDto.hpp"
#include "dto/QuestionDto.hpp"
#include <expected>
#include <memory>
#include <ranges>

namespace cowme::application {

class UpdateQuizUseCase {
public:
    explicit UpdateQuizUseCase(std::shared_ptr<core::IQuizRepository> repo)
        : m_quizRepo{std::move(repo)} {}

    [[nodiscard]] std::expected<void, core::RepoError> execute(const QuizDto& dto) const {
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
            .id = core::QuizId{dto.id},
            .name = dto.name,
            .description = dto.description,
            .questions = {std::ranges::begin(questions), std::ranges::end(questions)}
        };

        return m_quizRepo->update(quiz);
    }

private:
    std::shared_ptr<core::IQuizRepository> m_quizRepo;
};

} // namespace cowme::application
