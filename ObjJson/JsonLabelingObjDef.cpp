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

#include "JsonLabelingObjDef.h"

#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonValue>
#include <QFile>


ObjectInfoList JsonLabelingObjDef::open(const QString &path)
{
    ObjectInfoList ret;

    QFile f(path);
    if (!f.open(QIODevice::ReadOnly))
        return ret;

    QByteArray jsonStr = f.readAll();

    QJsonParseError err;
    auto jsonDoc = QJsonDocument::fromJson(jsonStr, &err);
    if (err.error != QJsonParseError::NoError || !jsonDoc.isArray())
        return ret;
    auto jsonRootAry = jsonDoc.array();
    foreach (QJsonValue jsonObjVal, jsonRootAry) {
        if (!jsonObjVal.isObject())
            continue;

        ObjectInfo objInfo;
        QJsonObject jsonObjObj = jsonObjVal.toObject();
        if (!jsonObjObj["objectName"].isString())
            continue;
        objInfo.objectName = jsonObjObj["objectName"].toString();

        if (!jsonObjObj["color"].isString())
            continue;
        QColor color;
        color.setNamedColor(jsonObjObj["color"].toString());
        if (!color.isValid())
            continue;
        objInfo.objectColor = color;

        if (jsonObjObj["aspectRatio"].isDouble()){
            objInfo.aspectRatio = jsonObjObj["aspectRatio"].toDouble();
            objInfo.aspectRatioSet = true;
        } else {
            objInfo.aspectRatioSet = false;
        }

        ret.append(objInfo);
    }
    return ret;
}

QString JsonLabelingObjDef::formatDescription()
{
    return "JSON format for object definition";
}

QString JsonLabelingObjDef::formatExtension()
{
    return "*.json";
}

QString JsonLabelingObjDef::formatName()
{
    return "Json";
}

QString JsonLabelingObjDef::name()
{
    return "Json Object Format";
}

QString JsonLabelingObjDef::version()
{
    return JSON_OBJ_DEF_PLUGIN_VERSION;
}

QString JsonLabelingObjDef::copyright()
{
    return JSON_OBJ_DEF_PLUGIN_COPYRIGHT;
}

QString JsonLabelingObjDef::guid()
{
    return JSON_OBJ_DEF_PLUGIN_GUID;
}


