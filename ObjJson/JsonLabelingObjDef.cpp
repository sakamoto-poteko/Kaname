/**************************************************************************
* Copyright (c) 2016 Sakamoto Poteko <poteko@poteko.moe>
*
* Permission is hereby granted, free of charge, to any person obtaining
* a copy of this software and associated documentation files (the
* "Software"), to deal in the Software without restriction, including
* without limitation the rights to use, copy, modify, merge, publish,
* distribute, sublicense, and/or sell copies of the Software, and to
* permit persons to whom the Software is furnished to do so, subject to
* the following conditions:
*
* The above copyright notice and this permission notice shall be
* included in all copies or substantial portions of the Software.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
* EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
* MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
* NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
* LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
* OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
* WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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


