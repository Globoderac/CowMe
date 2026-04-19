add_library(WaskMeApplication STATIC)
add_library(WaskMe::Application ALIAS WaskMeApplication)

target_sources(WaskMeApplication PRIVATE
    dto/QuestionDto.hpp
    dto/QuizDto.hpp
    strategies/IAnswerStrategy.hpp
    strategies/ComboSelectionStrategy.hpp
    strategies/TextInputStrategy.hpp
    strategies/CodeOutputMatchStrategy.hpp
    strategies/AnswerStrategyFactory.hpp
    usecases/StartQuizUseCase.hpp
    usecases/StartQuizUseCase.cpp
    usecases/SubmitAnswerUseCase.hpp
    usecases/SubmitAnswerUseCase.cpp
    usecases/GetAllQuizzesUseCase.hpp
    usecases/GetAllQuizzesUseCase.cpp
    usecases/CreateQuizUseCase.hpp
    usecases/CreateQuizUseCase.cpp
    usecases/DeleteQuizUseCase.hpp
    usecases/UpdateQuizUseCase.hpp
    usecases/SaveScoreUseCase.hpp
    usecases/GetLeaderboardUseCase.hpp
)

target_include_directories(WaskMeApplication PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(WaskMeApplication PUBLIC WaskMe::Core)