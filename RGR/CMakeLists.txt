cmake_minimum_required(VERSION 3.10)

project(RGR)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(SRC_DIR "src")
set(INCLUDE_DIR "include")
set(DATA_DIR "data")
set(PYTHON_DIR "python")

include_directories(${INCLUDE_DIR})

file(GLOB_RECURSE BIT_SEQUENCE_SRC ${SRC_DIR}/BitSequence/*.cpp)
file(GLOB_RECURSE SIGNAL_PROCESSING_SRC ${SRC_DIR}/SignalProcessing/*.cpp)
file(GLOB_RECURSE SIGNAL_ANALYSIS_SRC ${SRC_DIR}/SignalAnalysis/*.cpp)
file(GLOB_RECURSE UTILITIES_SRC ${SRC_DIR}/Utilities/*.cpp)
file(GLOB_RECURSE VISUALIZATION_SRC ${SRC_DIR}/Visualization/*.cpp)

set(MAIN_SRC ${SRC_DIR}/main.cpp)

add_executable(${PROJECT_NAME} \
    ${MAIN_SRC} \
    ${BIT_SEQUENCE_SRC} \
    ${SIGNAL_PROCESSING_SRC} \
    ${SIGNAL_ANALYSIS_SRC} \
    ${UTILITIES_SRC} \
    ${VISUALIZATION_SRC})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/${PYTHON_DIR} ${CMAKE_BINARY_DIR}/${PYTHON_DIR})

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/${DATA_DIR} ${CMAKE_BINARY_DIR}/${DATA_DIR})
