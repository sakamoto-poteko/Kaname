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

#include <tuple>

#include <QHash>
#include <QVector>
#include "LabelingBox.h"

typedef QList<LabelingBox> LabelingBoxList;
typedef std::tuple<QString, LabelingBoxList, QSize> Filename_LabelingBoxListImage_ImageSizeTuple;

class BoxManager : public QVector<Filename_LabelingBoxListImage_ImageSizeTuple>
{
public:
    QList<QString> keys() const
    {
        QList<QString> ret;
        for (auto pair : *this) {
            ret.append(std::get<0>(pair));
        }

        return ret;
    }

    LabelingBoxList &operator[](const QString &key)
    {
        for (int i = 0; i < size(); ++i) {
            Filename_LabelingBoxListImage_ImageSizeTuple &pair = QVector<Filename_LabelingBoxListImage_ImageSizeTuple>::operator[](i);
            if (std::get<0>(pair) == key) {
                return std::get<1>(pair);
            }
        }

        append(Filename_LabelingBoxListImage_ImageSizeTuple(key, LabelingBoxList(), QSize()));
        return std::get<1>(last());
    }

    LabelingBoxList const& operator[](const QString &key) const
    {
        for (int i = 0; i < size(); ++i) {
            const Filename_LabelingBoxListImage_ImageSizeTuple &pair = QVector<Filename_LabelingBoxListImage_ImageSizeTuple>::operator[](i);
            if (std::get<0>(pair) == key) {
                return std::get<1>(pair);
            }
        }

        throw;
    }

    void setImageSize(const QString &key, const QSize &imgSize)
    {
        for (int i = 0; i < size(); ++i) {
            Filename_LabelingBoxListImage_ImageSizeTuple &pair = QVector<Filename_LabelingBoxListImage_ImageSizeTuple>::operator[](i);
            auto pairKey = std::get<0>(pair);
            if (pairKey == key) {
                std::get<2>(pair) = imgSize;
                return;
            }
        }

        throw;
    }


    const QSize &getImageSize(const QString &key) const
    {
        for (int i = 0; i < size(); ++i) {
            const Filename_LabelingBoxListImage_ImageSizeTuple &pair = QVector<Filename_LabelingBoxListImage_ImageSizeTuple>::operator[](i);
            if (std::get<0>(pair) == key) {
                return std::get<2>(pair);
            }
        }

        throw;
    }

private:
};


#endif // MARKINGBOXMANAGER_H
