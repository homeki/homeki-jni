find_package(JNI REQUIRED)
find_path(TELLDUS_H telldus-core.h)
find_library(TELLDUS telldus-core)

set(SRC 
        tellstickeventqueue.cpp
        tellstick.cpp) 

add_library(homekijni SHARED ${SRC})

target_link_libraries(homekijni ${TELLDUS_LIBRARIES})
