SET(kamerka_SRCS 
  main.cpp
  mainwindow.cpp
  videowidget.cpp
  capturethread.cpp
  kamerka.qml
  Button.qml
  settingsdialog.cpp
  kamerka.kcfg
)

SET(kamerka_RCCS kamerka.qrc)

SET(kamerka_KCFGS settings.kcfgc)

qt4_add_resources(kamerka_RCC_SRCS ${kamerka_RCCS})

kde4_add_kcfg_files(kamerka_SRCS ${kamerka_KCFGS})

kde4_add_executable(kamerka ${kamerka_SRCS} ${kamerka_RCC_SRCS})
target_link_libraries(kamerka ${KDE4_KDEUI_LIBS} ${KDE4_KIO_LIBS} ${QT_QTDECLARATIVE_LIBRARY} ${QT_QTSCRIPT_LIBRARY} ${KDE4_PHONON_LIBS} ${LIBV4L2_LIBRARY} ${LIBV4LCONVERT_LIBRARY})

install(TARGETS kamerka DESTINATION ${BIN_INSTALL_DIR})
install(FILES ${kamerka_KCFGS} DESTINATION ${KCFG_INSTALL_DIR})
