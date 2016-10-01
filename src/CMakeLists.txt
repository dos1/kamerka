SET(kamerka_SRCS 
  main.cpp
  mainwindow.cpp
  videowidget.cpp
  capturethread.cpp
  imageeffect.cpp
  kamerka.qml
  Button.qml
  settingsdialog.cpp
  kamerka.kcfg
)

SET(kamerka_RCCS kamerka.qrc)

SET(kamerka_KCFGCS settings.kcfgc)
SET(kamerka_KCFGS kamerka.kcfg)

qt5_add_resources(kamerka_RCC_SRCS ${kamerka_RCCS})

kconfig_add_kcfg_files(kamerka_SRCS ${kamerka_KCFGCS})

add_executable(kamerka ${kamerka_SRCS} ${kamerka_RCC_SRCS})
target_link_libraries(kamerka Qt5::Core Qt5::Gui Qt5::Script Qt5::Qml Qt5::Quick Qt5::QuickWidgets KF5::ConfigCore KF5::ConfigGui KF5::CoreAddons KF5::Declarative KF5::WidgetsAddons KF5::KIOFileWidgets KF5::I18n KF5::KDELibs4Support KF5::Notifications ${PHONON_LIBRARY} ${LIBV4L2_LIBRARY} ${LIBV4LCONVERT_LIBRARY} ${QIMAGEBLITZ_LIBRARIES})

install(TARGETS kamerka DESTINATION ${BIN_INSTALL_DIR})
install(FILES ${kamerka_KCFGS} DESTINATION ${KCFG_INSTALL_DIR})
