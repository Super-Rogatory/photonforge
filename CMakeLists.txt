cmake_minimum_required(VERSION 3.12)
project(PhotonForge)
set(CMAKE_CXX_STANDARD 17)
file(GLOB_RECURSE SOURCES src/*.cpp)
include_directories(src)
add_executable(photonforge ${SOURCES})
