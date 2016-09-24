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

#ifndef JSONLABELINGOBJDEF_H
#define JSONLABELINGOBJDEF_H

#define JSON_OBJ_DEF_PLUGIN_GUID "{CFF8A040-3319-4185-A043-BB2D56476A6A}"
#define JSON_OBJ_DEF_PLUGIN_VERSION "1.0.0"
#define JSON_OBJ_DEF_PLUGIN_COPYRIGHT "Copyright (c) 2016, Afa.L Cheng <afa@afa.moe>"

#include <QObject>

#include "LabelingObjectDefinitionInterface.h"

class JsonLabelingObjDef : public QObject, LabelingObjectDefinitionInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "moe.afa.utils.Kaneme.LabelObjectDefinitionInterface" FILE "ObjJson.json")
    Q_INTERFACES(LabelingObjectDefinitionInterface)

public:
    ObjectInfoList open(const QString &path) Q_DECL_OVERRIDE;
    QString formatDescription() Q_DECL_OVERRIDE;
    QString formatExtension() Q_DECL_OVERRIDE;
    QString formatName() Q_DECL_OVERRIDE;
    QString name() Q_DECL_OVERRIDE;
    QString version() Q_DECL_OVERRIDE;
    QString copyright() Q_DECL_OVERRIDE;
    QString guid() Q_DECL_OVERRIDE;

};

#endif // JSONLABELINGOBJDEF_H
