cmake_minimum_required(VERSION 3.29)
project(project1)

set(CMAKE_CXX_STANDARD 20)

add_executable(project1 src/main.cpp
        src/Menu.cpp
        src/DataReader.cpp
        src/Dijsktra.cpp)
