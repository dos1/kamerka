project (kamerka)

SET(CMAKE_CXX_FLAGS "-Wall") # release

#SET(CMAKE_CXX_FLAGS "-g -Wall") # debug

SET(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/modules)

find_package(Qt4 REQUIRED)

 find_package(KDE4 REQUIRED)
include(KDE4Defaults)
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
target_link_libraries(kamerka ${KDE4_KDEUI_LIBS} ${QT_QTDECLARATIVE_LIBRARY} ${QT_QTSCRIPT_LIBRARY} ${QT_PHONON_LIBRARY} ${LIBV4L2_LIBRARY} "/usr/lib/libv4lconvert.so.0")

add_subdirectory(po)

install(TARGETS kamerka DESTINATION ${BIN_INSTALL_DIR})
install(FILES kamerka.desktop DESTINATION ${XDG_APPS_INSTALL_DIR})
install(FILES camera_click.ogg timer.ogg kamerka.notifyrc DESTINATION "${DATA_INSTALL_DIR}/kamerka")
install(FILES kamerka.png DESTINATION ${ICON_INSTALL_DIR})
