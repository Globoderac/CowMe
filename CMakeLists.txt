cmake_minimum_required(VERSION 3.27)
project(CowMe VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

# Enable Hot Reload for MSVC compilers if supported.
if (POLICY CMP0141)
  cmake_policy(SET CMP0141 NEW)
  set(CMAKE_MSVC_DEBUG_INFORMATION_FORMAT "$<IF:$<AND:$<C_COMPILER_ID:MSVC>,$<CXX_COMPILER_ID:MSVC>>,$<$<CONFIG:Debug,RelWithDebInfo>:EditAndContinue>,$<$<CONFIG:Debug,RelWithDebInfo>:ProgramDatabase>>")
endif()

find_package(Qt6 REQUIRED COMPONENTS Core Gui Quick QuickControls2 Sql)

add_subdirectory(src/core)
add_subdirectory(src/application)
add_subdirectory(src/infrastructure)
add_subdirectory(src/presentation)

qt_add_executable(CowMe ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

target_link_libraries(CowMe PRIVATE
    CowMe::Presentation
    CowMe::Infrastructure
    CowMe::Application
    CowMe::Core
    Qt6::Core
    Qt6::Gui
    Qt6::Quick
    Qt6::QuickControls2
    Qt6::Sql
)

# Auto-deploy Qt DLLs after build (Windows only)
if(WIN32)
    find_program(WINDEPLOYQT windeployqt6
        HINTS "${Qt6_DIR}/../../../bin"
    )
    if(WINDEPLOYQT)
        add_custom_command(TARGET CowMe POST_BUILD
            COMMAND "${WINDEPLOYQT}"
                --no-translations
                --no-system-d3d-compiler
                --qmldir "${CMAKE_SOURCE_DIR}/src/presentation/qml"
                "$<TARGET_FILE:CowMe>"
            COMMENT "Deploying Qt dependencies..."
        )
    endif()
endif()
