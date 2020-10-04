cmake_minimum_required(VERSION 3.18)

project(Mechanizm)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_BINARY_DIR ./build)

include(CheckIncludeFile)
include(CheckIncludeFileCXX)
include(CheckIncludeFiles)
include(ExternalProject)

ExternalProject_Add(libopenshot-audio
  GIT_REPOSITORY "https://github.com/OpenShot/libopenshot-audio"
  GIT_TAG "origin/develop"
  PREFIX ${CMAKE_BINARY_DIR}/libopenshot-audio
  INSTALL_COMMAND ""
)

ExternalProject_Add(libopenshot
  GIT_REPOSITORY "https://github.com/OpenShot/libopenshot"
  GIT_TAG "origin/develop"
  PREFIX ${CMAKE_BINARY_DIR}/libopenshot
  INSTALL_COMMAND ""
  CMAKE_CACHE_ARGS
    -DENABLE_MAGICK:BOOL=0
    -DENABLE_RUBY:BOOL=0
    -DENABLE_PYTHON:BOOL=0
)

add_subdirectory(src)

target_include_directories(mechanizm PUBLIC ${CMAKE_BINARY_DIR}/libopenshot/src/libopenshot/include)
target_include_directories(mechanizm PUBLIC ${CMAKE_BINARY_DIR}/libopenshot/src/libopenshot-build/include)
target_include_directories(mechanizm PUBLIC ${CMAKE_BINARY_DIR}/libopenshot-audio/src/libopenshot-audio-build/include)
