# Math operations library
add_library(math_operations
    math_operations.h
    math_operations.cpp
)

target_include_directories(math_operations PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})