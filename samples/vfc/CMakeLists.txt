cmake_minimum_required(VERSION 3.10)

project(vfc LANGUAGES CXX)

add_executable(vfc MACOSX_BUNDLE main.cpp)

target_link_libraries(vfc PUBLIC gyokuro)
