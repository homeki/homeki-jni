cmake_minimum_required(VERSION 2.6)
project(homekijni)

set(homekijni_lib_src telldusjni.cpp) 
include_directories(/usr/lib/jvm/java-6-openjdk/include)

add_library(homekijni SHARED ${homekijni_lib_src})
