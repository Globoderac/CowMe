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
#include "usecases/CreateQuizUseCase.hpp"
#include "usecases/DeleteQuizUseCase.hpp"
#include "usecases/UpdateQuizUseCase.hpp"
#include "usecases/SaveScoreUseCase.hpp"
#include "usecases/GetLeaderboardUseCase.hpp"

#include "viewmodels/QuizListViewModel.hpp"
#include "viewmodels/QuizSessionViewModel.hpp"
#include "viewmodels/CreateQuizViewModel.hpp"
#include "viewmodels/HallOfFameViewModel.hpp"

namespace {

struct UseCases {
	std::shared_ptr<cowme::application::GetAllQuizzesUseCase>  getAllQuizzes;
	std::shared_ptr<cowme::application::StartQuizUseCase>      startQuiz;
	std::shared_ptr<cowme::application::SubmitAnswerUseCase>   submitAnswer;
	std::shared_ptr<cowme::application::CreateQuizUseCase>     createQuiz;
	std::shared_ptr<cowme::application::DeleteQuizUseCase>     deleteQuiz;
	std::shared_ptr<cowme::application::UpdateQuizUseCase>     updateQuiz;
	std::shared_ptr<cowme::application::SaveScoreUseCase>      saveScore;
	std::shared_ptr<cowme::application::GetLeaderboardUseCase> getLeaderboard;
};

[[nodiscard]] UseCases createUseCases(const cowme::infrastructure::InfrastructureServices& svc) {
	return {
		.getAllQuizzes  = std::make_shared<cowme::application::GetAllQuizzesUseCase>(svc.quizRepo),
		.startQuiz     = std::make_shared<cowme::application::StartQuizUseCase>(svc.quizRepo),
		.submitAnswer  = std::make_shared<cowme::application::SubmitAnswerUseCase>(svc.questionRepo),
		.createQuiz    = std::make_shared<cowme::application::CreateQuizUseCase>(svc.quizRepo),
		.deleteQuiz    = std::make_shared<cowme::application::DeleteQuizUseCase>(svc.quizRepo),
		.updateQuiz    = std::make_shared<cowme::application::UpdateQuizUseCase>(svc.quizRepo),
		.saveScore     = std::make_shared<cowme::application::SaveScoreUseCase>(svc.hallOfFameRepo),
		.getLeaderboard = std::make_shared<cowme::application::GetLeaderboardUseCase>(svc.hallOfFameRepo),
	};
}

void registerViewModels(QQmlContext* ctx,
						cowme::presentation::QuizListViewModel& quizListVM,
						cowme::presentation::QuizSessionViewModel& quizSessionVM,
						cowme::presentation::CreateQuizViewModel& createQuizVM,
						cowme::presentation::HallOfFameViewModel& hallOfFameVM)
{
	ctx->setContextProperty("quizListVM",    &quizListVM);
	ctx->setContextProperty("quizSessionVM", &quizSessionVM);
	ctx->setContextProperty("createQuizVM",  &createQuizVM);
	ctx->setContextProperty("hallOfFameVM",  &hallOfFameVM);
}

} // anonymous namespace

int main(int argc, char* argv[])
{
	QGuiApplication app(argc, argv);
	app.setApplicationName("WaskMe - Wartsila");
	app.setApplicationVersion("0.1.0");
	app.setWindowIcon(QIcon(QStringLiteral(":/assets/icon.png")));
	QQuickStyle::setStyle("Basic");
	app.setFont(QFont(QStringLiteral("Segoe UI"), 10));

	// Infrastructure
	auto services = cowme::infrastructure::initializeInfrastructure();
	auto uc = createUseCases(services);

	// ViewModels
	cowme::presentation::QuizListViewModel    quizListVM(uc.getAllQuizzes, uc.deleteQuiz);
	cowme::presentation::QuizSessionViewModel quizSessionVM(uc.startQuiz, uc.submitAnswer, uc.saveScore);
	cowme::presentation::CreateQuizViewModel  createQuizVM(uc.createQuiz, uc.updateQuiz, services.quizRepo);
	cowme::presentation::HallOfFameViewModel  hallOfFameVM(uc.getLeaderboard, uc.saveScore);

	// QML engine
	QQmlApplicationEngine engine;
	registerViewModels(engine.rootContext(), quizListVM, quizSessionVM, createQuizVM, hallOfFameVM);

	engine.load(QUrl(QStringLiteral("qrc:/qml/Main.qml")));
	if (engine.rootObjects().isEmpty())
		return -1;

	return app.exec();
}