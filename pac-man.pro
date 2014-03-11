#-------------------------------------------------
#
# Project created by QtCreator 2014-03-05T23:07:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = pac-man
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    grid.cpp \
    gamepacman.cpp \
    constants.cpp

HEADERS  += mainwindow.h \
    grid.h \
    gamepacman.h

FORMS    += mainwindow.ui

RESOURCES += \
    main.qrc

CONFIG += c++11
