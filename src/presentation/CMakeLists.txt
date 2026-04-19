add_library(WaskMePresentation STATIC)
add_library(WaskMe::Presentation ALIAS WaskMePresentation)

target_sources(WaskMePresentation PRIVATE
    viewmodels/QuizListViewModel.hpp
    viewmodels/QuizListViewModel.cpp
    viewmodels/QuizSessionViewModel.hpp
    viewmodels/QuizSessionViewModel.cpp
    viewmodels/CreateQuizViewModel.hpp
    viewmodels/CreateQuizViewModel.cpp
    viewmodels/HallOfFameViewModel.hpp
)

qt_add_resources(WaskMePresentation "qml"
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
        assets/wartsila_text.png
)

target_include_directories(WaskMePresentation PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
target_link_libraries(WaskMePresentation PUBLIC WaskMe::Application Qt6::Quick Qt6::QuickControls2)