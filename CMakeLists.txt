project (kamerka)

SET(CMAKE_CXX_FLAGS "-Wall") # release

#SET(CMAKE_CXX_FLAGS "-g -Wall") # debug

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/modules)

find_package(Qt4 REQUIRED)
 
find_package(KDE4 REQUIRED)
include_directories(${KDE4_INCLUDES})

find_package(LibV4L2 REQUIRED)
include_directories(${LIBV2L2_INCLUDE_DIR})

set(kamerka_SRCS 
  main.cpp
  mainwindow.cpp
  videowidget.cpp
  capturethread.cpp
)

SET(kamerka_RCCS kamerka.qrc)

QT4_ADD_RESOURCES(kamerka_RCC_SRCS ${kamerka_RCCS})

kde4_add_executable(kamerka ${kamerka_SRCS} ${kamerka_RCC_SRCS})
target_link_libraries(kamerka ${KDE4_KDEUI_LIBS} ${QT_QTDECLARATIVE_LIBRARY} ${LIBV4L2_LIBRARY} "/usr/lib/libv4lconvert.so.0")
