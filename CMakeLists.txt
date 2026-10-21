cmake_minimum_required(VERSION 3.20)
project(cpp_log_analyzer VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

add_library(log_analyzer_lib src/log_analyzer.cpp)
target_include_directories(log_analyzer_lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)

add_executable(log_analyzer src/main.cpp)
target_link_libraries(log_analyzer PRIVATE log_analyzer_lib)

include(CTest)
if(BUILD_TESTING)
    add_executable(log_analyzer_tests tests/test_log_analyzer.cpp)
    target_link_libraries(log_analyzer_tests PRIVATE log_analyzer_lib)
    add_test(NAME log_analyzer_tests COMMAND log_analyzer_tests)
endif()
