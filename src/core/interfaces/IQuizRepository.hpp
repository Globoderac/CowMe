#pragma once
#include "entities/Quiz.hpp"
#include "interfaces/IQuestionRepository.hpp"
#include <expected>
#include <vector>

namespace cowme::core {

class IQuizRepository {
public:
    virtual ~IQuizRepository() = default;

    virtual std::expected<Quiz, RepoError> getById(QuizId id) const = 0;
    virtual std::expected<std::vector<Quiz>, RepoError> getAll() const = 0;
    virtual std::expected<QuizId, RepoError> add(const Quiz& quiz) = 0;
    virtual std::expected<void, RepoError> update(const Quiz& quiz) = 0;
    virtual std::expected<void, RepoError> remove(QuizId id) = 0;
};

} // namespace cowme::core