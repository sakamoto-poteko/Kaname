#ifndef YOLOLABELFORMATPLUGIN_H
#define YOLOLABELFORMATPLUGIN_H

#include <QObject>

#include "LabelDataFormatInterface.h"

#define YOLO_LABEL_FORMAT_PLUGIN_GUID "{8DA7315D-5996-4EE6-889A-B9ABA7118229}"
#define YOLO_LABEL_FORMAT_PLUGIN_VERSION "0.9.0"
#define YOLO_LABEL_FORMAT_PLUGIN_COPYRIGHT "Copyright (c) 2019, Afa.L Cheng <afa@afa.moe>"


class YoloLabelFormatPlugin : public QObject, LabelDataFormatInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "moe.afa.utils.Kaneme.LabelDataFormatInterface" FILE "YoloLabelFormatPlugin.json")
    Q_INTERFACES(LabelDataFormatInterface)

public:
    YoloLabelFormatPlugin(QObject *parent = 0);
    bool save(const BoxManager &boxes, const QString &savePath);
    bool open(const QString &openPath, BoxManager *boxMan, const QString &imgPath);
    QString formatDescription();
    QString formatExtension();
    QString formatName();
    QString name();
    QString version();
    QString copyright();
    QString guid();
};

#endif // YOLOLABELFORMATPLUGIN_H
