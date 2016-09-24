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
