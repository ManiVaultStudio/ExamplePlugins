# CMakeLists to generate a project that has all example plugins in one.

cmake_minimum_required(VERSION 3.1.0)

set(PROJECT "ExamplePlugins")

PROJECT(${PROJECT})

add_subdirectory(ExampleView)
add_subdirectory(ExampleAnalysis)
add_subdirectory(ExampleLoader)
add_subdirectory(ExampleData)
