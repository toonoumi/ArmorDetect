cmake_minimum_required (VERSION 2.6)
project (ArmorDetect)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} -std=c++11)
set(CMAKE_PREFIX_PATH $ENV{QT5_PREFIX})
add_subdirectory(src)

#set(CMAKE_BINARY_DIR "./build")
#set(CMAKE_PREFIX_PATH "/usr/local")
