#pragma once
#include "interfaces/IQuizRepository.hpp"
#include "database/DatabaseManager.hpp"
#include "repositories/SqliteQuestionRepository.hpp"
#include <QSqlQuery>
#include <QSqlError>

namespace cowme::infrastructure {

class SqliteQuizRepository final : public core::IQuizRepository {
public:
    explicit SqliteQuizRepository(DatabaseManager& db)
        : m_db{db} {}

    std::expected<core::Quiz, core::RepoError> getById(core::QuizId id) const override {
        QSqlQuery query(m_db.database());
        query.prepare("SELECT id, name, description FROM quizzes WHERE id = ?");
        query.addBindValue(id.value);
        if (!query.exec() || !query.next())
            return std::unexpected(core::RepoError::NotFound);

        core::Quiz quiz;
        quiz.id = core::QuizId{query.value(0).toLongLong()};
        quiz.name = query.value(1).toString();
        quiz.description = query.value(2).toString();
        quiz.questions = loadQuestions(quiz.id);
        return quiz;
    }

    std::expected<std::vector<core::Quiz>, core::RepoError> getAll() const override {
        QSqlQuery query(m_db.database());
        if (!query.exec("SELECT id, name, description FROM quizzes"))
            return std::unexpected(core::RepoError::DatabaseError);

        std::vector<core::Quiz> results;
        while (query.next()) {
            core::Quiz quiz;
            quiz.id = core::QuizId{query.value(0).toLongLong()};
            quiz.name = query.value(1).toString();
            quiz.description = query.value(2).toString();
            quiz.questions = loadQuestions(quiz.id);
            results.push_back(std::move(quiz));
        }
        return results;
    }

    std::expected<core::QuizId, core::RepoError> add(const core::Quiz& quiz) override {
        QSqlQuery query(m_db.database());
        query.prepare("INSERT INTO quizzes (name, description) VALUES (?, ?)");
        query.addBindValue(quiz.name);
        query.addBindValue(quiz.description);

        if (!query.exec())
            return std::unexpected(core::RepoError::DatabaseError);

        auto quizId = core::QuizId{query.lastInsertId().toLongLong()};

        for (const auto& question : quiz.questions) {
            addQuestionToQuiz(quizId, question);
        }

        return quizId;
    }

    std::expected<void, core::RepoError> update(const core::Quiz& quiz) override {
        if (!quiz.id.isValid())
            return std::unexpected(core::RepoError::ValidationError);

        QSqlQuery query(m_db.database());
        query.prepare("UPDATE quizzes SET name=?, description=? WHERE id=?");
        query.addBindValue(quiz.name);
        query.addBindValue(quiz.description);
        query.addBindValue(quiz.id.value);

        if (!query.exec())
            return std::unexpected(core::RepoError::DatabaseError);

        // Remove old questions and re-insert
        QSqlQuery del(m_db.database());
        del.prepare("DELETE FROM questions WHERE quiz_id = ?");
        del.addBindValue(quiz.id.value);
        del.exec();

        for (const auto& question : quiz.questions) {
            addQuestionToQuiz(quiz.id, question);
        }

        return {};
    }

    std::expected<void, core::RepoError> remove(core::QuizId id) override {
        QSqlQuery query(m_db.database());
        query.prepare("DELETE FROM quizzes WHERE id = ?");
        query.addBindValue(id.value);
        if (!query.exec())
            return std::unexpected(core::RepoError::DatabaseError);
        return {};
    }

private:
    std::vector<core::Question> loadQuestions(core::QuizId quizId) const {
        QSqlQuery query(m_db.database());
        query.prepare("SELECT id, title, code_snippet, correct_answer, explanation, "
                      "difficulty, strategy_type, category FROM questions WHERE quiz_id = ?");
        query.addBindValue(quizId.value);

        std::vector<core::Question> questions;
        if (query.exec()) {
            while (query.next()) {
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
                questions.push_back(std::move(q));
            }
        }
        return questions;
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

    void addQuestionToQuiz(core::QuizId quizId, const core::Question& question) {
        QSqlQuery query(m_db.database());
        query.prepare("INSERT INTO questions (quiz_id, title, code_snippet, correct_answer, "
                      "explanation, difficulty, strategy_type, category) "
                      "VALUES (?, ?, ?, ?, ?, ?, ?, ?)");
        query.addBindValue(quizId.value);
        query.addBindValue(question.title);
        query.addBindValue(question.codeSnippet);
        query.addBindValue(question.correctAnswer);
        query.addBindValue(question.explanation);
        query.addBindValue(static_cast<int>(question.difficulty));
        query.addBindValue(static_cast<int>(question.strategyType));
        query.addBindValue(question.category);

        if (query.exec()) {
            auto qId = core::QuestionId{query.lastInsertId().toLongLong()};
            for (const auto& opt : question.answerOptions) {
                QSqlQuery optQ(m_db.database());
                optQ.prepare("INSERT INTO answer_options (question_id, option_text) VALUES (?, ?)");
                optQ.addBindValue(qId.value);
                optQ.addBindValue(opt);
                optQ.exec();
            }
            for (const auto& tag : question.tags) {
                QSqlQuery tagQ(m_db.database());
                tagQ.prepare("INSERT INTO question_tags (question_id, tag) VALUES (?, ?)");
                tagQ.addBindValue(qId.value);
                tagQ.addBindValue(tag);
                tagQ.exec();
            }
        }
    }

    DatabaseManager& m_db;
};

} // namespace cowme::infrastructure
