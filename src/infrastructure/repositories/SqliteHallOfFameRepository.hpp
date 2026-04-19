#pragma once
#include "interfaces/IHallOfFameRepository.hpp"
#include "database/DatabaseManager.hpp"
#include <QSqlQuery>
#include <QSqlError>
#include <QDateTime>

namespace cowme::infrastructure {

class SqliteHallOfFameRepository : public core::IHallOfFameRepository {
public:
    explicit SqliteHallOfFameRepository(DatabaseManager& db) : m_db{db} {}

    std::expected<std::vector<core::HallOfFameEntry>, core::RepoError> getAll() const override {
        QSqlQuery query(m_db.database());
        query.prepare(R"(
            SELECT h.id, h.player_name, h.quiz_id, q.name, h.score, h.total_questions, h.elapsed_seconds, h.timestamp
            FROM hall_of_fame h
            LEFT JOIN quizzes q ON h.quiz_id = q.id
            ORDER BY h.score DESC, h.elapsed_seconds ASC, h.timestamp ASC
        )");
        if (!query.exec()) return std::unexpected(core::RepoError::DatabaseError);

        std::vector<core::HallOfFameEntry> entries;
        while (query.next()) {
            entries.push_back(rowToEntry(query));
        }
        return entries;
    }

    std::expected<std::vector<core::HallOfFameEntry>, core::RepoError> getByQuizId(int quizId) const override {
        QSqlQuery query(m_db.database());
        query.prepare(R"(
            SELECT h.id, h.player_name, h.quiz_id, q.name, h.score, h.total_questions, h.elapsed_seconds, h.timestamp
            FROM hall_of_fame h
            LEFT JOIN quizzes q ON h.quiz_id = q.id
            WHERE h.quiz_id = ?
            ORDER BY h.score DESC, h.elapsed_seconds ASC, h.timestamp ASC
        )");
        query.addBindValue(quizId);
        if (!query.exec()) return std::unexpected(core::RepoError::DatabaseError);

        std::vector<core::HallOfFameEntry> entries;
        while (query.next()) {
            entries.push_back(rowToEntry(query));
        }
        return entries;
    }

    std::expected<int, core::RepoError> add(const core::HallOfFameEntry& entry) override {
        QSqlQuery query(m_db.database());
        query.prepare(R"(
            INSERT INTO hall_of_fame (player_name, quiz_id, score, total_questions, elapsed_seconds, timestamp)
            VALUES (?, ?, ?, ?, ?, ?)
        )");
        query.addBindValue(entry.playerName);
        query.addBindValue(entry.quizId);
        query.addBindValue(entry.score);
        query.addBindValue(entry.totalQuestions);
        query.addBindValue(entry.elapsedSeconds);
        query.addBindValue(entry.timestamp.toString(Qt::ISODate));
        if (!query.exec()) return std::unexpected(core::RepoError::DatabaseError);
        return query.lastInsertId().toInt();
    }

    std::expected<void, core::RepoError> removeById(int id) override {
        QSqlQuery query(m_db.database());
        query.prepare("DELETE FROM hall_of_fame WHERE id = ?");
        query.addBindValue(id);
        if (!query.exec()) return std::unexpected(core::RepoError::DatabaseError);
        if (query.numRowsAffected() == 0) return std::unexpected(core::RepoError::NotFound);
        return {};
    }

private:
    DatabaseManager& m_db;

    static core::HallOfFameEntry rowToEntry(const QSqlQuery& q) {
        return {
            .id = q.value(0).toInt(),
            .playerName = q.value(1).toString(),
            .quizId = q.value(2).toInt(),
            .quizName = q.value(3).toString(),
            .score = q.value(4).toInt(),
            .totalQuestions = q.value(5).toInt(),
            .elapsedSeconds = q.value(6).toInt(),
            .timestamp = QDateTime::fromString(q.value(7).toString(), Qt::ISODate)
        };
    }
};

} // namespace cowme::infrastructure
