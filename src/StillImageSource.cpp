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


#include "StillImageSource.h"

StillImageSource::StillImageSource() :
    _loaded(false)
{
}

void StillImageSource::AddSource(QString source)
{
    if (!_loaded)
        _sourcePathList << source;
}

void StillImageSource::AddSources(QStringList sources)
{
    if (!_loaded)
        _sourcePathList << sources;
}

void StillImageSource::ClearSources()
{
    if (!_loaded)
        _sourcePathList.clear();
}

/**
 * @brief StillImageSource::load
 *
 * This function does not actually load. It just `freezes` the source list.
 */
void StillImageSource::load()
{
    _loaded = true;
    _sourcePathListCurrentPos = _sourcePathList.constBegin();
}

/**
 * @brief StillImageSource::moveNext
 * @note It will never move beyond the end
 *
 * @return false if at the end, true otherwise
 */
bool StillImageSource::moveNext()
{
    if (_sourcePathList.isEmpty())
        return false;

    if (_sourcePathListCurrentPos == _sourcePathList.constEnd())
        return false;

    _sourcePathListCurrentPos++;
    return true;
}

/**
 * @brief StillImageSource::movePrevious
 * @return false if at the head
 */
bool StillImageSource::movePrevious()
{
    if (_sourcePathList.isEmpty())
        return false;

    if (_sourcePathListCurrentPos == _sourcePathList.constBegin())
        return false;

    _sourcePathListCurrentPos--;
    return true;
}

bool StillImageSource::moveBegin()
{
    if (_sourcePathList.isEmpty())
        return false;

    _sourcePathListCurrentPos = _sourcePathList.constBegin();
    return true;
}

bool StillImageSource::moveEnd()
{
    if (_sourcePathList.isEmpty())
        return false;

    _sourcePathListCurrentPos = _sourcePathList.constEnd();
    _sourcePathListCurrentPos--;
    return true;
}

/**
 * @brief StillImageSource::getImage
 *
 * getImage() looks up cache table first. If not found in cache,
 * it loads the image file.
 *
 * @return Null QImage if not valid, otherwise loaded QImage
 */
QImage StillImageSource::getImage()
{
    QString str(*_sourcePathListCurrentPos);

    QImage img = _cacheMgr.getImage(str);
    if (!img.isNull())
        return img;

    img.load(str);
    if (!img.isNull())
        _cacheMgr.enqueue(img, str);

    return img;
}

