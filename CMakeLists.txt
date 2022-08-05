cmake_minimum_required(VERSION 3.21)
project(string_view_TF_IDF)
# the following options prevent compiler-optimization issues that are unwanted in an edu process
set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -O0 -Werror=return-type")
set(CMAKE_C_FLAGS_DEBUG "${CMAKE_C_FLAGS_DEBUG} -O0")

set(CMAKE_CXX_STANDARD 20)

add_executable(string_view_TF_IDF main.cpp words.h v2.cpp v3.cpp prototype.cpp v4.cpp v5.cpp fstringView.cpp v6.cpp v7.cpp v10.cpp v11.cpp v12.cpp v13.cpp v14.cpp v152ndtest.cpp v16.cpp v17.cpp v18.cpp)
