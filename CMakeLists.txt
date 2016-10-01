cmake_minimum_required(VERSION 2.8.12)

project(Kamerka)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall")

find_package(ECM 0.0.11 REQUIRED NO_MODULE)
set(CMAKE_MODULE_PATH ${ECM_MODULE_PATH} ${ECM_KDE_MODULE_DIR} ${PROJECT_SOURCE_DIR}/modules)

find_package(Qt5 REQUIRED COMPONENTS Core Script Gui Widgets Qml Quick QuickWidgets)

include(KDEInstallDirs)
include(KDECMakeSettings)
include(KDECompilerSettings)

include(FeatureSummary)

find_package(KF5 REQUIRED COMPONENTS CoreAddons WidgetsAddons Config I18n KIO Notifications KDELibs4Support Declarative)
include_directories(${KF5CoreAddons_INCLUDES} ${KF5Config_INCLUDES} ${KF5I18n_INCLUDES} ${KF5KIO_INCLUDES} ${KF5Notifications_INCLUDES} ${KF5KDELibs4Support_INCLUDES} ${KF5Declarative_INCLUDES})

find_package(Phonon4Qt5 REQUIRED)
include_directories(${PHONON_INCLUDE_DIR})

find_package(QImageBlitz 5.0.0 REQUIRED)
include_directories(${QIMAGEBLITZ_INCLUDE_DIR})

find_package(LibV4L2 REQUIRED)
include_directories(${LIBV2L2_INCLUDE_DIR})

find_package(LibV4LConvert REQUIRED)
include_directories(${LIBV2LCONVERT_INCLUDE_DIR})

add_subdirectory(src)
add_subdirectory(po)
add_subdirectory(data)

feature_summary(WHAT ALL FATAL_ON_MISSING_REQUIRED_PACKAGES)
