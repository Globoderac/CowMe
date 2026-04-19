add_library(WaskMeInfrastructure STATIC)
add_library(WaskMe::Infrastructure ALIAS WaskMeInfrastructure)

target_sources(WaskMeInfrastructure PRIVATE
    database/DatabaseManager.hpp
    repositories/SqliteQuestionRepository.hpp
    repositories/SqliteQuizRepository.hpp
    repositories/SqliteHallOfFameRepository.hpp
    InfrastructureInit.hpp
    InfrastructureInit.cpp
)

target_include_directories(WaskMeInfrastructure PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(WaskMeInfrastructure PUBLIC WaskMe::Application Qt6::Sql)