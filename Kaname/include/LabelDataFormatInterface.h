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

#ifndef LABELINTERFACE_H
#define LABELINTERFACE_H

#include <QHash>
#include <QVector>
#include <QRect>
#include <QList>
#include "Hash128Result.h"

class LabelDataFormatInterface
{
public:
    virtual ~LabelDataFormatInterface() { }
    virtual bool save(const QHash<Hash128Result, QPair<QString, QList<QRect>>> &boxes,
                      const QString &savePath,
                      const QVector<QString> &objnames) = 0;
    virtual QHash<Hash128Result, QPair<QString, QList<QRect>>> open(const QString &openPath) = 0;
    virtual QString formatDescription() = 0;
    virtual QString formatExtension() = 0;
    virtual QString formatName() = 0;
    virtual QString name() = 0;
    virtual QString version() = 0;
    virtual QString copyright() = 0;
    virtual QString guid() = 0;
};


#define LabelInterface_iid "moe.afa.utils.Kaneme.LabelDataFormatInterface"

Q_DECLARE_INTERFACE(LabelDataFormatInterface, LabelInterface_iid)

#endif // LABELINTERFACE_H
