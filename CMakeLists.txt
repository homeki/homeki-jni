cmake_minimum_required(VERSION 2.6)
project(telldusjni)

set(telldusjni_lib_src telldusjni.cpp) 
include_directories(/usr/lib/jvm/java-6-openjdk/include)

add_library(telldusjni SHARED ${telldusjni_lib_src})
