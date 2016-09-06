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
