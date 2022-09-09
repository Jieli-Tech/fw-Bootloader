cmake_minimum_required(VERSION 3.16)

project(UBootSerialUpdateDemoPrj VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

include_directories(./utils ./)

find_package(Qt6 REQUIRED COMPONENTS Core SerialPort Widgets)
qt_standard_project_setup()

add_executable(UBootSerialUpdateDemo
    comm/physerialthread.cpp
    main.cpp
    updatethread.cpp
    utils/utils.cpp
    mainwindow.cpp
)

target_link_libraries(UBootSerialUpdateDemo PRIVATE Qt6::Core Qt6::SerialPort Qt6::Widgets)
