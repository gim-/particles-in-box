#-------------------------------------------------
#
# Project created by QtCreator 2014-01-23T21:16:31
#
#-------------------------------------------------

QT     += core gui opengl widgets
CONFIG += C++11

TARGET = ParticlesInBox
TEMPLATE = app

SOURCES += src/main.cpp \
    src/demonstrationwindow.cpp \
    src/newexperimentwindow.cpp

FORMS  += src/ui/DemonstrationWindow.ui src/ui/NewExperimentWindow.ui

HEADERS += \
    src/newexperimentwindow.h \
    src/demonstrationwindow.h
