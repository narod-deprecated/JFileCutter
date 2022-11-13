#-------------------------------------------------
#
# Project created by QtCreator 2015-06-15T22:00:54
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = jFileCutter
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    listener.cpp \
    progressdialog.cpp \
    threadrunner.cpp

HEADERS  += mainwindow.h \
    listener.h \
    progressdialog.h \
    threadrunner.h

FORMS    += mainwindow.ui \
    progressdialog.ui

win32:RC_FILE = icon.rc

win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-fileCutterLib-Desktop_Qt_5_3/release/ -lfileCutterLib
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-fileCutterLib-Desktop_Qt_5_3/debug/ -lfileCutterLib

unix:CONFIG(release, debug|release): LIBS += -L$$PWD/../build-fileCutterLib-Desktop_Qt_5_3/release/ -lfileCutterLib
else:unix:CONFIG(debug, debug|release): LIBS += -L$$PWD/../build-fileCutterLib-Desktop_Qt_5_3/debug/ -lfileCutterLib

INCLUDEPATH += $$PWD/../fileCutterLib

DEPENDPATH += $$PWD/../build-fileCutterLib-Desktop_Qt_5_3/debug
DEPENDPATH += $$PWD/../build-fileCutterLib-Desktop_Qt_5_3/release
