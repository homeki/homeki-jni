cmake_minimum_required(VERSION 2.6)

project(homekijni)

set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)

find_package(JNI REQUIRED)
find_library(TELLDUS telldus-core) 

set(homekijni_lib_src tellstick.cpp) 

include_directories(${JAVA_INCLUDE_PATH})
include_directories(${PROJECT_SOURCE_DIR}/include)

add_library(homekijni SHARED ${homekijni_lib_src})

target_link_libraries(homekijni ${TELLDUS})
