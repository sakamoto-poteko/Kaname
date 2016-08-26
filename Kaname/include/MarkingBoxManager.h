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

#include <QVector>
#include <QMap>
#include <QRect>

class MarkingBoxManager
{
public:
    MarkingBoxManager();

    void setBoxName(qint64 hash, const QString &filename);
    QVector<QRect> getBoxes(qint64 hash);
    QVector<QRect> *getBoxesRef(qint64 hash);
    QMap<qint64, QPair<QString, QVector<QRect>>> getAllBoxes() { return _markingBoxes; }
    void clear() { _markingBoxes.clear(); }

private:
    QMap<qint64, QPair<QString, QVector<QRect>>> _markingBoxes;
};

#endif // MARKINGBOXMANAGER_H
