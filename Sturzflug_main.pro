#-------------------------------------------------
#
# Project created by QtCreator 2013-05-03T13:25:51
#
#-------------------------------------------------

QT       += core gui
contains(QT_VERSION, ^5\\..*\\..*): QT += widgets
#greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Sturzflug_main
TEMPLATE = app

include(qextserial/qextserialport.pri)

SOURCES += main.cpp\
        mainwindow.cpp\
    ips.cpp \
    weg.cpp \
    simulation.cpp


HEADERS  += mainwindow.h \
    ips.h \
    weg.h \
    define.h \
    simulation.h

FORMS    += mainwindow.ui



