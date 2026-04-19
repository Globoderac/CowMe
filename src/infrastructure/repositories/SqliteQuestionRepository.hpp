#pragma once
#include "interfaces/IQuestionRepository.hpp"
#include "database/DatabaseManager.hpp"
#include <QSqlQuery>
#include <QSqlError>

namespace cowme::infrastructure {

class SqliteQuestionRepository final : public core::IQuestionRepository {
public:
    explicit SqliteQuestionRepository(DatabaseManager& db)
        : m_db{db} {}

    std::expected<core::Question, core::RepoError> getById(core::QuestionId id) const override {
        QSqlQuery query(m_db.database());
        query.prepare("SELECT id, title, code_snippet, correct_answer, explanation, "
                      "difficulty, strategy_type, category FROM questions WHERE id = ?");
        query.addBindValue(id.value);
        if (!query.exec() || !query.next())
            return std::unexpected(core::RepoError::NotFound);

        return buildQuestion(query);
    }

    std::expected<std::vector<core::Question>, core::RepoError> getAll() const override {
        QSqlQuery query(m_db.database());
        if (!query.exec("SELECT id, title, code_snippet, correct_answer, explanation, "
                        "difficulty, strategy_type, category FROM questions"))
            return std::unexpected(core::RepoError::DatabaseError);

        std::vector<core::Question> results;
        while (query.next())
            results.push_back(buildQuestion(query));
        return results;
    }

    std::expected<std::vector<core::Question>, core::RepoError> getByCategory(
        const QString& category) const override
    {
        QSqlQuery query(m_db.database());
        query.prepare("SELECT id, title, code_snippet, correct_answer, explanation, "
                      "difficulty, strategy_type, category FROM questions WHERE category = ?");
        query.addBindValue(category);
        if (!query.exec())
            return std::unexpected(core::RepoError::DatabaseError);

        std::vector<core::Question> results;
        while (query.next())
            results.push_back(buildQuestion(query));
        return results;
    }

    std::expected<core::QuestionId, core::RepoError> add(const core::Question& question) override {
        if (!question.isValid())
            return std::unexpected(core::RepoError::ValidationError);

        QSqlQuery query(m_db.database());
        query.prepare("INSERT INTO questions (quiz_id, title, code_snippet, correct_answer, "
                      "explanation, difficulty, strategy_type, category) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
        query.addBindValue(0); // quiz_id set later when associating
        query.addBindValue(question.title);
        query.addBindValue(question.codeSnippet);
        query.addBindValue(question.correctAnswer);
        query.addBindValue(question.explanation);
        query.addBindValue(static_cast<int>(question.difficulty));
        query.addBindValue(static_cast<int>(question.strategyType));
        query.addBindValue(question.category);

        if (!query.exec())
            return std::unexpected(core::RepoError::DatabaseError);

        auto questionId = core::QuestionId{query.lastInsertId().toLongLong()};

        saveOptions(questionId, question.answerOptions);
        saveTags(questionId, question.tags);

        return questionId;
    }

    std::expected<void, core::RepoError> update(const core::Question& question) override {
        if (!question.id.isValid())
            return std::unexpected(core::RepoError::ValidationError);

        QSqlQuery query(m_db.database());
        query.prepare("UPDATE questions SET title=?, code_snippet=?, correct_answer=?, "
                      "explanation=?, difficulty=?, strategy_type=?, category=? WHERE id=?");
        query.addBindValue(question.title);
        query.addBindValue(question.codeSnippet);
        query.addBindValue(question.correctAnswer);
        query.addBindValue(question.explanation);
        query.addBindValue(static_cast<int>(question.difficulty));
        query.addBindValue(static_cast<int>(question.strategyType));
        query.addBindValue(question.category);
        query.addBindValue(question.id.value);

        if (!query.exec())
            return std::unexpected(core::RepoError::DatabaseError);

        clearOptions(question.id);
        clearTags(question.id);
        saveOptions(question.id, question.answerOptions);
        saveTags(question.id, question.tags);

        return {};
    }

    std::expected<void, core::RepoError> remove(core::QuestionId id) override {
        QSqlQuery query(m_db.database());
        query.prepare("DELETE FROM questions WHERE id = ?");
        query.addBindValue(id.value);
        if (!query.exec())
            return std::unexpected(core::RepoError::DatabaseError);
        return {};
    }

private:
    core::Question buildQuestion(QSqlQuery& query) const {
        core::Question q;
        q.id = core::QuestionId{query.value(0).toLongLong()};
        q.title = query.value(1).toString();
        q.codeSnippet = query.value(2).toString();
        q.correctAnswer = query.value(3).toString();
        q.explanation = query.value(4).toString();
        q.difficulty = static_cast<core::Difficulty>(query.value(5).toInt());
        q.strategyType = static_cast<core::AnswerStrategyType>(query.value(6).toInt());
        q.category = query.value(7).toString();
        q.answerOptions = loadOptions(q.id);
        q.tags = loadTags(q.id);
        return q;
    }

    QStringList loadOptions(core::QuestionId id) const {
        QSqlQuery query(m_db.database());
        query.prepare("SELECT option_text FROM answer_options WHERE question_id = ?");
        query.addBindValue(id.value);
        QStringList options;
        if (query.exec()) {
            while (query.next()) options << query.value(0).toString();
        }
        return options;
    }

    QStringList loadTags(core::QuestionId id) const {
        QSqlQuery query(m_db.database());
        query.prepare("SELECT tag FROM question_tags WHERE question_id = ?");
        query.addBindValue(id.value);
        QStringList tags;
        if (query.exec()) {
            while (query.next()) tags << query.value(0).toString();
        }
        return tags;
    }

    void saveOptions(core::QuestionId id, const QStringList& options) {
        for (const auto& opt : options) {
            QSqlQuery query(m_db.database());
            query.prepare("INSERT INTO answer_options (question_id, option_text) VALUES (?, ?)");
            query.addBindValue(id.value);
            query.addBindValue(opt);
            query.exec();
        }
    }

    void saveTags(core::QuestionId id, const QStringList& tags) {
        for (const auto& tag : tags) {
            QSqlQuery query(m_db.database());
            query.prepare("INSERT INTO question_tags (question_id, tag) VALUES (?, ?)");
            query.addBindValue(id.value);
            query.addBindValue(tag);
            query.exec();
        }
    }

    void clearOptions(core::QuestionId id) {
        QSqlQuery query(m_db.database());
        query.prepare("DELETE FROM answer_options WHERE question_id = ?");
        query.addBindValue(id.value);
        query.exec();
    }

    void clearTags(core::QuestionId id) {
        QSqlQuery query(m_db.database());
        query.prepare("DELETE FROM question_tags WHERE question_id = ?");
        query.addBindValue(id.value);
        query.exec();
    }

    DatabaseManager& m_db;
};

} // namespace cowme::infrastructure
