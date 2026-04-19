#pragma once
#include "interfaces/IHallOfFameRepository.hpp"
#include <memory>

namespace cowme::application {

class SaveScoreUseCase {
public:
    explicit SaveScoreUseCase(std::shared_ptr<core::IHallOfFameRepository> repo)
        : m_repo{std::move(repo)} {}

    std::expected<int, core::RepoError> execute(const core::HallOfFameEntry& entry) {
        return m_repo->add(entry);
    }

private:
    std::shared_ptr<core::IHallOfFameRepository> m_repo;
};

} // namespace cowme::application
