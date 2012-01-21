cmake_minimum_required(VERSION 2.6)

project(homekijni CXX)

#set(CMAKE_CXX_FLAGS "-m32 ${CMAKE_CXX_FLAGS}")
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(src)