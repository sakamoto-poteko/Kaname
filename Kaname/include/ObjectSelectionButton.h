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

#ifndef OBJECTSELECTIONBUTTON_H
#define OBJECTSELECTIONBUTTON_H

#include <QPushButton>

#include "ObjectInfo.h"

class ObjectSelectionButton : public QPushButton
{
Q_OBJECT
public:
    ObjectSelectionButton(const ObjectInfo &objInfo, QWidget *parent = Q_NULLPTR);

    static QColor getContrastColor(const QColor &background)
    {
        qreal gray = background.red() * 0.299 + background.green() * 0.587 + background.blue() * 0.114;
        return gray > 130 ? Qt::black : Qt::white;
    }

    ObjectInfo getObjInfo() const { return _objInfo; }

signals:
    void selected(ObjectInfo objInfo);

private:
    ObjectInfo  _objInfo;


};

#endif // OBJECTSELECTIONBUTTON_H
