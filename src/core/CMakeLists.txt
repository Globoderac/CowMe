add_library(WaskMeCore STATIC)
add_library(WaskMe::Core ALIAS WaskMeCore)

target_sources(WaskMeCore PRIVATE
	entities/Question.hpp
	entities/Quiz.hpp
	entities/HallOfFameEntry.hpp
	enums/Difficulty.hpp
	enums/AnswerStrategy.hpp
	interfaces/IQuestionRepository.hpp
	interfaces/IQuizRepository.hpp
	interfaces/IHallOfFameRepository.hpp
	value_objects/QuestionId.hpp
)

target_include_directories(WaskMeCore PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(WaskMeCore PUBLIC Qt6::Core)