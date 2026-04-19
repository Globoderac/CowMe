#pragma once
#include <QObject>
#include <QQmlEngine>
#include <QString>
#include <QStringList>
#include <QVariantList>
#include <QTimer>
#include <QElapsedTimer>
#include "usecases/StartQuizUseCase.hpp"
#include "usecases/SubmitAnswerUseCase.hpp"
#include "usecases/SaveScoreUseCase.hpp"
#include "dto/QuizDto.hpp"
#include <memory>

namespace cowme::presentation {

class QuizSessionViewModel : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString quizName READ quizName NOTIFY quizChanged)
    Q_PROPERTY(int currentIndex READ currentIndex NOTIFY currentIndexChanged)
    Q_PROPERTY(int totalQuestions READ totalQuestions NOTIFY quizChanged)
    Q_PROPERTY(QString questionTitle READ questionTitle NOTIFY currentIndexChanged)
    Q_PROPERTY(QString codeSnippet READ codeSnippet NOTIFY currentIndexChanged)
    Q_PROPERTY(QStringList answerOptions READ answerOptions NOTIFY currentIndexChanged)
    Q_PROPERTY(bool isFinished READ isFinished NOTIFY isFinishedChanged)
    Q_PROPERTY(QString lastFeedback READ lastFeedback NOTIFY feedbackChanged)
    Q_PROPERTY(int score READ score NOTIFY scoreChanged)
    Q_PROPERTY(bool lastAnswerCorrect READ lastAnswerCorrect NOTIFY feedbackChanged)
    Q_PROPERTY(bool answerLocked READ answerLocked NOTIFY feedbackChanged)
    Q_PROPERTY(QVariantList answerResults READ answerResults NOTIFY isFinishedChanged)
    Q_PROPERTY(int remainingSeconds READ remainingSeconds NOTIFY remainingSecondsChanged)
    Q_PROPERTY(int timeLimitSeconds READ timeLimitSeconds NOTIFY quizChanged)
    Q_PROPERTY(bool timerEnabled READ timerEnabled NOTIFY quizChanged)
    Q_PROPERTY(bool timeExpired READ timeExpired NOTIFY timeExpiredChanged)
    Q_PROPERTY(int elapsedSeconds READ elapsedSeconds NOTIFY isFinishedChanged)
    Q_PROPERTY(QString playerName READ playerName WRITE setPlayerName NOTIFY playerNameChanged)
    Q_PROPERTY(int quizId READ quizId NOTIFY quizChanged)

public:
    explicit QuizSessionViewModel(
        std::shared_ptr<application::StartQuizUseCase> startQuiz,
        std::shared_ptr<application::SubmitAnswerUseCase> submitAnswer,
        std::shared_ptr<application::SaveScoreUseCase> saveScore,
        QObject* parent = nullptr);

    [[nodiscard]] QString quizName() const;
    [[nodiscard]] int currentIndex() const { return m_currentIndex; }
    [[nodiscard]] int totalQuestions() const;
    [[nodiscard]] QString questionTitle() const;
    [[nodiscard]] QString codeSnippet() const;
    [[nodiscard]] QStringList answerOptions() const;
    [[nodiscard]] bool isFinished() const { return m_finished; }
    [[nodiscard]] QString lastFeedback() const { return m_lastFeedback; }
    [[nodiscard]] int score() const { return m_score; }
    [[nodiscard]] bool lastAnswerCorrect() const { return m_lastAnswerCorrect; }
    [[nodiscard]] bool answerLocked() const { return m_answerLocked; }
    [[nodiscard]] QVariantList answerResults() const { return m_answerResults; }
    [[nodiscard]] int elapsedSeconds() const { return m_elapsedSeconds; }
    [[nodiscard]] QString playerName() const { return m_playerName; }
    [[nodiscard]] int quizId() const { return m_quizId; }
    void setPlayerName(const QString& name) { if (m_playerName != name) { m_playerName = name; emit playerNameChanged(); } }

    Q_INVOKABLE void startQuiz(int quizId);
    Q_INVOKABLE void startQuizWithCount(int quizId, int count);
    Q_INVOKABLE void startQuizWithCountAndTime(int quizId, int count, int timeLimitSecs);
    Q_INVOKABLE int availableQuestionCount(int quizId);
    Q_INVOKABLE void submitAnswer(const QString& answer);
    Q_INVOKABLE void nextQuestion();

    [[nodiscard]] int remainingSeconds() const { return m_remainingSeconds; }
    [[nodiscard]] int timeLimitSeconds() const { return m_timeLimitSeconds; }
    [[nodiscard]] bool timerEnabled() const { return m_timeLimitSeconds > 0; }
    [[nodiscard]] bool timeExpired() const { return m_timeExpired; }

signals:
    void quizChanged();
    void currentIndexChanged();
    void isFinishedChanged();
    void feedbackChanged();
    void scoreChanged();
    void remainingSecondsChanged();
    void timeExpiredChanged();
    void playerNameChanged();

private:
    std::shared_ptr<application::StartQuizUseCase> m_startQuiz;
    std::shared_ptr<application::SubmitAnswerUseCase> m_submitAnswer;
    std::shared_ptr<application::SaveScoreUseCase> m_saveScore;
    application::QuizDto m_quiz;
    int m_currentIndex{0};
    bool m_finished{false};
    QString m_lastFeedback;
    int m_score{0};
    bool m_lastAnswerCorrect{false};
    bool m_answerLocked{false};
    QVariantList m_answerResults;
    QTimer m_timer;
    QElapsedTimer m_elapsedTimer;
    int m_remainingSeconds{0};
    int m_timeLimitSeconds{0};
    bool m_timeExpired{false};
    int m_elapsedSeconds{0};
    QString m_playerName;
    int m_quizId{0};

    void initQuizState();
    void onTimerTick();
    void finishQuiz();
};

} // namespace cowme::presentation
