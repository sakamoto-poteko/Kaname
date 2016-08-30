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

#ifndef MARKINGBOXMANAGER_H
#define MARKINGBOXMANAGER_H

#include <QHash>
#include <QRect>

#include "Hash128Result.h"

class MarkingBoxManager
{
public:
    MarkingBoxManager() {}

    void setBoxName(Hash128Result hash, const QString &filename)
    {
        _markingBoxes[hash].first = filename;
    }

    QList<QRect> getBoxes(Hash128Result hash)
    {
        return _markingBoxes[hash].second;
    }

    QList<QRect> *getBoxesRef(Hash128Result hash)
    {
        return &_markingBoxes[hash].second;
    }

    QHash<Hash128Result, QPair<QString, QList<QRect>>> getAllBoxes() { return _markingBoxes; }

    void clear() { _markingBoxes.clear(); }

    QHash<Hash128Result, QPair<QString, QList<QRect>>> getMarkingBoxes() const
    {
        return _markingBoxes;
    }

    void setMarkingBoxes(const QHash<Hash128Result, QPair<QString, QList<QRect>>> &markingBoxes)
    {
        _markingBoxes = markingBoxes;
    }

private:
    QHash<Hash128Result, QPair<QString, QList<QRect>>> _markingBoxes;
};

#endif // MARKINGBOXMANAGER_H
