#-------------------------------------------------
#
# Project created by QtCreator 2014-01-09T07:39:12
#
#-------------------------------------------------

QT       += core gui sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QuickManager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    maindatabase.cpp \
    alltaluka.cpp \
    taluka.cpp \
    adduser.cpp \
    talukadialog.cpp

HEADERS  += mainwindow.h \
    maindatabase.h \
    alltaluka.h \
    taluka.h \
    adduser.h \
    talukadialog.h

FORMS    += mainwindow.ui \
    adduser.ui \
    talukadialog.ui
