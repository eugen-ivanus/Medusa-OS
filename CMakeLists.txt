cmake_minimum_required(VERSION 3.22)
project(osdev)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt6 COMPONENTS
        Core
        Gui
        Widgets
        Network
        REQUIRED)

add_executable(osdev main.cpp
        TerminalWidget.h
        TerminalWidget.cpp
        AIWidget.h
        AIWidget.cpp
        benchmarkImplem.cpp
        benchmarkImplem.h
        systMon.cpp
        systMon.h
)
target_link_libraries(osdev
        Qt::Core
        Qt::Gui
        Qt::Widgets
        Qt::Network
)

