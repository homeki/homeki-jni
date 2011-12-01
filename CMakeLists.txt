cmake_minimum_required(VERSION 2.6)

project(homekijni)

find_package(JNI REQUIRED)

set(homekijni_lib_src telldusjni.cpp) 

include_directories(${JAVA_INCLUDE_PATH})

add_library(homekijni SHARED ${homekijni_lib_src})
