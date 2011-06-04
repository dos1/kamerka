SET(kamerka_SRCS 
  main.cpp
  mainwindow.cpp
  videowidget.cpp
  capturethread.cpp
)

SET(kamerka_RCCS kamerka.qrc)

QT4_ADD_RESOURCES(kamerka_RCC_SRCS ${kamerka_RCCS})

kde4_add_executable(kamerka ${kamerka_SRCS} ${kamerka_RCC_SRCS})
target_link_libraries(kamerka ${KDE4_KDEUI_LIBS} ${QT_QTDECLARATIVE_LIBRARY} ${QT_QTSCRIPT_LIBRARY} ${QT_PHONON_LIBRARY} ${LIBV4L2_LIBRARY} "/usr/lib/libv4lconvert.so.0")

install(TARGETS kamerka DESTINATION ${BIN_INSTALL_DIR})
