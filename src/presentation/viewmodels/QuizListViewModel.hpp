#pragma once
#include <QObject>
#include <QQmlEngine>
#include <QString>
#include <QStringList>
#include "usecases/GetAllQuizzesUseCase.hpp"
#include "usecases/DeleteQuizUseCase.hpp"
#include "dto/QuizDto.hpp"
#include <memory>
#include <vector>

namespace cowme::presentation {

class QuizListViewModel : public QObject {
    Q_OBJECT
    QML_ELEMENT
    Q_PROPERTY(QStringList quizNames READ quizNames NOTIFY quizNamesChanged)
    Q_PROPERTY(int quizCount READ quizCount NOTIFY quizNamesChanged)
    Q_PROPERTY(bool loading READ loading NOTIFY loadingChanged)
    Q_PROPERTY(QString errorMessage READ errorMessage NOTIFY errorMessageChanged)

public:
    explicit QuizListViewModel(
        std::shared_ptr<application::GetAllQuizzesUseCase> getAllQuizzes,
        std::shared_ptr<application::DeleteQuizUseCase> deleteQuiz,
        QObject* parent = nullptr);

    Q_INVOKABLE bool deleteQuiz(int quizId);

    [[nodiscard]] QStringList quizNames() const { return m_quizNames; }
    [[nodiscard]] bool loading() const { return m_loading; }
    [[nodiscard]] QString errorMessage() const { return m_errorMessage; }

    Q_INVOKABLE void loadQuizzes();
    Q_INVOKABLE int quizIdAt(int index) const;
    Q_INVOKABLE QString quizDescriptionAt(int index) const;
    Q_INVOKABLE int quizQuestionCountAt(int index) const;
    Q_INVOKABLE int quizCount() const;

signals:
    void quizNamesChanged();
    void loadingChanged();
    void errorMessageChanged();

private:
    std::shared_ptr<application::GetAllQuizzesUseCase> m_getAllQuizzes;
    std::shared_ptr<application::DeleteQuizUseCase> m_deleteQuiz;
    std::vector<application::QuizDto> m_quizzes;
    QStringList m_quizNames;
    bool m_loading{false};
    QString m_errorMessage;
};

} // namespace cowme::presentation
