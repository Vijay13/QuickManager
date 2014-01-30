#-------------------------------------------------
#
# Project created by QtCreator 2014-01-09T07:39:12
#
#-------------------------------------------------

QT       += core gui sql printsupport

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = QuickManager
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    maindatabase.cpp \
    adduser.cpp \
    talukadialog.cpp \
    schooldialog.cpp \
    talukamanager.cpp \
    DataModel/allschool.cpp \
    DataModel/alltaluka.cpp \
    DataModel/school.cpp \
    DataModel/taluka.cpp \
    DataManager/filemanager.cpp \
    DataManager/schoolbillmanager.cpp \
    DataManager/schoolmanager.cpp \
    DataModel/qmerror.cpp \
    DataModel/allerror.cpp

HEADERS  += mainwindow.h \
    maindatabase.h \
    adduser.h \
    talukadialog.h \
    schooldialog.h \
    talukamanager.h \
    DataModel/allschool.h \
    DataModel/alltaluka.h \
    DataModel/school.h \
    DataModel/taluka.h \
    DataManager/filemanager.h \
    DataManager/schoolbillmanager.h \
    DataManager/schoolmanager.h \
    DataModel/qmerror.h \
    DataModel/allerror.h

FORMS    += mainwindow.ui \
    adduser.ui \
    talukadialog.ui \
    schooldialog.ui \
    talukamanager.ui
