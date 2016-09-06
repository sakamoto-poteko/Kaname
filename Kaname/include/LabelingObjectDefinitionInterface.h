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
#ifndef LABELINGOBJECTDEFINITIONINTERFACE_H
#define LABELINGOBJECTDEFINITIONINTERFACE_H

#include "ObjectInfo.h"

class LabelingObjectDefinitionInterface
{
public:
    virtual ~LabelingObjectDefinitionInterface() {}

    virtual ObjectInfoList open(const QString &path) = 0;
    virtual QString formatDescription() = 0;
    virtual QString formatExtension() = 0;
    virtual QString formatName() = 0;
    virtual QString name() = 0;
    virtual QString version() = 0;
    virtual QString copyright() = 0;
    virtual QString guid() = 0;
};


#define ObjDefInterface_iid "moe.afa.utils.Kaneme.LabelingObjectDefinitionInterface"

Q_DECLARE_INTERFACE(LabelingObjectDefinitionInterface, ObjDefInterface_iid)

#endif // LABELINGOBJECTDEFINITIONINTERFACE_H
