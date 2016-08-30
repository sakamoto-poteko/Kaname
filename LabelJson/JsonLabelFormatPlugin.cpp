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

#include "JsonLabelFormatPlugin.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QFileInfo>
#include <QVector>
#include <QRect>

bool JsonLabelForamtPlugin::save(const QHash<Hash128Result, QPair<QString, QList<QRect>>> &boxes,
                                 const QString &savePath, const QVector<QString> &objnames)
{
    QFile file(savePath);

    if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
        return false;

    QJsonArray jsonImagesAry;
    auto keys = boxes.keys();
    foreach (auto key, keys) {
        auto img = boxes[key];
        if (img.first.isEmpty())
            continue;

        QJsonObject jsonImageObj;

        QJsonArray jsonBoxes;
        for (int i = 0; i < img.second.size(); ++i) {
            QRect box(img.second.at(i));

            QJsonObject jsonRectObj;
            jsonRectObj.insert("top", box.top());
            jsonRectObj.insert("left", box.left());
            jsonRectObj.insert("width", box.width());
            jsonRectObj.insert("height", box.height());

            if (i < objnames.size())
                jsonRectObj.insert("object", objnames.at(i));

            jsonBoxes.append(jsonRectObj);
        }
        jsonImageObj.insert("boxes", jsonBoxes);

        QFileInfo finfo(img.first);
        jsonImageObj.insert("file", finfo.fileName());

        jsonImageObj.insert("hash", key.toString());

        jsonImagesAry.append(jsonImageObj);
    }

    QJsonArray jsonObjNames;
    foreach (QString objname, objnames) {
        jsonObjNames.append(objname);
    }

    QJsonObject jsonRoot;
    jsonRoot.insert("images", jsonImagesAry);
    jsonRoot.insert("objectNames", jsonObjNames);

    QJsonDocument jsonDoc(jsonRoot);
    QByteArray docByteArray = jsonDoc.toJson();

    file.write(docByteArray);
    file.close();
    return true;
}

QHash<Hash128Result, QPair<QString, QList<QRect>>> JsonLabelForamtPlugin::open(const QString &openPath)
{
    throw("not implemented");
}

QString JsonLabelForamtPlugin::formatDescription()
{
    return "JSON format for labeling";
}

QString JsonLabelForamtPlugin::formatExtension()
{
    return "*.json";
}

QString JsonLabelForamtPlugin::formatName()
{
    return "Json";
}

QString JsonLabelForamtPlugin::name()
{
    return "Json Labeling Format";
}

QString JsonLabelForamtPlugin::version()
{
    return JSON_LABEL_FORMAT_PLUGIN_VERSION;
}

QString JsonLabelForamtPlugin::copyright()
{
    return JSON_LABEL_FORMAT_PLUGIN_COPYRIGHT;
}

QString JsonLabelForamtPlugin::guid()
{
    return JSON_LABEL_FORMAT_PLUGIN_GUID;
}


