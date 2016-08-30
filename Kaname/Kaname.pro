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
    src/MarkingBoxManager.cpp \
    src/ObjectNameEditor.cpp \
    src/About.cpp \
    src/Hash128Result.cpp

HEADERS  += include/Kaname.h \
    include/AbstractImageSource.h \
    include/StillImageSource.h \
    include/ImageSourceCacheMgr.h \
    include/MarkingWidget.h \
    include/MarkingBoxManager.h \
    include/Kaname_global.h \
    include/ObjectNameEditor.h \
    include/About.h \
    include/LabelDataFormatInterface.h \
    include/Hash128Result.h

FORMS    += form/Kaname.ui \
    form/ObjectNameEditor.ui \
    form/About.ui

RESOURCES += \
    resources/resources.qrc

gcc:QMAKE_LFLAGS += -static-libgcc

RC_FILE += \
    resources/kaname.rc
