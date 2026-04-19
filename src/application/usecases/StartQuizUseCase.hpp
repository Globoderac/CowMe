#pragma once
#include "interfaces/IQuizRepository.hpp"
#include "dto/QuizDto.hpp"
#include <expected>
#include <memory>

namespace cowme::application {

class StartQuizUseCase {
public:
    explicit StartQuizUseCase(std::shared_ptr<core::IQuizRepository> repo);
    [[nodiscard]] std::expected<QuizDto, core::RepoError> execute(int64_t quizId) const;

private:
    std::shared_ptr<core::IQuizRepository> m_quizRepo;
};

} // namespace cowme::application
