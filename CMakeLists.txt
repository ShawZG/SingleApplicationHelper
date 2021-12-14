cmake_minimum_required(VERSION 3.7.0)

project(SingleApplicationHelper LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)

find_package(Qt5 COMPONENTS Core Network REQUIRED)

#add_library(${PROJECT_NAME} OBJECT )
add_library(${PROJECT_NAME} STATIC "")
target_sources(${PROJECT_NAME} PRIVATE SingleApplicationHelper.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Core Qt5::Network)
