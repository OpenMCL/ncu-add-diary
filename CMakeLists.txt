cmake_minimum_required(VERSION 3.2)
project(add_diary LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)

set(PROJECT_HEADER ${CMAKE_SOURCE_DIR}/include)
set(SOURCE_DIR ${CMAKE_SOURCE_DIR}/src)
set(JSON_DIR ${CMAKE_SOURCE_DIR}/3rdparty/json)
set(FAMILY_DATA_DIR ${CMAKE_SOURCE_DIR}/family_data)

set(JSON_Install OFF CACHE INTERNAL "")
add_subdirectory(${JSON_DIR})

file(GLOB FAMILY_JSON "${FAMILY_DATA_DIR}/*.json")
file(GLOB FAMILY_TXT "${FAMILY_DATA_DIR}/*.txt")

include_directories(
    ${JSON_DIR}/include
)

add_executable(add_diary 
    ${SOURCE_DIR}/add_diary.cpp
)

target_link_libraries(add_diary PRIVATE nlohmann_json::nlohmann_json)

file(COPY ${FAMILY_JSON} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})
file(COPY ${FAMILY_TXT} DESTINATION ${CMAKE_CURRENT_BINARY_DIR})