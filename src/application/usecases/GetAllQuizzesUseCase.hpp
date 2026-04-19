#pragma once
#include "interfaces/IQuizRepository.hpp"
#include "dto/QuizDto.hpp"
#include <expected>
#include <vector>
#include <memory>

namespace cowme::application {

class GetAllQuizzesUseCase {
public:
    explicit GetAllQuizzesUseCase(std::shared_ptr<core::IQuizRepository> repo);
    [[nodiscard]] std::expected<std::vector<QuizDto>, core::RepoError> execute() const;

private:
    std::shared_ptr<core::IQuizRepository> m_quizRepo;
};

} // namespace cowme::application
