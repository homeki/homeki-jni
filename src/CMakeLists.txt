find_package(JNI REQUIRED)
find_path(TELLDUS_H telldus-core.h)
find_library(TELLDUS telldus-core)

set(SRC 
        tellstickeventqueue.cpp
        tellstick.cpp) 
        
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${JAVA_INCLUDE_PATH})
include_directories(${TELLDUS_H})

add_library(homekijni SHARED ${SRC})

target_link_libraries(homekijni ${TELLDUS})
