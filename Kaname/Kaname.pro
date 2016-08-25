#-------------------------------------------------
#
# Project created by QtCreator 2016-08-16T14:59:19
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Kaname
TEMPLATE = app

CONFIG += c++11

INCLUDEPATH += include

SOURCES += src/main.cpp\
        src/Kaname.cpp \
    src/AbstractImageSource.cpp \
    src/StillImageSource.cpp \
    src/ImageSourceCacheMgr.cpp \
    src/MarkingWidget.cpp \
    src/MarkingBoxManager.cpp

HEADERS  += include/Kaname.h \
    include/AbstractImageSource.h \
    include/StillImageSource.h \
    include/ImageSourceCacheMgr.h \
    include/MarkingWidget.h \
    include/MarkingBoxManager.h

FORMS    += Kaname.ui

RESOURCES += \
    resources/resources.qrc

