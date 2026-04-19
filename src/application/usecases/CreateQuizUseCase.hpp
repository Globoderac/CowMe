#pragma once
#include "interfaces/IQuizRepository.hpp"
#include "dto/QuizDto.hpp"
#include <expected>
#include <memory>

namespace cowme::application {

class CreateQuizUseCase {
public:
    explicit CreateQuizUseCase(std::shared_ptr<core::IQuizRepository> repo);
    [[nodiscard]] std::expected<int64_t, core::RepoError> execute(const QuizDto& dto) const;

private:
    std::shared_ptr<core::IQuizRepository> m_quizRepo;
};

} // namespace cowme::application
