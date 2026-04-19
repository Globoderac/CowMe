#pragma once
#include "entities/Question.hpp"
#include "value_objects/QuestionId.hpp"
#include <expected>
#include <vector>
#include <QString>

namespace cowme::core {

enum class RepoError {
    NotFound,
    DuplicateEntry,
    DatabaseError,
    ValidationError
};

class IQuestionRepository {
public:
    virtual ~IQuestionRepository() = default;

    virtual std::expected<Question, RepoError> getById(QuestionId id) const = 0;
    virtual std::expected<std::vector<Question>, RepoError> getAll() const = 0;
    virtual std::expected<std::vector<Question>, RepoError> getByCategory(const QString& category) const = 0;
    virtual std::expected<QuestionId, RepoError> add(const Question& question) = 0;
    virtual std::expected<void, RepoError> update(const Question& question) = 0;
    virtual std::expected<void, RepoError> remove(QuestionId id) = 0;
};

} // namespace cowme::core