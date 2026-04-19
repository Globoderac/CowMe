#pragma once
#include "interfaces/IHallOfFameRepository.hpp"
#include <memory>

namespace cowme::application {

class GetLeaderboardUseCase {
public:
    explicit GetLeaderboardUseCase(std::shared_ptr<core::IHallOfFameRepository> repo)
        : m_repo{std::move(repo)} {}

    std::expected<std::vector<core::HallOfFameEntry>, core::RepoError> execute() {
        return m_repo->getAll();
    }

    std::expected<std::vector<core::HallOfFameEntry>, core::RepoError> executeForQuiz(int quizId) {
        return m_repo->getByQuizId(quizId);
    }

    [[nodiscard]] std::shared_ptr<core::IHallOfFameRepository> repo() const { return m_repo; }

private:
    std::shared_ptr<core::IHallOfFameRepository> m_repo;
};

} // namespace cowme::application
