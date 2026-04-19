add_library(CowMePresentation STATIC)
add_library(CowMe::Presentation ALIAS CowMePresentation)

target_sources(CowMePresentation PRIVATE
    viewmodels/QuizListViewModel.hpp
    viewmodels/QuizListViewModel.cpp
    viewmodels/QuizSessionViewModel.hpp
    viewmodels/QuizSessionViewModel.cpp
    viewmodels/CreateQuizViewModel.hpp
    viewmodels/CreateQuizViewModel.cpp
    viewmodels/HallOfFameViewModel.hpp
)

qt_add_resources(CowMePresentation "qml"
    PREFIX "/"
    FILES
        qml/Main.qml
        qml/WelcomePage.qml
        qml/QuizListPage.qml
        qml/QuizSessionPage.qml
        qml/CreateQuizPage.qml
        qml/HallOfFamePage.qml
        assets/wlogo.png
        assets/cpp_mascot.jpg
        assets/vessel.jpg
        assets/icon.png
)

target_include_directories(CowMePresentation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(CowMePresentation PUBLIC CowMe::Application Qt6::Quick Qt6::QuickControls2)