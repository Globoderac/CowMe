add_library(CowMeCore STATIC)
add_library(CowMe::Core ALIAS CowMeCore)

target_sources(CowMeCore PRIVATE
	entities/Question.hpp
	entities/Quiz.hpp
	entities/QuizResult.hpp
	entities/HallOfFameEntry.hpp
	enums/Difficulty.hpp
	enums/AnswerStrategy.hpp
	interfaces/IQuestionRepository.hpp
	interfaces/IQuizRepository.hpp
	interfaces/IHallOfFameRepository.hpp
	value_objects/QuestionId.hpp
	value_objects/QuizId.hpp
)

target_include_directories(CowMeCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(CowMeCore PUBLIC Qt6::Core)