#-------------------------------------------------
#
# Project created by QtCreator 2016-08-25T19:13:46
#
#-------------------------------------------------

QT       += gui

TARGET = LabelJson
TEMPLATE = lib
CONFIG += plugin c++11 shared

SOURCES += \
    JsonLabelFormatPlugin.cpp

HEADERS += \
    JsonLabelFormatPlugin.h
DISTFILES += \
    JsonLabelFormatPlugin.json

INCLUDEPATH += ../Kaname/include
