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
#include <QDir>
#include <QList>

bool JsonLabelForamtPlugin::save(const BoxManager &boxes, const QString &savePath)
{
    QFile file(savePath);

    if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
        return false;

    QJsonArray jsonImagesAry;
    auto keys = boxes.keys();
    foreach (QString key, keys) {
        LabelingBoxList boxList = boxes[key];

        QJsonObject jsonImageObj;

        QJsonArray jsonBoxes;
        for (int i = 0; i < boxList.size(); ++i) {
            LabelingBox box = boxList.at(i);

            QRectF rect = box.position;
            QJsonObject jsonRectObj;
            jsonRectObj.insert("top", rect.top());
            jsonRectObj.insert("left", rect.left());
            jsonRectObj.insert("width", rect.width());
            jsonRectObj.insert("height", rect.height());
            jsonRectObj.insert("object", box.objName);
            jsonRectObj.insert("color", box.color.name());
            if (box.aspectRatioSet)
                jsonRectObj.insert("aspectRatio", box.aspectRatio);

            jsonBoxes.append(jsonRectObj);
        }
        jsonImageObj.insert("boxes", jsonBoxes);
        jsonImageObj.insert("file", key);

        jsonImagesAry.append(jsonImageObj);
    }


    QJsonObject jsonRoot;
    jsonRoot.insert("images", jsonImagesAry);

    QJsonDocument jsonDoc(jsonRoot);
    QByteArray docByteArray = jsonDoc.toJson();

    file.write(docByteArray);
    file.close();
    return true;
}

bool JsonLabelForamtPlugin::open(const QString &openPath, BoxManager *boxman, const QString &imgPath)
{
    Q_UNUSED(imgPath);

    QFile f(openPath);
    if (!f.open(QIODevice::ReadOnly))
        return false;

    QByteArray jsonStr = f.readAll();

    BoxManager &man = *boxman;

    QJsonParseError err;
    auto jsonDoc = QJsonDocument::fromJson(jsonStr, &err);
    if (err.error != QJsonParseError::NoError || !jsonDoc.isObject())
        return false;

    auto jsonRootObject = jsonDoc.object();
    auto jsonImagesVal = jsonRootObject["images"];
    if (!jsonImagesVal.isArray())
        return false;
    auto jsonImagesArray = jsonImagesVal.toArray();

    foreach (QJsonValue jsonImgVal, jsonImagesArray) {
        if (!jsonImgVal.isObject())
            continue;

        QJsonObject jsonImgObj = jsonImgVal.toObject();
        QJsonValue jsonBoxesVal = jsonImgObj["boxes"];
        QJsonValue jsonImgFile = jsonImgObj["file"];
        if (!jsonImgFile.isString())
            continue;
        QString filename = jsonImgFile.toString();

        if (!jsonBoxesVal.isArray())
            continue;
        QJsonArray jsonBoxesArray = jsonBoxesVal.toArray();

        LabelingBoxList boxList;
        foreach (QJsonValue jsonBoxVal, jsonBoxesArray) {
            if (!jsonBoxVal.isObject())
                continue;

            LabelingBox box;
            QJsonObject jsonBoxObj = jsonBoxVal.toObject();

            double top, left, height, width;
            if (!jsonBoxObj["height"].isDouble())
                continue;
            height = jsonBoxObj["height"].toDouble();

            if (!jsonBoxObj["top"].isDouble())
                continue;
            top = jsonBoxObj["top"].toDouble();

            if (!jsonBoxObj["left"].isDouble())
                continue;
            left = jsonBoxObj["left"].toDouble();

            if (!jsonBoxObj["width"].isDouble())
                continue;
            width = jsonBoxObj["width"].toDouble();

            box.position = QRectF(left, top, width, height);

            if (!jsonBoxObj["object"].isString())
                continue;
            box.objName = jsonBoxObj["object"].toString();

            if (!jsonBoxObj["color"].isString())
                continue;
            box.color.setNamedColor(jsonBoxObj["color"].toString());
            if (!box.color.isValid())
                continue;

            if (!jsonBoxObj["aspectRatio"].isDouble())
                box.aspectRatioSet = false;
            else {
                box.aspectRatioSet = true;
                box.aspectRatio = jsonBoxObj["aspectRatio"].toDouble();
            }
            boxList.append(box);
        }

        man[filename] = boxList;
    }

    return !man.empty();
}

QString JsonLabelForamtPlugin::formatDescription()
{
    return "JSON format for annotation";
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
    return "Json Annotating Format";
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




