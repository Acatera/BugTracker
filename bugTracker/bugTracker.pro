#-------------------------------------------------
#
# Project created by QtCreator 2015-04-17T19:20:57
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = bugTracker
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    issueitem.cpp \
    issue.cpp \
    datastore.cpp

HEADERS  += mainwindow.h \
    issueitem.h \
    issue.h \
    datastore.h

FORMS    += mainwindow.ui
