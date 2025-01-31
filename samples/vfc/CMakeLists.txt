cmake_minimum_required(VERSION 3.10)

set(SAMPLE_NAME "vfc")

project(${SAMPLE_NAME} LANGUAGES CXX)

add_executable(${SAMPLE_NAME} main.cpp)

target_link_libraries(${SAMPLE_NAME} PUBLIC gyokuro)

add_custom_target(copy_resources ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${gyokuro_SOURCE_DIR}/resources
    ${CMAKE_CURRENT_BINARY_DIR}/resources
    COMMENT "Copying resources to binary directory"
)

add_dependencies(${SAMPLE_NAME} copy_resources)
