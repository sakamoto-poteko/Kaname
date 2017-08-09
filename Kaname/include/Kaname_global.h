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

#ifndef KANAME_GLOBAL_H
#define KANAME_GLOBAL_H

#define KANAME_VERSION "2.2.0"
#define KANAME_COPYRIGHT "Copyright (c) 2017, Afa.L Cheng <afa@afa.moe>"

#include <QHash>
#include <QUuid>

class LabelDataFormatInterface;
class LabelingObjectDefinitionInterface;

class KanamePlugins
{
public:
    // extension, object
    QHash<QUuid, LabelDataFormatInterface *> LabelDataFormatInterfaces;
    QHash<QUuid, LabelingObjectDefinitionInterface *> LabelObjectDefinitionInterfaces;
};

extern KanamePlugins __kanamePlugins;
#endif // KANAME_GLOBAL_H
