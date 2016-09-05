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

#include <QPalette>
#include <QColor>

#include "ObjectSelectionButton.h"

ObjectSelectionButton::ObjectSelectionButton(const ObjectInfo &objInfo, QWidget *parent) :
    QPushButton(parent), _objInfo(objInfo)
{
    QColor conColor = getContrastColor(_objInfo.objectColor);
    setStyleSheet(QString("border:0px;background-color:rgb(%1,%2,%3);color:rgb(%4,%5,%6);").
                  arg(_objInfo.objectColor.red()).arg(_objInfo.objectColor.green()).arg(_objInfo.objectColor.blue()).
                  arg(conColor.red()).arg(conColor.green()).arg(conColor.blue()));

    setText(QString("%1, %2").arg(_objInfo.objectName).
            arg(_objInfo.aspectRatioSet ? QString::number(_objInfo.aspectRatio) : tr("N/A")));
    connect(this, &ObjectSelectionButton::clicked, [this](){ emit selected(_objInfo); });
}

