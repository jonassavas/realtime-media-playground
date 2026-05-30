cmake_minimum_required(VERSION 3.20)

project(
    RealtimeMediaPlayground
    VERSION 0.1
    LANGUAGES CXX
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(sender
    src/sender/main.cpp
)

target_include_directories(sender PRIVATE include)

add_executable(receiver
    src/receiver/main.cpp
)

target_include_directories(receiver PRIVATE include)