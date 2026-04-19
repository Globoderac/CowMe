#include "QuizSessionViewModel.hpp"
#include <QVariantMap>
#include <QDateTime>
#include <algorithm>
#include <random>
#include <ranges>

namespace cowme::presentation {

namespace {

void selectAndShuffleQuestions(std::vector<application::QuestionDto>& questions, int count) {
    std::random_device rd;
    std::mt19937 rng(rd());

    if (count <= 0 || count >= static_cast<int>(questions.size())) {
        std::ranges::shuffle(questions, rng);
        return;
    }

    std::vector<application::QuestionDto> easy, medium, hard, expert;
    for (auto& q : questions) {
        switch (q.difficulty) {
            case core::Difficulty::Easy:   easy.push_back(std::move(q)); break;
            case core::Difficulty::Medium: medium.push_back(std::move(q)); break;
            case core::Difficulty::Hard:   hard.push_back(std::move(q)); break;
            case core::Difficulty::Expert: expert.push_back(std::move(q)); break;
        }
    }

    std::ranges::shuffle(easy, rng);
    std::ranges::shuffle(medium, rng);
    std::ranges::shuffle(hard, rng);
    std::ranges::shuffle(expert, rng);

    // Weighted distribution: Easy ~30%, Medium ~35%, Hard ~25%, Expert ~10%
    int nEasy   = static_cast<int>(std::round(count * 0.30));
    int nMedium = static_cast<int>(std::round(count * 0.35));
    int nHard   = static_cast<int>(std::round(count * 0.25));
    int nExpert = count - nEasy - nMedium - nHard;

    nEasy   = std::min(nEasy,   static_cast<int>(easy.size()));
    nMedium = std::min(nMedium, static_cast<int>(medium.size()));
    nHard   = std::min(nHard,   static_cast<int>(hard.size()));
    nExpert = std::min(nExpert, static_cast<int>(expert.size()));

    int total = nEasy + nMedium + nHard + nExpert;
    auto fillFrom = [&](std::vector<application::QuestionDto>& pool, int& taken) {
        while (total < count && taken < static_cast<int>(pool.size())) {
            ++taken;
            ++total;
        }
    };
    fillFrom(easy, nEasy);
    fillFrom(medium, nMedium);
    fillFrom(hard, nHard);
    fillFrom(expert, nExpert);

    questions.clear();
    auto take = [&](std::vector<application::QuestionDto>& pool, int n) {
        for (int i = 0; i < n && i < static_cast<int>(pool.size()); ++i)
            questions.push_back(std::move(pool[i]));
    };
    take(easy, nEasy);
    take(medium, nMedium);
    take(hard, nHard);
    take(expert, nExpert);

    std::ranges::shuffle(questions, rng);
}

} // anonymous namespace

QuizSessionViewModel::QuizSessionViewModel(
    std::shared_ptr<application::StartQuizUseCase> startQuiz,
    std::shared_ptr<application::SubmitAnswerUseCase> submitAnswer,
    std::shared_ptr<application::SaveScoreUseCase> saveScore,
    QObject* parent)
    : QObject{parent}
    , m_startQuiz{std::move(startQuiz)}
    , m_submitAnswer{std::move(submitAnswer)}
    , m_saveScore{std::move(saveScore)}
{
    m_timer.setInterval(1000);
    connect(&m_timer, &QTimer::timeout, this, &QuizSessionViewModel::onTimerTick);
}

QString QuizSessionViewModel::quizName() const {
    return m_quiz.name;
}

int QuizSessionViewModel::totalQuestions() const {
    return static_cast<int>(m_quiz.questionCount());
}

QString QuizSessionViewModel::questionTitle() const {
    if (m_currentIndex < 0 || m_currentIndex >= totalQuestions()) return {};
    return m_quiz.questions[m_currentIndex].title;
}

QString QuizSessionViewModel::codeSnippet() const {
    if (m_currentIndex < 0 || m_currentIndex >= totalQuestions()) return {};
    return m_quiz.questions[m_currentIndex].codeSnippet;
}

QStringList QuizSessionViewModel::answerOptions() const {
    if (m_currentIndex < 0 || m_currentIndex >= totalQuestions()) return {};
    return m_quiz.questions[m_currentIndex].answerOptions;
}

void QuizSessionViewModel::startQuiz(int quizId) {
    auto result = m_startQuiz->execute(quizId);
    if (!result) return;

    m_quiz = std::move(result.value());
    m_quizId = quizId;
    selectAndShuffleQuestions(m_quiz.questions, -1);
    m_timeLimitSeconds = 0;
    initQuizState();
}

int QuizSessionViewModel::availableQuestionCount(int quizId) {
    auto result = m_startQuiz->execute(quizId);
    if (result) return static_cast<int>(result->questionCount());
    return 0;
}

void QuizSessionViewModel::startQuizWithCount(int quizId, int count) {
    auto result = m_startQuiz->execute(quizId);
    if (!result) return;

    m_quiz = std::move(result.value());
    m_quizId = quizId;
    selectAndShuffleQuestions(m_quiz.questions, count);
    m_timeLimitSeconds = 0;
    initQuizState();
}

void QuizSessionViewModel::startQuizWithCountAndTime(int quizId, int count, int timeLimitSecs) {
    auto result = m_startQuiz->execute(quizId);
    if (!result) return;

    m_quiz = std::move(result.value());
    m_quizId = quizId;
    selectAndShuffleQuestions(m_quiz.questions, count);
    m_timeLimitSeconds = timeLimitSecs;
    initQuizState();
}

void QuizSessionViewModel::submitAnswer(const QString& answer) {
    if (m_finished || m_answerLocked || m_currentIndex >= totalQuestions()) return;

    auto questionId = m_quiz.questions[m_currentIndex].id;
    auto result = m_submitAnswer->execute(questionId, answer);
    if (!result) return;

    const auto& res = result.value();
    m_answerLocked = true;
    m_lastAnswerCorrect = res.correct;

    QVariantMap entry;
    entry[QStringLiteral("question")] = m_quiz.questions[m_currentIndex].title;
    entry[QStringLiteral("userAnswer")] = answer;
    entry[QStringLiteral("correctAnswer")] = res.correctAnswer;
    entry[QStringLiteral("correct")] = res.correct;
    m_answerResults.append(entry);

    if (res.correct) {
        ++m_score;
        m_lastFeedback = QStringLiteral("Correct!");
    } else {
        m_lastFeedback = QString("Incorrect — the answer is: %1\n%2")
            .arg(res.correctAnswer, res.explanation);
    }
    emit scoreChanged();
    emit feedbackChanged();
}

void QuizSessionViewModel::nextQuestion() {
    if (m_currentIndex + 1 >= totalQuestions()) {
        finishQuiz();
    } else {
        ++m_currentIndex;
        m_lastFeedback.clear();
        m_answerLocked = false;
        m_lastAnswerCorrect = false;
        emit currentIndexChanged();
        emit feedbackChanged();
    }
}

void QuizSessionViewModel::initQuizState() {
    m_currentIndex = 0;
    m_finished = false;
    m_score = 0;
    m_lastFeedback.clear();
    m_lastAnswerCorrect = false;
    m_answerLocked = false;
    m_answerResults.clear();
    m_timeExpired = false;
    m_elapsedSeconds = 0;
    m_elapsedTimer.start();

    m_timer.stop();
    if (m_timeLimitSeconds > 0) {
        m_remainingSeconds = m_timeLimitSeconds;
        m_timer.start();
    } else {
        m_remainingSeconds = 0;
    }

    emit quizChanged();
    emit currentIndexChanged();
    emit isFinishedChanged();
    emit scoreChanged();
    emit feedbackChanged();
    emit remainingSecondsChanged();
    emit timeExpiredChanged();
}

void QuizSessionViewModel::onTimerTick() {
    if (m_remainingSeconds <= 0 || m_finished) {
        m_timer.stop();
        return;
    }

    --m_remainingSeconds;
    emit remainingSecondsChanged();

    if (m_remainingSeconds <= 0) {
        m_timer.stop();
        m_timeExpired = true;
        emit timeExpiredChanged();
        finishQuiz();
    }
}

void QuizSessionViewModel::finishQuiz() {
    m_finished = true;
    m_timer.stop();
    m_elapsedSeconds = static_cast<int>(m_elapsedTimer.elapsed() / 1000);

    if (!m_playerName.isEmpty()) {
        core::HallOfFameEntry entry{
            .playerName = m_playerName,
            .quizId = m_quizId,
            .quizName = m_quiz.name,
            .score = m_score,
            .totalQuestions = totalQuestions(),
            .elapsedSeconds = m_elapsedSeconds,
            .timestamp = QDateTime::currentDateTime()
        };
        m_saveScore->execute(entry);
    }

    emit isFinishedChanged();
}

} // namespace cowme::presentation
