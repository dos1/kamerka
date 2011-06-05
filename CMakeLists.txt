project (kamerka)

SET(CMAKE_CXX_FLAGS "-Wall")

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/modules)

find_package(Qt4 REQUIRED)

find_package(KDE4 REQUIRED)
include(KDE4Defaults)
include_directories(${KDE4_INCLUDES})

find_package(LibV4L2 REQUIRED)
include_directories(${LIBV2L2_INCLUDE_DIR})

find_package(LibV4LConvert REQUIRED)
include_directories(${LIBV2LCONVERT_INCLUDE_DIR})

add_subdirectory(src)
add_subdirectory(po)
add_subdirectory(data)
