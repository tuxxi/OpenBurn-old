cmake_minimum_required(VERSION 2.8.11)
project(OpenBurn)

set(CMAKE_CXX_STANDARD 14)
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

set(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -Wall -Wextra -Wpedantic")

# Find the QtWidgets library
find_package(Qt5Widgets)
get_target_property(QtWidgets_location Qt5::Widgets LOCATION)

find_package(Qt5Core)
get_target_property(QtCore_location Qt5::Core LOCATION)

find_package(Qt5Gui)
get_target_property(QtGui_location Qt5::Gui LOCATION)

find_package(Qt5PrintSupport)
get_target_property(QtPrintSupport_location Qt5::PrintSupport LOCATION)

add_executable(OpenBurn
        src/thirdparty/qcustomplot/qcustomplot.cpp
        src/thirdparty/qcustomplot/qcustomplot.h
        src/ui/dialogs/exportdialog.cpp
        src/ui/dialogs/exportdialog.hpp
        src/ui/dialogs/globalsettingsdialog.cpp
        src/ui/dialogs/globalsettingsdialog.hpp
        src/ui/dialogs/graindesigntypes.cpp
        src/ui/dialogs/graindesigntypes.hpp
        src/ui/dialogs/graindialog.cpp
        src/ui/dialogs/graindialog.hpp
        src/ui/dialogs/nozzledesigntypes.cpp
        src/ui/dialogs/nozzledesigntypes.hpp
        src/ui/dialogs/nozzledialog.cpp
        src/ui/dialogs/nozzledialog.hpp
        src/ui/dialogs/simsettingsdialog.cpp
        src/ui/dialogs/simsettingsdialog.hpp
        src/ui/graphics/graingraphicsitem.cpp
        src/ui/graphics/graingraphicsitem.hpp
        src/ui/graphics/motorgraphicsitem.cpp
        src/ui/graphics/motorgraphicsitem.hpp
        src/ui/graphics/nozzlegraphicsitem.cpp
        src/ui/graphics/nozzlegraphicsitem.hpp
        src/ui/widgets/graintablewidget.cpp
        src/ui/widgets/graintablewidget.hpp
        src/ui/widgets/unitscombobox.cpp
        src/ui/widgets/unitscombobox.hpp
        src/ui/designtab.cpp
        src/ui/designtab.hpp
        src/ui/mainwindow.cpp
        src/ui/mainwindow.hpp
        src/ui/propellanttab.cpp
        src/ui/propellanttab.hpp
        src/ui/simtab.cpp
        src/ui/simtab.hpp
        src/commands.hpp
        src/commands.cpp
        src/export.hpp
        src/export.cpp
        src/grain.cpp
        src/grain.hpp
        src/main.cpp
        src/motor.cpp
        src/motor.hpp
        src/motorsim.cpp
        src/motorsim.hpp
        src/nozzle.cpp
        src/nozzle.hpp
        src/propellant.cpp
        src/propellant.hpp
        src/settings.cpp
        src/settings.hpp
        src/units.cpp
        src/units.hpp
        src/util.cpp
        src/util.hpp)

target_link_libraries(OpenBurn Qt5::Widgets Qt5::Core Qt5::Gui Qt5::PrintSupport)
