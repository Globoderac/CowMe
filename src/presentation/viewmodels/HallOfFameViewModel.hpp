#pragma once
#include <QObject>
#include <QQmlEngine>
#include <QVariantList>
#include <QVariantMap>
#include "usecases/GetLeaderboardUseCase.hpp"
#include "usecases/SaveScoreUseCase.hpp"
#include <memory>
#include <numeric>

namespace cowme::presentation {

class HallOfFameViewModel : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QVariantList entries READ entries NOTIFY entriesChanged)
    Q_PROPERTY(int totalAttempts READ totalAttempts NOTIFY entriesChanged)
    Q_PROPERTY(double averageScorePercent READ averageScorePercent NOTIFY entriesChanged)
    Q_PROPERTY(int bestScore READ bestScore NOTIFY entriesChanged)
    Q_PROPERTY(bool devMode READ devMode WRITE setDevMode NOTIFY devModeChanged)

public:
    explicit HallOfFameViewModel(
        std::shared_ptr<application::GetLeaderboardUseCase> getLeaderboard,
        std::shared_ptr<application::SaveScoreUseCase> saveScore,
        QObject* parent = nullptr)
        : QObject{parent}
        , m_getLeaderboard{std::move(getLeaderboard)}
        , m_saveScore{std::move(saveScore)}
    {}

    [[nodiscard]] QVariantList entries() const { return m_entries; }
    [[nodiscard]] int totalAttempts() const { return m_entries.size(); }
    [[nodiscard]] bool devMode() const { return m_devMode; }
    void setDevMode(bool on) { if (m_devMode != on) { m_devMode = on; emit devModeChanged(); } }

    [[nodiscard]] double averageScorePercent() const {
        if (m_entries.isEmpty()) return 0.0;
        double sum = 0.0;
        for (const auto& e : m_entries) {
            auto map = e.toMap();
            int total = map["totalQuestions"].toInt();
            if (total > 0)
                sum += 100.0 * map["score"].toInt() / total;
        }
        return sum / m_entries.size();
    }

    [[nodiscard]] int bestScore() const {
        if (m_entries.isEmpty()) return 0;
        return m_entries.first().toMap()["score"].toInt();
    }

    Q_INVOKABLE void loadAll() {
        m_entries.clear();
        auto result = m_getLeaderboard->execute();
        if (result) {
            for (const auto& e : result.value())
                m_entries.append(entryToVariant(e));
        }
        emit entriesChanged();
    }

    Q_INVOKABLE void saveScore(const QString& playerName, int quizId, const QString& quizName,
                               int score, int totalQuestions, int elapsedSeconds) {
        core::HallOfFameEntry entry{
            .playerName = playerName,
            .quizId = quizId,
            .quizName = quizName,
            .score = score,
            .totalQuestions = totalQuestions,
            .elapsedSeconds = elapsedSeconds,
            .timestamp = QDateTime::currentDateTime()
        };
        m_saveScore->execute(entry);
    }

    Q_INVOKABLE void deleteEntry(int entryId) {
        m_getLeaderboard->repo()->removeById(entryId);
        loadAll();
    }

signals:
    void entriesChanged();
    void devModeChanged();

private:
    std::shared_ptr<application::GetLeaderboardUseCase> m_getLeaderboard;
    std::shared_ptr<application::SaveScoreUseCase> m_saveScore;
    QVariantList m_entries;
    bool m_devMode{false};

    static QVariantMap entryToVariant(const core::HallOfFameEntry& e) {
        QVariantMap map;
        map[QStringLiteral("id")] = e.id;
        map[QStringLiteral("playerName")] = e.playerName;
        map[QStringLiteral("quizName")] = e.quizName;
        map[QStringLiteral("score")] = e.score;
        map[QStringLiteral("totalQuestions")] = e.totalQuestions;
        map[QStringLiteral("elapsedSeconds")] = e.elapsedSeconds;
        map[QStringLiteral("timestamp")] = e.timestamp.toString("yyyy-MM-dd hh:mm");
        return map;
    }
};

} // namespace cowme::presentation
