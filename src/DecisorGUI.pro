#-------------------------------------------------
#
# Project created by QtCreator 2016-04-06T15:34:16
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DecisorGUI

TEMPLATE = app

SOURCES += main.cpp\
    mainwindow.cpp \
    form.cpp \
    Decisor.cpp \
    StringUtils.cpp \
    filereader.cpp \
    bocrwindow.cpp \
    alternative.cpp \
    filewriter.cpp \
    scalesettings.cpp

HEADERS  += mainwindow.h \
    form.h \
    Decisor.hpp \
    StringUtils.hpp \
    filereader.h \
    bocrwindow.h \
    alternative.h \
    filewriter.h \
    scalesettings.h

FORMS    += mainwindow.ui \
    bocrwindow.ui \
    scalesettings.ui

RC_FILE += icon.rc
