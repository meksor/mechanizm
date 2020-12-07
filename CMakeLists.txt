cmake_minimum_required(VERSION 3.18.0)
project(mechanizm VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED True)
# set(CMAKE_BINARY_DIR ./build)

set(CMAKE_AUTOMOC ON)
set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(CheckIncludeFile)
include(CheckIncludeFileCXX)
include(CheckIncludeFiles)
include(ExternalProject)

find_package(Qt5Widgets REQUIRED)
include_directories(/usr/include/qt/QtCore)

ExternalProject_Add(libopenshot-audio-dl
  GIT_REPOSITORY "https://github.com/OpenShot/libopenshot-audio"
  GIT_TAG "origin/develop"
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/third
  CMAKE_CACHE_ARGS
    -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_CURRENT_BINARY_DIR}/third
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
)

SET(OSA_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/third/include/libopenshot-audio/)
SET(OSA_LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/third/lib)

ExternalProject_Add(libopenshot-dl
  GIT_REPOSITORY "https://github.com/OpenShot/libopenshot"
  GIT_TAG "414a2cd"
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/third
  CMAKE_CACHE_ARGS
    -DENABLE_MAGICK:BOOL=0
    -DENABLE_RUBY:BOOL=0
    -DENABLE_PYTHON:BOOL=0
    -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_CURRENT_BINARY_DIR}/third
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
)

SET(OS_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/third/include/libopenshot/)
SET(OS_LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/third/lib)

add_library(libopenshot-audio SHARED IMPORTED)
set_target_properties(libopenshot-audio PROPERTIES IMPORTED_LOCATION ${OSA_LIB_DIR}/libopenshot-audio.so)
include_directories(${OSA_INCLUDE_DIR})
#link_directories(${OSA_LIB_DIR})

add_library(libopenshot SHARED IMPORTED)
set_target_properties(libopenshot PROPERTIES IMPORTED_LOCATION ${OS_LIB_DIR}/libopenshot.so)
include_directories(${OS_INCLUDE_DIR})
#link_directories(${OS_LIB_DIR})

add_subdirectory(src)
include_directories(src)

target_link_libraries(mechanizm libopenshot)
add_dependencies(mechanizm libopenshot-dl)

target_link_libraries(mechanizm libopenshot-audio)
add_dependencies(mechanizm libopenshot-audio-dl)

target_link_libraries(mechanizm jsoncpp)
target_link_libraries(mechanizm Qt5::Widgets)
