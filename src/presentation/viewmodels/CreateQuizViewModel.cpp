#include "CreateQuizViewModel.hpp"

namespace cowme::presentation {

CreateQuizViewModel::CreateQuizViewModel(
    std::shared_ptr<application::CreateQuizUseCase> createQuiz,
    std::shared_ptr<application::UpdateQuizUseCase> updateQuiz,
    std::shared_ptr<core::IQuizRepository> quizRepo,
    QObject* parent)
    : QObject(parent)
    , m_createQuiz{std::move(createQuiz)}
    , m_updateQuiz{std::move(updateQuiz)}
    , m_quizRepo{std::move(quizRepo)}
{}

void CreateQuizViewModel::setQuizName(const QString& name)
{
    if (m_quizName != name) {
        m_quizName = name;
        emit quizNameChanged();
        emit canSaveChanged();
    }
}

void CreateQuizViewModel::setQuizDescription(const QString& desc)
{
    if (m_quizDescription != desc) {
        m_quizDescription = desc;
        emit quizDescriptionChanged();
    }
}

void CreateQuizViewModel::addQuestion(
    const QString& title,
    const QString& codeSnippet,
    const QStringList& answerOptions,
    const QString& correctAnswer,
    const QString& explanation,
    int difficulty,
    int strategyType,
    const QString& category)
{
    application::QuestionDto dto{
        .id = 0,
        .title = title,
        .codeSnippet = codeSnippet,
        .answerOptions = answerOptions,
        .correctAnswer = correctAnswer,
        .explanation = explanation,
        .difficulty = static_cast<core::Difficulty>(difficulty),
        .strategyType = static_cast<core::AnswerStrategyType>(strategyType),
        .tags = {},
        .category = category
    };
    m_questions.push_back(std::move(dto));
    emit questionsChanged();
    emit canSaveChanged();
}

void CreateQuizViewModel::removeQuestion(int index)
{
    if (index >= 0 && index < static_cast<int>(m_questions.size())) {
        m_questions.erase(m_questions.begin() + index);
        emit questionsChanged();
        emit canSaveChanged();
    }
}

QString CreateQuizViewModel::questionTitleAt(int index) const
{
    if (index >= 0 && index < static_cast<int>(m_questions.size()))
        return m_questions[index].title;
    return {};
}

QString CreateQuizViewModel::questionCategoryAt(int index) const
{
    if (index >= 0 && index < static_cast<int>(m_questions.size()))
        return m_questions[index].category;
    return {};
}

int CreateQuizViewModel::questionDifficultyAt(int index) const
{
    if (index >= 0 && index < static_cast<int>(m_questions.size()))
        return static_cast<int>(m_questions[index].difficulty);
    return 0;
}

QString CreateQuizViewModel::questionCodeSnippetAt(int index) const
{
    if (index >= 0 && index < static_cast<int>(m_questions.size()))
        return m_questions[index].codeSnippet;
    return {};
}

QStringList CreateQuizViewModel::questionAnswerOptionsAt(int index) const
{
    if (index >= 0 && index < static_cast<int>(m_questions.size()))
        return m_questions[index].answerOptions;
    return {};
}

QString CreateQuizViewModel::questionCorrectAnswerAt(int index) const
{
    if (index >= 0 && index < static_cast<int>(m_questions.size()))
        return m_questions[index].correctAnswer;
    return {};
}

QString CreateQuizViewModel::questionExplanationAt(int index) const
{
    if (index >= 0 && index < static_cast<int>(m_questions.size()))
        return m_questions[index].explanation;
    return {};
}

int CreateQuizViewModel::questionStrategyTypeAt(int index) const
{
    if (index >= 0 && index < static_cast<int>(m_questions.size()))
        return static_cast<int>(m_questions[index].strategyType);
    return 0;
}

void CreateQuizViewModel::updateQuestion(
    int index,
    const QString& title,
    const QString& codeSnippet,
    const QStringList& answerOptions,
    const QString& correctAnswer,
    const QString& explanation,
    int difficulty,
    int strategyType,
    const QString& category)
{
    if (index < 0 || index >= static_cast<int>(m_questions.size())) return;

    auto& q = m_questions[index];
    q.title = title;
    q.codeSnippet = codeSnippet;
    q.answerOptions = answerOptions;
    q.correctAnswer = correctAnswer;
    q.explanation = explanation;
    q.difficulty = static_cast<core::Difficulty>(difficulty);
    q.strategyType = static_cast<core::AnswerStrategyType>(strategyType);
    q.category = category;
    emit questionsChanged();
}

bool CreateQuizViewModel::save()
{
    if (!canSave()) return false;

    m_saving = true;
    emit savingChanged();

    application::QuizDto quiz{
        .id = m_editMode ? m_editQuizId : 0,
        .name = m_quizName,
        .description = m_quizDescription,
        .questions = m_questions
    };

    bool success = false;
    if (m_editMode) {
        auto result = m_updateQuiz->execute(quiz);
        success = result.has_value();
    } else {
        auto result = m_createQuiz->execute(quiz);
        success = result.has_value();
    }

    m_saving = false;
    emit savingChanged();

    if (success) {
        m_errorMessage.clear();
        emit errorMessageChanged();
        reset();
        emit quizCreated();
        return true;
    } else {
        m_errorMessage = m_editMode
            ? QStringLiteral("Failed to update quiz.")
            : QStringLiteral("Failed to create quiz.");
        emit errorMessageChanged();
        return false;
    }
}

void CreateQuizViewModel::loadForEdit(int quizId, const QString& name, const QString& description)
{
    reset();
    m_editMode = true;
    m_editQuizId = static_cast<int64_t>(quizId);
    m_quizName = name;
    m_quizDescription = description;

    // Load existing questions from DB
    auto quizResult = m_quizRepo->getById(core::QuizId{m_editQuizId});
    if (quizResult) {
        for (const auto& q : quizResult->questions) {
            application::QuestionDto dto{
                .id = q.id.value,
                .title = q.title,
                .codeSnippet = q.codeSnippet,
                .answerOptions = q.answerOptions,
                .correctAnswer = q.correctAnswer,
                .explanation = q.explanation,
                .difficulty = q.difficulty,
                .strategyType = q.strategyType,
                .tags = q.tags,
                .category = q.category
            };
            m_questions.push_back(std::move(dto));
        }
    }

    emit editModeChanged();
    emit quizNameChanged();
    emit quizDescriptionChanged();
    emit questionsChanged();
    emit canSaveChanged();
}

void CreateQuizViewModel::reset()
{
    m_quizName.clear();
    m_quizDescription.clear();
    m_questions.clear();
    m_errorMessage.clear();
    m_editMode = false;
    m_editQuizId = 0;
    emit editModeChanged();
    emit quizNameChanged();
    emit quizDescriptionChanged();
    emit questionsChanged();
    emit canSaveChanged();
    emit errorMessageChanged();
}

} // namespace cowme::presentation
