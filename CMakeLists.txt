project(AGV_Cmake)
cmake_minimum_required(VERSION 2.8)

#设置依赖库Qt5
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5Gui)
find_package(Qt5Core)
find_package(Qt5Xml)

# Instruct CMake to run moc automatically when needed.
#set(CMAKE_AUTOMOC ON)

SET(AGV_Cmake_SRCS main.cpp mainwindow.cpp myimagewidget.cpp vgacarfunctionthread.cpp coordinatetransform.cpp aboutdialog.cpp Joystick.cpp)
SET(AGV_Cmake_MOC_SRCS mainwindow.h myimagewidget.h vgacarfunctionthread.h coordinatetransform.h aboutdialog.h Joystick.h)
SET(AGV_Cmake_UIS mainwindow.ui myimagewidget.ui aboutdialog.ui)
SET(AGV_Cmake_QRCS main.qrc)

QT5_WRAP_CPP(AGV_Cmake_MOCS ${AGV_Cmake_MOC_SRCS})
QT5_WRAP_UI(AGV_Cmake_GUI_UIS ${AGV_Cmake_UIS})
QT5_ADD_RESOURCES(AGV_Cmake_RCC_QRCS ${AGV_Cmake_QRCS})

SET(AGV_Cmake_ALL_SRCS ${AGV_Cmake_SRCS} ${AGV_Cmake_MOCS} ${AGV_Cmake_GUI_UIS} ${AGV_Cmake_RCC_QRCS})

# Tell CMake to create the Project executable
add_executable(${PROJECT_NAME} ${AGV_Cmake_ALL_SRCS})

# Use the Widgets module from Qt 5.
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::Gui Qt5::Xml dinput8.lib dxguid.lib)
