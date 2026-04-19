#pragma once
#include <QObject>
#include <QQmlEngine>
#include <QString>
#include <QStringList>
#include <QVariantList>
#include <QVariantMap>
#include "usecases/CreateQuizUseCase.hpp"
#include "usecases/UpdateQuizUseCase.hpp"
#include "dto/QuizDto.hpp"
#include "dto/QuestionDto.hpp"
#include <memory>
#include <vector>

namespace cowme::presentation {

class CreateQuizViewModel : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QString quizName READ quizName WRITE setQuizName NOTIFY quizNameChanged)
    Q_PROPERTY(QString quizDescription READ quizDescription WRITE setQuizDescription NOTIFY quizDescriptionChanged)
    Q_PROPERTY(int questionCount READ questionCount NOTIFY questionsChanged)
    Q_PROPERTY(bool canSave READ canSave NOTIFY canSaveChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)
    Q_PROPERTY(bool saving READ saving NOTIFY savingChanged)
    Q_PROPERTY(bool editMode READ editMode NOTIFY editModeChanged)

public:
    explicit CreateQuizViewModel(
        std::shared_ptr<application::CreateQuizUseCase> createQuiz,
        std::shared_ptr<application::UpdateQuizUseCase> updateQuiz,
        std::shared_ptr<core::IQuizRepository> quizRepo,
        QObject* parent = nullptr);

    [[nodiscard]] QString quizName() const { return m_quizName; }
    [[nodiscard]] QString quizDescription() const { return m_quizDescription; }
    [[nodiscard]] int questionCount() const { return static_cast<int>(m_questions.size()); }
    [[nodiscard]] bool canSave() const { return !m_quizName.isEmpty() && !m_questions.empty(); }
    [[nodiscard]] QString errorMessage() const { return m_errorMessage; }
    [[nodiscard]] bool saving() const { return m_saving; }
    [[nodiscard]] bool editMode() const { return m_editMode; }

    void setQuizName(const QString& name);
    void setQuizDescription(const QString& desc);

    Q_INVOKABLE void loadForEdit(int quizId, const QString& name, const QString& description);

    Q_INVOKABLE void addQuestion(
        const QString& title,
        const QString& codeSnippet,
        const QStringList& answerOptions,
        const QString& correctAnswer,
        const QString& explanation,
        int difficulty,
        int strategyType,
        const QString& category);

    Q_INVOKABLE void removeQuestion(int index);

    Q_INVOKABLE QString questionTitleAt(int index) const;
    Q_INVOKABLE QString questionCategoryAt(int index) const;
    Q_INVOKABLE int questionDifficultyAt(int index) const;
    Q_INVOKABLE QString questionCodeSnippetAt(int index) const;
    Q_INVOKABLE QStringList questionAnswerOptionsAt(int index) const;
    Q_INVOKABLE QString questionCorrectAnswerAt(int index) const;
    Q_INVOKABLE QString questionExplanationAt(int index) const;
    Q_INVOKABLE int questionStrategyTypeAt(int index) const;

    Q_INVOKABLE void updateQuestion(
        int index,
        const QString& title,
        const QString& codeSnippet,
        const QStringList& answerOptions,
        const QString& correctAnswer,
        const QString& explanation,
        int difficulty,
        int strategyType,
        const QString& category);

    Q_INVOKABLE bool save();
    Q_INVOKABLE void reset();

signals:
    void quizNameChanged();
    void quizDescriptionChanged();
    void questionsChanged();
    void canSaveChanged();
    void errorMessageChanged();
    void savingChanged();
    void editModeChanged();
    void quizCreated();

private:
    std::shared_ptr<application::CreateQuizUseCase> m_createQuiz;
    std::shared_ptr<application::UpdateQuizUseCase> m_updateQuiz;
    std::shared_ptr<core::IQuizRepository> m_quizRepo;
    QString m_quizName;
    QString m_quizDescription;
    std::vector<application::QuestionDto> m_questions;
    QString m_errorMessage;
    bool m_saving{false};
    bool m_editMode{false};
    int64_t m_editQuizId{0};
};

} // namespace cowme::presentation
