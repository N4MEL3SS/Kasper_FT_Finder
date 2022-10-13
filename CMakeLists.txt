cmake_minimum_required(VERSION 3.24)
project(Finder)

set(CMAKE_C_STANDARD 99)
add_compile_options(-Wall -Wextra -Werror)

add_executable(finder finder.c)
