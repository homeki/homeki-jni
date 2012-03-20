find_package(JNI REQUIRED)
find_package(Boost REQUIRED COMPONENTS thread)
find_path(TELLDUS_H telldus-core.h)
find_library(TELLDUS telldus-core)

set(SRC 
        tellstickeventqueue.cpp
        tellstick.cpp) 
        
include_directories(${PROJECT_SOURCE_DIR})
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${JAVA_INCLUDE_PATH})
include_directories(${Boost_INCLUDE_DIRS})
include_directories(${TELLDUS_H})

add_library(homekijni SHARED ${SRC})

target_link_libraries(homekijni ${TELLDUS} ${Boost_LIBRARIES})

