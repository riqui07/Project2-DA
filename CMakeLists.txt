cmake_minimum_required(VERSION 3.30.5)
project(project2)

set(CMAKE_CXX_STANDARD 17)

add_executable(project2
        Code/main.cpp
        Code/data_structures/
        Code/data_structures/Parser.h
        Code/data_structures/Parser.cpp
        Code/data_structures/Web.h
        Code/data_structures/Web.cpp
        Code/data_structures/SpiderMan.h
        Code/data_structures/SpiderMan.cpp
        Code/data_structures/Graph.h
)
