#-------------------------------------------------
#
# Project created by QtCreator 2016-08-16T14:59:19
#
#-------------------------------------------------

QT       += core gui opengl

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
    src/ObjectNameEditor.cpp \
    src/About.cpp \
    src/Hash128Result.cpp \
    src/BoxManager.cpp \
    src/GraphicsBoxItem.cpp \
    src/LabelingGraphicsView.cpp \
    src/LabelingScene.cpp \
    src/LabelingBox.cpp \
    src/ObjectInfo.cpp \
    src/ObjectSelectionButton.cpp

HEADERS  += include/Kaname.h \
    include/AbstractImageSource.h \
    include/StillImageSource.h \
    include/ImageSourceCacheMgr.h \
    include/Kaname_global.h \
    include/ObjectNameEditor.h \
    include/About.h \
    include/LabelDataFormatInterface.h \
    include/Hash128Result.h \
    include/BoxManager.h \
    include/GraphicsBoxItem.h \
    include/LabelingGraphicsView.h \
    include/LabelingScene.h \
    include/LabelingBox.h \
    include/ObjectInfo.h \
    include/ObjectSelectionButton.h \
    include/LabelingObjectDefinitionInterface.h

FORMS    += form/Kaname.ui \
    form/ObjectNameEditor.ui \
    form/About.ui

RESOURCES += \
    resources/resources.qrc

gcc:QMAKE_LFLAGS += -static-libgcc

RC_FILE += \
    resources/kaname.rc
