cmake_minimum_required(VERSION 3.4)

project(saper)

find_package(SFML 2.5 COMPONENTS graphics audio REQUIRED)

add_executable(saper Game.cpp mainApp.cpp)

target_link_libraries(saper sfml-graphics sfml-audio)

