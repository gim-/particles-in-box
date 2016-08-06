#-------------------------------------------------
#
# Project created by QtCreator 2014-01-23T21:16:31
#
#-------------------------------------------------

QT     += core gui opengl widgets printsupport
CONFIG += C++11

TARGET = ParticlesInBox
TEMPLATE = app

LIBS += -lopengl32

SOURCES += src/main.cpp \
    src/demonstrationwindow.cpp \
    src/newexperimentwindow.cpp \
    src/clickablelabel.cpp \
    src/particlewidget.cpp \
    src/world.cpp \
    src/qcustomplot/qcustomplot.cpp

FORMS  += src/ui/DemonstrationWindow.ui src/ui/NewExperimentWindow.ui

HEADERS += \
    src/newexperimentwindow.h \
    src/demonstrationwindow.h \
    src/clickablelabel.h \
    src/particlewidget.h \
    src/world.h \
    src/qcustomplot/qcustomplot.h
