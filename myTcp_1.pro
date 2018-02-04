#-------------------------------------------------
#
# Project created by QtCreator 2016-08-17T11:32:44
#
#-------------------------------------------------

QT       += core gui network
QT       += xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = myTcp_1
TEMPLATE = app


SOURCES += main.cpp \
    server.cpp \
    jlproto.cpp \
    zclTools.cpp

HEADERS  += \
    server.h \
    jlproto.h \
    zclTools.h

FORMS    += \
    server.ui
