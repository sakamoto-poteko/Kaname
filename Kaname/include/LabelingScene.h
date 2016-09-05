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

#ifndef LABELINGSCENE_H
#define LABELINGSCENE_H

#include <QGraphicsScene>

#include "BoxManager.h"

class LabelingScene : public QGraphicsScene
{
public:
    LabelingScene(const QImage &image, const QString &imgName, BoxManager *boxMan, QObject *parent = Q_NULLPTR);
    ~LabelingScene();

    void setBackgroundImage(const QImage &image)
    {
        _backgroundImage = image;
        setSceneRect(image.rect());
    }

    void clearBackgroundImage()
    {
        _backgroundImage = QImage();
        setSceneRect(QRectF());
    }

    QString imgName() const
    {
        return _imgName;
    }

    void setImgName(const QString &imgName)
    {
        _imgName = imgName;
    }

    BoxManager *boxMan() const
    {
        return _boxMan;
    }

    void setBoxMan(BoxManager *boxMan)
    {
        _boxMan = boxMan;
    }

public slots:
    void syncToBoxManager();
    void syncFromBoxManager();

protected:
    LabelingScene(QObject *parent = Q_NULLPTR) : QGraphicsScene(parent), _boxMan(0) { }

    virtual void drawBackground(QPainter *painter, const QRectF &rect);

    QImage _backgroundImage;
    QString _imgName;
    BoxManager *_boxMan;
};

#endif // LABELINGSCENE_H
