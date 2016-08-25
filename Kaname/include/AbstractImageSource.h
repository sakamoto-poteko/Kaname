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

#ifndef ABSTRACTIMAGESOURCE_H
#define ABSTRACTIMAGESOURCE_H

#include <QObject>
#include <QImage>

class AbstractImageSource : public QObject
{
    Q_OBJECT
public:
    AbstractImageSource();
    virtual ~AbstractImageSource();
    virtual int  sourceCount() = 0;
    virtual QImage getImage() = 0;
    virtual QString getImageName() = 0;

public slots:
    virtual void addSource(QString source) = 0;
    virtual void addSources(QStringList sources) = 0;
    virtual void clearSources() = 0;
    virtual void load() = 0;
    virtual bool moveNext() = 0;
    virtual bool movePrevious() = 0;
    virtual bool moveBegin() = 0;
    virtual bool moveEnd() = 0;

signals:
    void sourceChanged(QStringList sources);
    void sourceStatusChanged(bool loaded);
};

#endif // ABSTRACTIMAGESOURCE_H
