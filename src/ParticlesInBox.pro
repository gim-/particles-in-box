#-------------------------------------------------
#
# Project created by QtCreator 2014-01-23T21:16:31
#
#-------------------------------------------------

QT       += core gui
QT       += opengl

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = ParticlesInBox
TEMPLATE = app


HEADERS  += mainwindow.h \
    glwidget.h \
    aboutwindow.h \
    World.h \
    worldsettings.h \
    qcustomplot.h \
    calcthread.h

SOURCES += main.cpp\
        mainwindow.cpp \
    glwidget.cpp \
    aboutwindow.cpp \
    World.cpp \
    worldsettings.cpp \
    qcustomplot.cpp \
    calcthread.cpp


FORMS    += mainwindow.ui \
    aboutwindow.ui \
    worldsettings.ui
