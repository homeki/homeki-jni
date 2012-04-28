# Defines
#  TELLDUS_FOUND - if system contains libtelldus-core.so
#  TELLDUS_INCLUDE_DIRS - where to find include files
#  TELLDUS_LIBRARIES - where to find the so

find_path(TELLDUS_INCLUDE_DIRS telldus-core.h HINTS /opt/telldus/include)
find_library(TELLDUS_LIBRARIES telldus-core HINTS /opt/telldus/lib)

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(Telldus DEFAULT_MSG TELLDUS_LIBRARIES TELLDUS_INCLUDE_DIRS)

