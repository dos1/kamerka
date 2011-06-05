SET(kamerka_SRCS 
  main.cpp
  mainwindow.cpp
  videowidget.cpp
  capturethread.cpp
)

SET(kamerka_RCCS kamerka.qrc)

QT4_ADD_RESOURCES(kamerka_RCC_SRCS ${kamerka_RCCS})

kde4_add_executable(kamerka ${kamerka_SRCS} ${kamerka_RCC_SRCS})
target_link_libraries(kamerka ${KDE4_KDEUI_LIBS} ${QT_QTDECLARATIVE_LIBRARY} ${QT_QTSCRIPT_LIBRARY} ${KDE4_PHONON_LIBS} ${LIBV4L2_LIBRARY} ${LIBV4LCONVERT_LIBRARY})

install(TARGETS kamerka DESTINATION ${BIN_INSTALL_DIR})
