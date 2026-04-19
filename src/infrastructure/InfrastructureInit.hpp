#pragma once
#include "database/DatabaseManager.hpp"
#include "repositories/SqliteQuestionRepository.hpp"
#include "repositories/SqliteQuizRepository.hpp"
#include "repositories/SqliteHallOfFameRepository.hpp"
#include <memory>

namespace cowme::infrastructure {

struct InfrastructureServices {
    std::shared_ptr<core::IQuestionRepository> questionRepo;
    std::shared_ptr<core::IQuizRepository> quizRepo;
    std::shared_ptr<core::IHallOfFameRepository> hallOfFameRepo;
};

[[nodiscard]] InfrastructureServices initializeInfrastructure(const QString& dbPath = "waskme.db");

} // namespace cowme::infrastructure
