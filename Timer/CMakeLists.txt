# Timer library CMakeLists.txt
cmake_minimum_required(VERSION 3.00)

# Timer library target
add_library(Timer STATIC
    src/Timer.cpp
)

# Include directories
target_include_directories(Timer PUBLIC
    include
)

# C++23 standard
set_target_properties(Timer
    PROPERTIES
        CXX_STANDARD 23
        CXX_STANDARD_REQUIRED ON
)