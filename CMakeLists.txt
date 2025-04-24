
cmake_minimum_required(VERSION 3.5)

project(qtframeless VERSION 0.1 LANGUAGES CXX)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
include_directories(${CMAKE_CURRENT_SOURCE_DIR})

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB QTFRAMELESS_PROJECT_SOURCES
    "./src/*.h"
    "./src/*.cpp"
    "./src/*.ui"
    "./src/*/*.h"
    "./src/*/*.cpp"
    "./src/*/*.ui"
)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools)


add_library(qtframeless SHARED ${QTFRAMELESS_PROJECT_SOURCES})
target_link_libraries(${PROJECT_NAME} PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_compile_definitions(qtframeless PRIVATE QTFRAMELESS_LIBRARY)

if (WIN32)
    if (CMAKE_CXX_COMPILER_ID STREQUAL "MSVC")
        #add_definitions(-D_HAS_STD_BYTE=0) # c++17 std::byte
        target_compile_definitions(${PROJECT_NAME} PRIVATE _HAS_STD_BYTE=0)
    endif ()
endif()

include("./output/makeme.cmake")
#ADD_SUBDIRECTORY(${PROJECT_SOURCE_DIR}/test/framelessdemo)

include("../qtgllogger/output/includeme.cmake")


include("../qtshadowcache/output/includeme.cmake")



