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
    return false;
}




//QHash<Hash128Result, QPair<QString, QList<QRect>>> JsonLabelForamtPlugin::open(const QString &openPath,
//                                                                               QVector<QString> *objectNames, const QString &imgPath)
//{
//    QHash<Hash128Result, QPair<QString, QList<QRect>>> ret;
//    QVector<QString> objNames;

//    QDir imgdir = imgPath.isEmpty() ? QFileInfo(openPath).absoluteDir() : imgPath;

//    QFile f(openPath);
//    if (!f.open(QIODevice::ReadOnly))
//        return ret;

//    QByteArray jsonStr = f.readAll();

//    QJsonParseError err;
//    auto jsonDoc = QJsonDocument::fromJson(jsonStr, &err);
//    if (err.error != QJsonParseError::NoError || !jsonDoc.isObject())
//        return ret;

//    auto jsonRootObject = jsonDoc.object();
//    auto jsonImagesVal = jsonRootObject["images"];
//    if (!jsonImagesVal.isArray())
//        return ret;
//    auto jsonImagesArray = jsonImagesVal.toArray();

//    auto jsonObjectNamesVal = jsonRootObject["objectNames"];
//    if (!jsonObjectNamesVal.isArray())
//        return ret;
//    auto jsonObjectNamesArray = jsonObjectNamesVal.toArray();

//    foreach (QJsonValue jsonImgVal, jsonImagesArray) {
//        if (!jsonImgVal.isObject())
//            continue;

//        QJsonObject jsonImgObj = jsonImgVal.toObject();
//        QJsonValue jsonBoxesVal = jsonImgObj["boxes"];
//        if (!jsonBoxesVal.isArray())
//            continue;
//        QJsonArray jsonBoxesArray = jsonBoxesVal.toArray();

//        QJsonValue jsonFilenameVal = jsonImgObj["file"];
//        if (!jsonFilenameVal.isString())
//            continue;
//        QString filename = jsonFilenameVal.toString();

//        QJsonValue jsonHashVal = jsonImgObj["hash"];
//        if (!jsonHashVal.isString())
//            continue;
//        Hash128Result hash(jsonHashVal.toString());

//        QPair<QString, QList<QRect>> imgInfoPair;
//        imgInfoPair.first = imgdir.filePath(filename);

//        foreach (QJsonValue jsonBoxVal, jsonBoxesArray) {
//            if (!jsonBoxVal.isObject())
//                continue;

//            int left, top, height, width;
//            QJsonObject jsonBoxObj = jsonBoxVal.toObject();

//            if (!jsonBoxObj["height"].isDouble())
//                continue;
//            height = jsonBoxObj["height"].toInt();

//            if (!jsonBoxObj["top"].isDouble())
//                continue;
//            top = jsonBoxObj["top"].toInt();

//            if (!jsonBoxObj["left"].isDouble())
//                continue;
//            left = jsonBoxObj["left"].toInt();

//            if (!jsonBoxObj["width"].isDouble())
//                continue;
//            width = jsonBoxObj["width"].toInt();

//            imgInfoPair.second.append(QRect(left, top, width, height));
//        }

//        ret[hash] = imgInfoPair;
//    }

//    for (QJsonValue jsonObjName : jsonObjectNamesArray) {
//        if (!jsonObjName.isString()) {
//            objNames.clear();
//            break;
//        }

//        objNames.append(jsonObjName.toString());
//    }

//    if (objectNames)
//        *objectNames = objNames;

//    return ret;
//}

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




