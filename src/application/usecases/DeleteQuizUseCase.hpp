#pragma once
#include "interfaces/IQuizRepository.hpp"
#include <expected>
#include <memory>

namespace cowme::application {

class DeleteQuizUseCase {
public:
    explicit DeleteQuizUseCase(std::shared_ptr<core::IQuizRepository> repo)
        : m_quizRepo{std::move(repo)} {}

    [[nodiscard]] std::expected<void, core::RepoError> execute(int64_t quizId) const {
        return m_quizRepo->remove(core::QuizId{quizId});
    }

private:
    std::shared_ptr<core::IQuizRepository> m_quizRepo;
};

} // namespace cowme::application
