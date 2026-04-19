add_library(CowMeInfrastructure STATIC)
add_library(CowMe::Infrastructure ALIAS CowMeInfrastructure)

target_sources(CowMeInfrastructure PRIVATE
    database/DatabaseManager.hpp
    repositories/SqliteQuestionRepository.hpp
    repositories/SqliteQuizRepository.hpp
    repositories/SqliteHallOfFameRepository.hpp
    InfrastructureInit.hpp
    InfrastructureInit.cpp
)

target_include_directories(CowMeInfrastructure PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(CowMeInfrastructure PUBLIC CowMe::Application Qt6::Sql)