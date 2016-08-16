/*************************************************************************
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
 ************************************************************************/

#ifndef STILLIMAGESOURCE_H
#define STILLIMAGESOURCE_H

#include "AbstractImageSource.h"
#include "ImageSourceCacheMgr.h"

class StillImageSource : public AbstractImageSource
{
public:
    StillImageSource();

    virtual void AddSource(QString source);
    virtual void AddSources(QStringList sources);
    virtual void ClearSources();

    virtual void load();
    virtual bool moveNext();
    virtual bool movePrevious();
    virtual bool moveBegin();
    virtual bool moveEnd();
    virtual QImage getImage();

protected:
    bool _loaded;
    QStringList _sourcePathList;
    QStringList::ConstIterator _sourcePathListCurrentPos;
    ImageSourceCacheMgr _cacheMgr;
};

#endif // STILLIMAGESOURCE_H
