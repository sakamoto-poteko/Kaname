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

#ifndef GRAPHICSBOXITEM_H
#define GRAPHICSBOXITEM_H

#include <QGraphicsItem>

#include "LabelingBox.h"

class GraphicsBoxItem : public QGraphicsItem
{
public:
    GraphicsBoxItem(const QString &objectName, const QColor &color);

    virtual QRectF boundingRect() const;
    virtual void paint(QPainter *painter, const QStyleOptionGraphicsItem *option, QWidget *widget);
    virtual GraphicsBoxItem *clone();
    void setCenter(const QPointF &center);
    void setTopLeft(const QPointF &topLeft);

    void normalize()
    {
        prepareGeometryChange();
        auto spos = scenePos();
        QRectF cr(spos.x(), spos.y(), _width, _height);
        QRectF nr = cr.normalized();
        setPos(nr.topLeft());
        _width = nr.width();
        _height = nr.height();
    }

    void setSize(const QPointF &size)
    {
        prepareGeometryChange();

        if (aspectRatioSet()) {
            qreal aspWidth = size.x() * aspectRatio();
            _height = size.x();
            _width = aspWidth;
        } else {
            _width = size.x();
            _height = size.y();
        }
        _center = QRectF(0, 0, _width, _height).center();
    }

    QPointF size()
    {
        return QPointF(_width, _height);
    }

    qreal width() const
    {
        return _width;
    }

    qreal height() const
    {
        return _height;
    }

    QColor color() const
    {
        return _color;
    }

    void setColor(const QColor &color)
    {
        prepareGeometryChange();
        _color = color;
    }

    void unsetAspectRatio()
    {
        _aspectRatioSet = false;
    }

    bool aspectRatioSet() const
    {
        return _aspectRatioSet;
    }

    qreal aspectRatio() const
    {
        return _aspectRatio;
    }

    void setAspectRatio(qreal aspectRatio)
    {
        _aspectRatio = aspectRatio;
        _aspectRatioSet = true;
    }

    ///
    /// \brief centerOverlaps
    /// \param point coord in item coord system
    /// \return
    ///
    bool centerOverlaps(const QPointF &point)
    {
        return (point - _center).manhattanLength() < OVERLAP_THRESHOLD;
    }

    void select()
    {
        _selected = true;
        setZValue(100);
        setAcceptHoverEvents(true);
    }

    void unselect()
    {
        _selected = false;
        setZValue(0);
        setAcceptHoverEvents(false);
    }

    QRectF rect() const
    {
        return QRectF(scenePos(), QSizeF(_width, _height));
    }

    QString objName() const
    {
        return _objName;
    }

    void setObjName(const QString &objName)
    {
        _objName = objName;
    }

    static LabelingBox toLabelingBox(const GraphicsBoxItem *item);
    static GraphicsBoxItem *fromLableingBox(const LabelingBox &box);

protected:
    virtual void hoverMoveEvent(QGraphicsSceneHoverEvent *event);
    virtual void mousePressEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseMoveEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseReleaseEvent(QGraphicsSceneMouseEvent *event);
    virtual void mouseDoubleClickEvent(QGraphicsSceneMouseEvent *event);

    qreal _width;
    qreal _height;
    QColor _color;
    qreal _aspectRatio;
    bool _aspectRatioSet;
    QPointF _center;
    bool _selected;
    QString _objName;

    static const qreal OVERLAP_THRESHOLD;

private:
    GraphicsBoxItem() : _width(0), _height(0), _aspectRatioSet(false), _selected(false) {}
};

#endif // GRAPHICSBOXITEM_H
