#pragma once
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QString>
#include <mutex>

namespace cowme::infrastructure {

class DatabaseManager {
public:
    static DatabaseManager& instance() {
        static DatabaseManager mgr;
        return mgr;
    }

    [[nodiscard]] bool initialize(const QString& dbPath = "waskme.db") {
        std::lock_guard lock(m_mutex);
        if (m_initialized) return true;

        m_db = QSqlDatabase::addDatabase("QSQLITE");
        m_db.setDatabaseName(dbPath);

        if (!m_db.open()) return false;

        // Enable foreign key support for cascade deletes
        QSqlQuery pragmaQuery(m_db);
        pragmaQuery.exec("PRAGMA foreign_keys = ON");

        m_initialized = createTables();
        return m_initialized;
    }

    [[nodiscard]] QSqlDatabase& database() { return m_db; }
    [[nodiscard]] bool isInitialized() const noexcept { return m_initialized; }

private:
    DatabaseManager() = default;
    ~DatabaseManager() { if (m_db.isOpen()) m_db.close(); }
    DatabaseManager(const DatabaseManager&) = delete;
    DatabaseManager& operator=(const DatabaseManager&) = delete;

    [[nodiscard]] bool createTables() {
        QSqlQuery query(m_db);

        bool ok = query.exec(R"(
            CREATE TABLE IF NOT EXISTS quizzes (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                name TEXT NOT NULL,
                description TEXT DEFAULT ''
            )
        )");
        if (!ok) return false;

        ok = query.exec(R"(
            CREATE TABLE IF NOT EXISTS questions (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                quiz_id INTEGER NOT NULL,
                title TEXT NOT NULL,
                code_snippet TEXT DEFAULT '',
                correct_answer TEXT NOT NULL,
                explanation TEXT DEFAULT '',
                difficulty INTEGER DEFAULT 0,
                strategy_type INTEGER DEFAULT 0,
                category TEXT DEFAULT '',
                FOREIGN KEY (quiz_id) REFERENCES quizzes(id) ON DELETE CASCADE
            )
        )");
        if (!ok) return false;

        ok = query.exec(R"(
            CREATE TABLE IF NOT EXISTS answer_options (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                question_id INTEGER NOT NULL,
                option_text TEXT NOT NULL,
                FOREIGN KEY (question_id) REFERENCES questions(id) ON DELETE CASCADE
            )
        )");
        if (!ok) return false;

        ok = query.exec(R"(
            CREATE TABLE IF NOT EXISTS question_tags (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                question_id INTEGER NOT NULL,
                tag TEXT NOT NULL,
                FOREIGN KEY (question_id) REFERENCES questions(id) ON DELETE CASCADE
            )
        )");
        if (!ok) return false;

        ok = query.exec(R"(
            CREATE TABLE IF NOT EXISTS hall_of_fame (
                id INTEGER PRIMARY KEY AUTOINCREMENT,
                player_name TEXT NOT NULL,
                quiz_id INTEGER NOT NULL,
                score INTEGER NOT NULL,
                total_questions INTEGER NOT NULL,
                elapsed_seconds INTEGER NOT NULL,
                timestamp TEXT NOT NULL,
                FOREIGN KEY (quiz_id) REFERENCES quizzes(id) ON DELETE CASCADE
            )
        )");

        return ok;
    }

    QSqlDatabase m_db;
    std::mutex m_mutex;
    bool m_initialized{false};
};

} // namespace cowme::infrastructure
