#include "QuizListViewModel.hpp"

namespace cowme::presentation {

QuizListViewModel::QuizListViewModel(
    std::shared_ptr<application::GetAllQuizzesUseCase> getAllQuizzes,
    std::shared_ptr<application::DeleteQuizUseCase> deleteQuiz,
    QObject* parent)
    : QObject{parent}
    , m_getAllQuizzes{std::move(getAllQuizzes)}
    , m_deleteQuiz{std::move(deleteQuiz)}
{}

void QuizListViewModel::loadQuizzes() {
    m_loading = true;
    emit loadingChanged();

    auto result = m_getAllQuizzes->execute();
    if (result) {
        m_quizzes = std::move(result.value());
        m_quizNames.clear();
        for (const auto& quiz : m_quizzes) {
            m_quizNames << quiz.name;
        }
        m_errorMessage.clear();
    } else {
        m_errorMessage = "Failed to load quizzes";
        m_quizNames.clear();
        m_quizzes.clear();
    }

    m_loading = false;
    emit quizNamesChanged();
    emit errorMessageChanged();
    emit loadingChanged();
}

int QuizListViewModel::quizIdAt(int index) const {
    if (index < 0 || index >= static_cast<int>(m_quizzes.size()))
        return -1;
    return static_cast<int>(m_quizzes[index].id);
}

QString QuizListViewModel::quizDescriptionAt(int index) const {
    if (index < 0 || index >= static_cast<int>(m_quizzes.size()))
        return {};
    return m_quizzes[index].description;
}

int QuizListViewModel::quizQuestionCountAt(int index) const {
    if (index < 0 || index >= static_cast<int>(m_quizzes.size()))
        return 0;
    return static_cast<int>(m_quizzes[index].questionCount());
}

int QuizListViewModel::quizCount() const {
    return static_cast<int>(m_quizzes.size());
}

bool QuizListViewModel::deleteQuiz(int quizId) {
    auto result = m_deleteQuiz->execute(static_cast<int64_t>(quizId));
    if (result) {
        loadQuizzes();
        return true;
    }
    m_errorMessage = "Failed to delete quiz";
    emit errorMessageChanged();
    return false;
}

} // namespace cowme::presentation
