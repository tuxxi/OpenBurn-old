#-------------------------------------------------
#
# Project created by QtCreator 2017-09-29T13:49:04
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = OpenBurn
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which as been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


SOURCES += \
        src/main.cpp \
    src/grain.cpp \
    src/propellant.cpp \
    src/motorsim.cpp \
    src/nozzle.cpp \
    src/ui/graindialog.cpp \
    src/ui/mainwindow.cpp

HEADERS += \
    src/grain.h \
    src/propellant.h \
    src/motorsim.h \
    src/nozzle.h \
    src/ui/graindialog.h \
    src/ui/mainwindow.h \
    src/util.h

FORMS += \
    src/ui/graindialog.ui \
    src/ui/mainwindow.ui
