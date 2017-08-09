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

#ifndef LABELINGBOX_H
#define LABELINGBOX_H

#include <QColor>
#include <QRectF>

struct LabelingBox
{
    QSize   imageSize;
    QRectF  position;
    QColor  color;
    qreal   aspectRatio;
    bool    aspectRatioSet;
    QString objName;
};

#endif // LABELINGBOX_H
