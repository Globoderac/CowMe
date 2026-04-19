#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QQuickStyle>
#include <QFont>
#include <QIcon>
#include "InfrastructureInit.hpp"
#include "usecases/GetAllQuizzesUseCase.hpp"
#include "usecases/StartQuizUseCase.hpp"
#include "usecases/SubmitAnswerUseCase.hpp"
#include "usecases/SaveScoreUseCase.hpp"
#include "usecases/GetLeaderboardUseCase.hpp"
#include "viewmodels/QuizListViewModel.hpp"
#include "viewmodels/QuizSessionViewModel.hpp"
#include "viewmodels/CreateQuizViewModel.hpp"
#include "viewmodels/HallOfFameViewModel.hpp"
#include "usecases/CreateQuizUseCase.hpp"
#include "usecases/DeleteQuizUseCase.hpp"
#include "usecases/UpdateQuizUseCase.hpp"

int main(int argc, char *argv[])
{
	QGuiApplication app(argc, argv);
	app.setApplicationName("CowMe - Wartsila");
	app.setApplicationVersion("0.1.0");
	app.setWindowIcon(QIcon(QStringLiteral(":/assets/icon.png")));

	QQuickStyle::setStyle("Basic");

	QFont defaultFont(QStringLiteral("Segoe UI"), 10);
	app.setFont(defaultFont);

	// Infrastructure (Singleton DB + repositories)
	auto services = cowme::infrastructure::initializeInfrastructure();

	// Use cases (DI via constructor injection)
	auto getAllQuizzes = std::make_shared<cowme::application::GetAllQuizzesUseCase>(
		services.quizRepo);
	auto startQuiz = std::make_shared<cowme::application::StartQuizUseCase>(
		services.quizRepo);
	auto submitAnswer = std::make_shared<cowme::application::SubmitAnswerUseCase>(
		services.questionRepo);
	auto createQuiz = std::make_shared<cowme::application::CreateQuizUseCase>(
		services.quizRepo);
	auto deleteQuiz = std::make_shared<cowme::application::DeleteQuizUseCase>(
		services.quizRepo);
	auto updateQuiz = std::make_shared<cowme::application::UpdateQuizUseCase>(
		services.quizRepo);
	auto saveScore = std::make_shared<cowme::application::SaveScoreUseCase>(
		services.hallOfFameRepo);
	auto getLeaderboard = std::make_shared<cowme::application::GetLeaderboardUseCase>(
		services.hallOfFameRepo);

	// ViewModels
	cowme::presentation::QuizListViewModel quizListVM(getAllQuizzes, deleteQuiz);
	cowme::presentation::QuizSessionViewModel quizSessionVM(startQuiz, submitAnswer, saveScore);
	cowme::presentation::CreateQuizViewModel createQuizVM(createQuiz, updateQuiz, services.quizRepo);
	cowme::presentation::HallOfFameViewModel hallOfFameVM(getLeaderboard, saveScore);

	// QML engine
	QQmlApplicationEngine engine;
	engine.rootContext()->setContextProperty("quizListVM", &quizListVM);
	engine.rootContext()->setContextProperty("quizSessionVM", &quizSessionVM);
	engine.rootContext()->setContextProperty("createQuizVM", &createQuizVM);
	engine.rootContext()->setContextProperty("hallOfFameVM", &hallOfFameVM);

	engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));
	if (engine.rootObjects().isEmpty())
		return -1;

	return app.exec();
}