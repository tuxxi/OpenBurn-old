######################################################################
# Automatically generated by qmake (3.0) Thu Nov 16 16:06:14 2017
######################################################################

QT+= core gui
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport
TEMPLATE = app
TARGET = OpenBurn
INCLUDEPATH += .

# Input
HEADERS += src/grain.h \
           src/motor.h \
           src/motorsim.h \
           src/nozzle.h \
           src/propellant.h \
           src/settings.h \
           src/units.h \
           src/util.h \
           src/ui/designtab.h \
           src/ui/mainwindow.h \
           src/ui/propellanttab.h \
           src/ui/simtab.h \
           src/ui/dialogs/graindesigntypes.h \
           src/ui/dialogs/graindialog.h \
           src/ui/dialogs/nozzledesigntypes.h \
           src/ui/dialogs/nozzledialog.h \
           src/ui/dialogs/simsettingsdialog.h \
           src/ui/graphics/graingraphicsitem.h \
           src/ui/graphics/motorgraphicsitem.h \
           src/ui/graphics/nozzlegraphicsitem.h \
           src/ui/widgets/graintablewidget.h \
           src/ui/widgets/qcustomplot.h \
           src/ui/widgets/unitscombobox.h
SOURCES += src/grain.cpp \
           src/main.cpp \
           src/motor.cpp \
           src/motorsim.cpp \
           src/nozzle.cpp \
           src/propellant.cpp \
           src/settings.cpp \
           src/units.cpp \
           src/util.cpp \
           src/ui/designtab.cpp \
           src/ui/mainwindow.cpp \
           src/ui/propellanttab.cpp \
           src/ui/simtab.cpp \
           src/ui/dialogs/graindesigntypes.cpp \
           src/ui/dialogs/graindialog.cpp \
           src/ui/dialogs/nozzledesigntypes.cpp \
           src/ui/dialogs/nozzledialog.cpp \
           src/ui/dialogs/simsettingsdialog.cpp \
           src/ui/graphics/graingraphicsitem.cpp \
           src/ui/graphics/motorgraphicsitem.cpp \
           src/ui/graphics/nozzlegraphicsitem.cpp \
           src/ui/widgets/graintablewidget.cpp \
           src/ui/widgets/qcustomplot.cpp \
           src/ui/widgets/unitscombobox.cpp
