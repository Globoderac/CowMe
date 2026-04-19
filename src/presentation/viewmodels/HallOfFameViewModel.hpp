#pragma once
#include <QObject>
#include <QQmlEngine>
#include <QVariantList>
#include "usecases/GetLeaderboardUseCase.hpp"
#include "usecases/SaveScoreUseCase.hpp"
#include <memory>

namespace cowme::presentation {

class HallOfFameViewModel : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QVariantList entries READ entries NOTIFY entriesChanged)

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

    Q_INVOKABLE void loadAll() {
        m_entries.clear();
        auto result = m_getLeaderboard->execute();
        if (result) {
            for (const auto& e : result.value()) {
                m_entries.append(entryToVariant(e));
            }
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

signals:
    void entriesChanged();

private:
    std::shared_ptr<application::GetLeaderboardUseCase> m_getLeaderboard;
    std::shared_ptr<application::SaveScoreUseCase> m_saveScore;
    QVariantList m_entries;

    static QVariantMap entryToVariant(const core::HallOfFameEntry& e) {
        QVariantMap map;
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
