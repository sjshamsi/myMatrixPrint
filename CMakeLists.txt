cmake_minimum_required(VERSION 3.16)

project(myMatrixPrint LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)


add_executable(Print2DMatrix src/Print2DMatrix.cpp)
target_include_directories(Print2DMatrix PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include)