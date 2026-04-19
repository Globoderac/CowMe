add_library(CowMeApplication STATIC)
add_library(CowMe::Application ALIAS CowMeApplication)

target_sources(CowMeApplication PRIVATE
    dto/QuestionDto.hpp
    dto/QuizDto.hpp
    dto/QuizResultDto.hpp
    strategies/IAnswerStrategy.hpp
    strategies/ComboSelectionStrategy.hpp
    strategies/TextInputStrategy.hpp
    strategies/CodeOutputMatchStrategy.hpp
    strategies/AnswerStrategyFactory.hpp
    services/IQuizService.hpp
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

target_include_directories(CowMeApplication PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(CowMeApplication PUBLIC CowMe::Core)