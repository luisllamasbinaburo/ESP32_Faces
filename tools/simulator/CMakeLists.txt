cmake_minimum_required(VERSION 3.10)
project(face_sim CXX)

# Designated initializers in EyePresets.h need C++20 on MSVC (GCC/Clang accept them earlier)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../../src)
file(GLOB LIB_SOURCES ${LIB_DIR}/*.cpp)

add_executable(face_sim main.cpp ${LIB_SOURCES})
target_include_directories(face_sim PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/arduino ${LIB_DIR})

if(MSVC)
  target_compile_options(face_sim PRIVATE /W3 /wd4244 /wd4305 /D_CRT_SECURE_NO_WARNINGS)
else()
  target_compile_options(face_sim PRIVATE -Wall -Wextra -Wno-missing-field-initializers)
endif()
