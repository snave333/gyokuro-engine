cmake_minimum_required(VERSION 3.10)

set(SAMPLE_NAME "models")

project(${SAMPLE_NAME} LANGUAGES CXX)

add_executable(${SAMPLE_NAME} main.cpp)

target_link_libraries(${SAMPLE_NAME} PUBLIC gyokuro)

set(COPY_ENGINE_RESOURCES_TARGET_NAME "copy_${SAMPLE_NAME}_gyokuro_resources")
add_custom_target(${COPY_ENGINE_RESOURCES_TARGET_NAME} ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${gyokuro_SOURCE_DIR}/resources
    ${CMAKE_CURRENT_BINARY_DIR}/resources
    COMMENT "Copying engine resources to binary directory"
)
add_dependencies(${SAMPLE_NAME} ${COPY_ENGINE_RESOURCES_TARGET_NAME})

set(COPY_RESOURCES_TARGET_NAME "copy_${SAMPLE_NAME}_resources")
add_custom_target(${COPY_RESOURCES_TARGET_NAME} ALL
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_CURRENT_SOURCE_DIR}/resources
    ${CMAKE_CURRENT_BINARY_DIR}/resources
    COMMENT "Copying sample resources to binary directory"
)
add_dependencies(${SAMPLE_NAME} ${COPY_RESOURCES_TARGET_NAME})
