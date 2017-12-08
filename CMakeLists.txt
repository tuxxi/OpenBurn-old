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
        src/ui/dialogs/globalsettingsdialog.cpp
        src/ui/dialogs/globalsettingsdialog.h
        src/ui/dialogs/graindesigntypes.cpp
        src/ui/dialogs/graindesigntypes.h
        src/ui/dialogs/graindialog.cpp
        src/ui/dialogs/graindialog.h
        src/ui/dialogs/nozzledesigntypes.cpp
        src/ui/dialogs/nozzledesigntypes.h
        src/ui/dialogs/nozzledialog.cpp
        src/ui/dialogs/nozzledialog.h
        src/ui/dialogs/simsettingsdialog.cpp
        src/ui/dialogs/simsettingsdialog.h
        src/ui/graphics/graingraphicsitem.cpp
        src/ui/graphics/graingraphicsitem.h
        src/ui/graphics/motorgraphicsitem.cpp
        src/ui/graphics/motorgraphicsitem.h
        src/ui/graphics/nozzlegraphicsitem.cpp
        src/ui/graphics/nozzlegraphicsitem.h
        src/ui/widgets/graintablewidget.cpp
        src/ui/widgets/graintablewidget.h
        src/ui/widgets/unitscombobox.cpp
        src/ui/widgets/unitscombobox.h
        src/ui/designtab.cpp
        src/ui/designtab.h
        src/ui/mainwindow.cpp
        src/ui/mainwindow.h
        src/ui/propellanttab.cpp
        src/ui/propellanttab.h
        src/ui/simtab.cpp
        src/ui/simtab.h
		src/commands.h
		src/commands.cpp
        src/grain.cpp
        src/grain.h
        src/main.cpp
        src/motor.cpp
        src/motor.h
        src/motorsim.cpp
        src/motorsim.h
        src/nozzle.cpp
        src/nozzle.h
        src/propellant.cpp
        src/propellant.h
        src/settings.cpp
        src/settings.h
        src/units.cpp
        src/units.h
        src/util.cpp
        src/util.h)

target_link_libraries(OpenBurn Qt5::Widgets Qt5::Core Qt5::Gui Qt5::PrintSupport)
