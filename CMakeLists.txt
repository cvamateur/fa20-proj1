cmake_minimum_required(VERSION 3.10)

project(fa20-proj1)

set(CMAKE_C_STANDARD 11)

add_executable(imageloadertester imageloadertester.c imageloader.c)
add_executable(steganography steganography.c imageloader.c)
