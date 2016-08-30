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

#include "MarkingBoxManager.h"

void MarkingBoxManager::setBoxName(Hash128Result hash, const QString &filename)
{
    _markingBoxes[hash].first = filename;
}

QList<QRect> MarkingBoxManager::getBoxes(Hash128Result hash)
{
    return _markingBoxes[hash].second;
}

QList<QRect> *MarkingBoxManager::getBoxesRef(Hash128Result hash)
{
    return &_markingBoxes[hash].second;
}
