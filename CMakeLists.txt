cmake_minimum_required(VERSION 2.8)

project(homekijni CXX)

# Add custom module directory (custom FindX.cmake files)
set(CMAKE_MODULE_PATH "${CMAKE_SOURCE_DIR}/cmake/Modules/")

# Find packages
find_package(JNI REQUIRED)
find_package(Telldus REQUIRED)

# Add include directories
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${JAVA_INCLUDE_PATH})
include_directories(${TELLDUS_INCLUDE_DIRS})

# Setup output locations
set(LIBRARY_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/lib)
set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/bin)

add_subdirectory(src)
