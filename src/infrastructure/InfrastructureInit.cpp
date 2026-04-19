#include "InfrastructureInit.hpp"
#include <stdexcept>

namespace cowme::infrastructure {

InfrastructureServices initializeInfrastructure(const QString& dbPath) {
    auto& db = DatabaseManager::instance();
    if (!db.initialize(dbPath)) {
        throw std::runtime_error("Failed to initialize database");
    }

    return InfrastructureServices{
        .questionRepo = std::make_shared<SqliteQuestionRepository>(db),
        .quizRepo = std::make_shared<SqliteQuizRepository>(db),
        .hallOfFameRepo = std::make_shared<SqliteHallOfFameRepository>(db)
    };
}

} // namespace cowme::infrastructure
