#-------------------------------------------------
#
# Project created by QtCreator 2016-05-11T09:58:51
#
#-------------------------------------------------

QT       += core gui
CONFIG  += c++11

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets printsupport

TARGET = nm_iz
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    qcustomplot.cpp \
    nmpt_simulator.cpp

HEADERS  += mainwindow.h \
    qcustomplot.h \
    nmpt_simulator.h

FORMS    += mainwindow.ui
