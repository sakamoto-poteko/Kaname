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

#include <QPainter>

#include "LabelingScene.h"
#include "GraphicsBoxItem.h"

LabelingScene::LabelingScene(const QImage &image, const QString &imgName, BoxManager *boxMan, QObject *parent):
    QGraphicsScene(parent), _imgName(imgName), _boxMan(boxMan)
{
    setBackgroundImage(image);
    syncFromBoxManager();
    connect(this, &LabelingScene::changed, this, &LabelingScene::syncToBoxManager);
}

LabelingScene::~LabelingScene()
{
}

QList<GraphicsBoxItem *> LabelingScene::boxItems()
{
    auto is = items();

    QList<GraphicsBoxItem *> bis;
    foreach (QGraphicsItem *gi, is) {
        GraphicsBoxItem *gbi = dynamic_cast<GraphicsBoxItem *>(gi);
        if (!gbi)
            continue;
        else
            bis.append(gbi);
    }

    qSort(bis.begin(), bis.end(), [](GraphicsBoxItem *a, GraphicsBoxItem *b) { return a->id() < b->id(); });
    return bis;
}

uint32_t LabelingScene::nextId()
{
    return ++_nextId;
}

void LabelingScene::syncToBoxManager()
{
    LabelingBoxList list;

    foreach (QGraphicsItem *item, items()) {
        GraphicsBoxItem *gi = static_cast<GraphicsBoxItem *>(item);
        list.append(GraphicsBoxItem::toLabelingBox(gi));
    }

    (*_boxMan)[imgName()] = list;
}

void LabelingScene::syncFromBoxManager()
{
    clear();
    auto list = (*_boxMan)[_imgName];
    foreach (auto lb, list) {
        auto *i = GraphicsBoxItem::fromLableingBox(lb, nextId());
        addItem(i);
    }
}

void LabelingScene::drawBackground(QPainter *painter, const QRectF &rect)
{
    if (_backgroundImage.isNull())
        return;

    painter->drawImage(rect, _backgroundImage, rect);
}

uint32_t LabelingScene::_nextId = 0;

