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


#include "ImageSourceCacheMgr.h"

ImageSourceCacheMgr::ImageSourceCacheMgr(ssize_t cacheSize) :
    _cacheSize(cacheSize)
{
}

/**
 * @brief ImageSourceCacheMgr::getImage
 * @param key
 * Returns the cached image. If key was not found in cache, return a null QImage.
 * @return
 */
QImage ImageSourceCacheMgr::getImage(const QString &key)
{
    return _imageCache[key];
}

void ImageSourceCacheMgr::enqueue(const QImage &image, const QString &key)
{
    if (_keys.size() == _cacheSize) {
        dequeue();
    }

    _keys.enqueue(key);
    _imageCache[key] = image;
}

void ImageSourceCacheMgr::dequeue()
{
    if (!_imageCache.isEmpty()) {
        auto key = _keys.dequeue();
        _imageCache.remove(key);
    }
}

void ImageSourceCacheMgr::clear()
{
    _keys.clear();
    _imageCache.clear();
}
