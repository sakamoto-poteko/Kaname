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
#include <QVector>
#include "LabelingBox.h"

typedef QList<LabelingBox> LabelingBoxList;

class BoxManager : public QVector<QPair<QString, LabelingBoxList>>
{
public:
    QList<QString> keys() const
    {
        QList<QString> ret;
        for (auto pair : *this) {
            ret.append(pair.first);
        }

        return ret;
    }

    LabelingBoxList &operator[](const QString &key)
    {
        for (int i = 0; i < size(); ++i) {
            QPair<QString, LabelingBoxList> &pair = QVector<QPair<QString, LabelingBoxList>>::operator[](i);
            if (pair.first == key) {
                return pair.second;
            }
        }

        append(QPair<QString, LabelingBoxList>(key, LabelingBoxList()));
        return last().second;
    }

    LabelingBoxList const& operator[](const QString &key) const
    {
        for (int i = 0; i < size(); ++i) {
            const QPair<QString, LabelingBoxList> &pair = QVector<QPair<QString, LabelingBoxList>>::operator[](i);
            if (pair.first == key) {
                return pair.second;
            }
        }

        throw;
    }

private:
};


#endif // MARKINGBOXMANAGER_H
