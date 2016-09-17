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
#include <QGraphicsScene>
#include <QGraphicsSceneHoverEvent>
#include <QCursor>

#include "GraphicsBoxItem.h"

#include <QDebug>

GraphicsBoxItem::GraphicsBoxItem(const QString &objectName, const QColor &color) :
    _width(0), _height(0), _color(color), _aspectRatio(0.),
    _aspectRatioSet(false), _selected(false),
    _objName(objectName)
{
}


QRectF GraphicsBoxItem::boundingRect() const
{
    int l = _width < 0 ? _width : 0;
    int t = _height < 0 ? _height : 0;
    int w = _width < 0 ? -_width : _width;
    int h = _height < 0 ? -_height : _height;

    return QRectF(l, t, w + 2, h + 2);
}

void GraphicsBoxItem::paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget)
{
    Q_UNUSED(option);
    Q_UNUSED(widget);
    QRect rect(0, 0, _width, _height);
    scene()->sceneRect();
    painter->setPen(QPen(_selected ? Qt::white : _color, 2));
    painter->drawRect(rect);
    QPoint center(rect.center());
    painter->setBrush(_selected ? Qt::white : _color);
    painter->drawEllipse(center, 2, 2);
}

GraphicsBoxItem *GraphicsBoxItem::clone()
{
    GraphicsBoxItem *cloned = new GraphicsBoxItem(_objName, _color);
    cloned->_height = _height;
    cloned->_width  = _width;
    cloned->_aspectRatio = _aspectRatio;
    cloned->_aspectRatioSet = _aspectRatioSet;
    cloned->_center = _center;
    return cloned;
}

void GraphicsBoxItem::setCenter(const QPointF &center)
{
    QPointF oldCenter = mapToScene(QRectF(0, 0, _width, _height).center());
    QPointF centerDiff = center - oldCenter;
    QPointF newPos = scenePos() + centerDiff;

    qreal sw = scene()->width();
    qreal sh = scene()->height();

    if (newPos.x() < 0)
        newPos.setX(0);
    if (newPos.y() < 0)
        newPos.setY(0);

    if (newPos.x() + _width > sw) {
        newPos.setX(sw - _width);
    }
    if (newPos.y() + _height > sh) {
        newPos.setY(sh - _height);
    }

    setPos(newPos);
}

void GraphicsBoxItem::setTopLeft(const QPointF &topLeft)
{
    auto newPos = topLeft;
    qreal sw = scene()->width();
    qreal sh = scene()->height();

    if (newPos.x() < 0)
        newPos.setX(0);
    if (newPos.y() < 0)
        newPos.setY(0);

    if (newPos.x() + _width > sw) {
        newPos.setX(sw - _width);
    }
    if (newPos.y() + _height > sh) {
        newPos.setY(sh - _height);
    }

    setPos(newPos);
}

void GraphicsBoxItem::growBox()
{
    auto center = _center;
    qreal sw = scene()->width();
    qreal sh = scene()->height();

    throw("not implemented");
}

void GraphicsBoxItem::shrinkBox()
{

}

void GraphicsBoxItem::growBorder(int orientation)
{
    auto center = _center;

    if (orientation) {
        if (_width + 2. > scene()->width())
            return;
        setSize(QPointF(_width + 2., _height));
    } else {
        if (_height + 2. > scene()->height())
            return;
        setSize(QPointF(_width, _height + 2.));
    }
//    setCenter(center);
}

void GraphicsBoxItem::shrinkBorder(int orientation)
{
    auto center = _center;

    if (orientation) {
        if (_width < 2.)
            return;
        setSize(QPointF(_width - 2., _height));
    } else {
        if (_height < 2.)
            return;
        setSize(QPointF(_width, _height - 2.));
    }
//    setCenter(center);
}

LabelingBox GraphicsBoxItem::toLabelingBox(const GraphicsBoxItem *item)
{
    LabelingBox b;
    b.aspectRatio = item->aspectRatio();
    b.aspectRatioSet = item->aspectRatioSet();
    b.color = item->color();
    b.objName = item->objName();
    b.position = item->rect();
    return b;
}

GraphicsBoxItem *GraphicsBoxItem::fromLableingBox(const LabelingBox &box)
{
    GraphicsBoxItem *item = new GraphicsBoxItem(box.objName, box.color);
    item->_aspectRatio = box.aspectRatio;
    item->_aspectRatioSet = box.aspectRatioSet;
    item->_color = box.color;
    item->_objName = box.objName;
    item->setPos(box.position.topLeft());
    item->setSize(QPointF(box.position.width(), box.position.height()));
    return item;
}

void GraphicsBoxItem::hoverMoveEvent(QGraphicsSceneHoverEvent *event)
{
//    if (centerOverlaps(event->pos())) {
//        setCursor(Qt::SizeAllCursor);
//        event->ignore();
//    } else {
//        setCursor(Qt::CrossCursor);
//        event->accept();
//    }
}

void GraphicsBoxItem::mousePressEvent(QGraphicsSceneMouseEvent *event)
{
}

void GraphicsBoxItem::mouseMoveEvent(QGraphicsSceneMouseEvent *event)
{
}

void GraphicsBoxItem::mouseReleaseEvent(QGraphicsSceneMouseEvent *event)
{
}

void GraphicsBoxItem::mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event)
{
}

const qreal GraphicsBoxItem::OVERLAP_THRESHOLD = 5.;




