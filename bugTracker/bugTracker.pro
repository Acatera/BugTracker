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
    datastore.cpp \
    issuedetailsui.cpp \
    settings.cpp

HEADERS  += mainwindow.h \
    issueitem.h \
    issue.h \
    datastore.h \
    issuedetailsui.h \
    settings.h

FORMS    += mainwindow.ui \
    issuedetailsui.ui

RESOURCES += \
    images.qrc

QMAKE_CXXFLAGS += -std=c++0x
