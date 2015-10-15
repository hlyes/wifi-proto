#-------------------------------------------------
#
# Project created by QtCreator 2015-10-06T16:25:46
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = ScenarioServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    client.cpp

HEADERS  += mainwindow.h \
    client.h

FORMS    += mainwindow.ui
