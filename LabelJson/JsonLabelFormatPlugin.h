/**************************************************************************
 * Copyright (C) 2016 Afa.L Cheng (afa@afa.moe)
 *
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 ***************************************************************************/

#ifndef LABELJSONPLUGIN_H
#define LABELJSONPLUGIN_H

#define JSON_LABEL_FORMAT_PLUGIN_GUID "59ED2ED0-889E-4175-A0E1-B5C8297B8FF8"
#define JSON_LABEL_FORMAT_PLUGIN_VERSION "1.0.0"
#define JSON_LABEL_FORMAT_PLUGIN_COPYRIGHT "Copyright (c) 2016, Afa.L Cheng <afa@afa.moe>"


#include <QObject>

#include "LabelDataFormatInterface.h"

class JsonLabelForamtPlugin : public QObject, LabelDataFormatInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "moe.afa.utils.Kaneme.LabelDataFormatInterface" FILE "JsonLabelFormatPlugin.json")
    Q_INTERFACES(LabelDataFormatInterface)
public:
    bool save(const QMap<qint64, QPair<QString, QVector<QRect>>> &boxes,
              const QString &savePath,
              const QVector<QString> &objnames) Q_DECL_OVERRIDE;
    QString formatDescription() Q_DECL_OVERRIDE;
    QString formatExtension() Q_DECL_OVERRIDE;
    QString formatName() Q_DECL_OVERRIDE;
    QString name() Q_DECL_OVERRIDE;
    QString version() Q_DECL_OVERRIDE;
    QString copyright() Q_DECL_OVERRIDE;
    QString guid() Q_DECL_OVERRIDE;
};

#endif // LABELJSONPLUGIN_H
