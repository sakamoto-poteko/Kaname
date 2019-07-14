#ifndef VOCLABELFORMATPLUGIN_H
#define VOCLABELFORMATPLUGIN_H

#include <QObject>

#include <QObject>

#include "LabelDataFormatInterface.h"

#define JSON_LABEL_FORMAT_PLUGIN_GUID "{59ED2ED0-889E-4175-A0E1-B5C8297B8FF8}"
#define JSON_LABEL_FORMAT_PLUGIN_VERSION "0.9.0"
#define JSON_LABEL_FORMAT_PLUGIN_COPYRIGHT "Copyright (c) 2019, Afa.L Cheng <afa@afa.moe>"

class VOCLabelFormatPlugin : public QObject, LabelDataFormatInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "moe.afa.utils.Kaneme.LabelDataFormatInterface" FILE "VOCLabelFormatPlugin.json")
    Q_INTERFACES(LabelDataFormatInterface)
public:
    virtual bool save(const BoxManager &boxes, const QString &savePath) Q_DECL_OVERRIDE;
    virtual bool open(const QString &openPath, BoxManager *boxman, const QString &imgPath) Q_DECL_OVERRIDE;

    QString formatDescription() Q_DECL_OVERRIDE;
    QString formatExtension() Q_DECL_OVERRIDE;
    QString formatName() Q_DECL_OVERRIDE;
    QString name() Q_DECL_OVERRIDE;
    QString version() Q_DECL_OVERRIDE;
    QString copyright() Q_DECL_OVERRIDE;
    QString guid() Q_DECL_OVERRIDE;
};

#endif // VOCLABELFORMATPLUGIN_H
