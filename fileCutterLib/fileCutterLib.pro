#-------------------------------------------------
#
# Project created by QtCreator 2015-06-17T10:16:13
#
#-------------------------------------------------

#QT        += core gui

QT       -= gui

TARGET = fileCutterLib
TEMPLATE = lib

#VERSION =  1.0.0
DEFINES += FILECUTTERLIB_LIBRARY

SOURCES += filecutterlib.cpp \
    strutils.cpp

HEADERS += filecutterlib.h\
        filecutterlib_global.h \
    strutils.h

unix {
    target.path = /usr/lib
    INSTALLS += target
}
