#pragma once
#include "entities/HallOfFameEntry.hpp"
#include "interfaces/IQuestionRepository.hpp"
#include <expected>
#include <vector>

namespace cowme::core {

class IHallOfFameRepository {
public:
    virtual ~IHallOfFameRepository() = default;

    virtual std::expected<std::vector<HallOfFameEntry>, RepoError> getAll() const = 0;
    virtual std::expected<std::vector<HallOfFameEntry>, RepoError> getByQuizId(int quizId) const = 0;
    virtual std::expected<int, RepoError> add(const HallOfFameEntry& entry) = 0;
};

} // namespace cowme::core
