cmake_minimum_required(VERSION 3.10)    # Minimum is likely much lower - just getting rid of cmake warning
project(fixed-integer-literals)

set(CMAKE_CXX_EXTENSIONS Off)

set(Sources TestMain.cpp)
set(Headers SizedIntLiterals.h)

add_executable(${PROJECT_NAME} ${Sources} ${Headers})
target_compile_features(${PROJECT_NAME} PUBLIC cxx_std_11)
