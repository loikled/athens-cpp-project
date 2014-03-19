#-------------------------------------------------
#
# Project created by QtCreator 2014-03-17T15:55:55
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = projet_athens
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    mytextedit.cpp \
    protocolhandler.cpp \
    packet.cpp \
    packetnick.cpp \
    packetjoin.cpp

HEADERS  += mainwindow.h \
    mytextedit.h \
    protocolhandler.h \
    packet.h \
    packetnick.h \
    packetjoin.h
