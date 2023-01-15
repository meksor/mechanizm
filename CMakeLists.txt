cmake_minimum_required(VERSION 3.18.0)
project(mechanizm VERSION 0.1.0)

set(CMAKE_CXX_STANDARD 20)
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
  GIT_TAG "origin/master"
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/third
  CMAKE_CACHE_ARGS
    -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_CURRENT_BINARY_DIR}/third
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
)

SET(OSA_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/third/include/libopenshot-audio/)
SET(OSA_LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/third/lib)
SET(OSA_CMAKE_PATH ${CMAKE_CURRENT_BINARY_DIR}/third/src/libopenshot-audio-dl-build)

ExternalProject_Add(libopenshot-dl
  GIT_REPOSITORY "https://github.com/OpenShot/libopenshot"
  GIT_TAG "7617e9165e67ef68525aa9cc247dd54bd384c724"
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/third
  CMAKE_CACHE_ARGS
    -DENABLE_MAGICK:BOOL=0
    -DENABLE_RUBY:BOOL=0
    -DENABLE_PYTHON:BOOL=0
    -DENABLE_OPENCV:BOOL=0
    -DCMAKE_INSTALL_PREFIX:STRING=${CMAKE_CURRENT_BINARY_DIR}/third
    -DCMAKE_BUILD_TYPE:STRING=${CMAKE_BUILD_TYPE}
    -DCMAKE_PREFIX_PATH:STRING=${OSA_CMAKE_PATH}
)

SET(OS_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/third/include/libopenshot/)
SET(OS_LIB_DIR ${CMAKE_CURRENT_BINARY_DIR}/third/lib)


ExternalProject_Add(cxxmidi-dl
  GIT_REPOSITORY "https://github.com/5tan/cxxmidi"
  PREFIX ${CMAKE_CURRENT_BINARY_DIR}/third
  BUILD_COMMAND ""
  CONFIGURE_COMMAND ""
  INSTALL_COMMAND 
    mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/third/include/cxxmidi &&
    cp -r ${CMAKE_CURRENT_BINARY_DIR}/third/src/cxxmidi-dl/include/cxxmidi/ ${CMAKE_CURRENT_BINARY_DIR}/third/include/;
)

SET(CM_INCLUDE_DIR ${CMAKE_CURRENT_BINARY_DIR}/third/include/)

add_library(libopenshot-audio SHARED IMPORTED)
set_target_properties(libopenshot-audio PROPERTIES IMPORTED_LOCATION ${OSA_LIB_DIR}/libopenshot-audio.so)
include_directories(${OSA_INCLUDE_DIR})
#link_directories(${OSA_LIB_DIR})

add_library(libopenshot SHARED IMPORTED)
set_target_properties(libopenshot PROPERTIES IMPORTED_LOCATION ${OS_LIB_DIR}/libopenshot.so)
include_directories(${OS_INCLUDE_DIR})
#link_directories(${OS_LIB_DIR})

add_library(cxxmidi SHARED IMPORTED)
include_directories(${CM_INCLUDE_DIR})


add_subdirectory(src)
include_directories(src)

target_link_libraries(mechanizm libopenshot)
add_dependencies(mechanizm libopenshot-dl)

target_link_libraries(mechanizm libopenshot-audio)
add_dependencies(mechanizm libopenshot-audio-dl)

target_link_libraries(mechanizm jsoncpp)
target_link_libraries(mechanizm Qt5::Widgets)
