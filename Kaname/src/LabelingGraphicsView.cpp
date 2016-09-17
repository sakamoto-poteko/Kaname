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

#include <QMouseEvent>
#include <QTimeLine>
#include <QGLWidget>

#include "LabelingGraphicsView.h"

#include <QDebug>

LabelingGraphicsView::LabelingGraphicsView(QWidget *parent) :
    QGraphicsView(parent), _dragState(NotDragging),
    _currentBoxItem(0), _selectedBoxItem(0),
    _numScheduledScalings(0), _boxThreshold(50),
    _currentBoxObjName("unknown"), _currentBoxColor(Qt::red),
    _currentAspectRatioSet(false), _currentAspectRatio(0.)
{
    QGLFormat f;
    f.setSampleBuffers(true);
    f.setSamples(16);
    auto *w = new QGLWidget(f);
    setViewport(w);
    setAlignment(Qt::AlignLeft | Qt::AlignTop);
    setMouseTracking(true);
}

void LabelingGraphicsView::scaleUp2x()
{
    scale(2, 2);
}

void LabelingGraphicsView::scaleDown2x()
{
    scale(0.5, 0.5);
}

void LabelingGraphicsView::scaleImgSize()
{
    resetMatrix();
}

void LabelingGraphicsView::scaleFitWindow()
{
    fitInView(sceneRect(), Qt::KeepAspectRatio);
}

void LabelingGraphicsView::selectNextObject()
{
    // NOT IMPLEMENTED
}

void LabelingGraphicsView::clearSelection()
{
    if (_selectedBoxItem) {
        _selectedBoxItem->unselect();
        _selectedBoxItem = 0;
    }
}

void LabelingGraphicsView::moveSelectedBoxDown()
{
    if (!_selectedBoxItem)
        return;

    auto newPos = _selectedBoxItem->scenePos();
    newPos.setY(newPos.y() + 1.);
    _selectedBoxItem->setTopLeft(newPos);
}

void LabelingGraphicsView::moveSelectedBoxUp()
{
    if (!_selectedBoxItem)
        return;

    auto newPos = _selectedBoxItem->scenePos();
    newPos.setY(newPos.y() - 1.);
    _selectedBoxItem->setTopLeft(newPos);
}

void LabelingGraphicsView::moveSelectedBoxLeft()
{
    if (!_selectedBoxItem)
        return;

    auto newPos = _selectedBoxItem->scenePos();
    newPos.setX(newPos.x() - 1.);
    _selectedBoxItem->setTopLeft(newPos);
}

void LabelingGraphicsView::moveSelectedBoxRight()
{
    if (!_selectedBoxItem)
        return;

    auto newPos = _selectedBoxItem->scenePos();
    newPos.setX(newPos.x() + 1.);
    _selectedBoxItem->setTopLeft(newPos);
}

void LabelingGraphicsView::growSelectedBoxHorizentally()
{
    if (!_selectedBoxItem)
        return;

    _selectedBoxItem->growBorder(1);
}

void LabelingGraphicsView::growSelectedBoxVertically()
{
    if (!_selectedBoxItem)
        return;

    _selectedBoxItem->growBorder(0);
}

void LabelingGraphicsView::shrinkSelectedBoxHorizentally()
{
    if (!_selectedBoxItem)
        return;

    _selectedBoxItem->shrinkBorder(1);
}

void LabelingGraphicsView::shrinkSelectedBoxVertically()
{
    if (!_selectedBoxItem)
        return;

    _selectedBoxItem->shrinkBorder(0);
}

void LabelingGraphicsView::deleteSelected()
{
    if (!_selectedBoxItem || !scene())
        return;

    scene()->removeItem(_selectedBoxItem);
    delete _selectedBoxItem;
    _selectedBoxItem = 0;
}

void LabelingGraphicsView::setObjectInfo(const ObjectInfo &info)
{
    setCurrentBoxObjName(info.objectName);
    setCurrentBoxColor(info.objectColor);
    setCurrentAspectRatioSet(info.aspectRatioSet);
    setCurrentAspectRatio(info.aspectRatio);
}

void LabelingGraphicsView::mousePressEvent(QMouseEvent *event)
{
    if (!scene())
        return;

    QPointF mpos = mapToScene(event->pos());

    if (mpos.x() > sceneRect().width() || mpos.y() > sceneRect().height())
        return;

    GraphicsBoxItem *pressedItem = centerOverlappedBox(mpos);
    _dragOrigin = mpos;

    if (event->modifiers() == Qt::ControlModifier && event->button() == Qt::LeftButton) {
        if (pressedItem) {
            _dragState = CopyingBox;
            _currentBoxItem = pressedItem->clone();
            _currentBoxItem->setCenter(mpos);
            _currentBoxItem->select();
            scene()->addItem(_currentBoxItem);
            setCursor(Qt::DragCopyCursor);
            return;
        }
    }

    if (event->button() == Qt::LeftButton) {
        if (pressedItem) {
            _dragState = MovingBox;
            _currentBoxItem = pressedItem;
            _currentBoxItem->select();
            setCursor(Qt::SizeAllCursor);
            return;
        } else {
            _dragState = NewBox;
            _currentBoxItem = new GraphicsBoxItem(currentBoxObjName(), currentBoxColor());
            if (_currentAspectRatioSet)
                _currentBoxItem->setAspectRatio(_currentAspectRatio);
            _currentBoxItem->setPos(mpos);
            scene()->addItem(_currentBoxItem);
            setCursor(Qt::CrossCursor);
            return;
        }
    }
}

void LabelingGraphicsView::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    if (!scene())
        return;

    if (_currentBoxItem) {
        _currentBoxItem->normalize();
        if ((_currentBoxItem->width() < _boxThreshold ||
                _currentBoxItem->height() < _boxThreshold) &&
                event->modifiers() != Qt::ShiftModifier) {  // Smaller than threshold
            scene()->removeItem(_currentBoxItem);
            delete _currentBoxItem;
        } else {
            if (_currentBoxItem != _selectedBoxItem) {   // Not current selection
//                _currentBoxItem->unselect();
                if (_currentBoxItem) {
                    if (_selectedBoxItem)
                        _selectedBoxItem->unselect();
                    _selectedBoxItem = _currentBoxItem;
                    _selectedBoxItem->select();
                }
            } else {
            }
        }

        _currentBoxItem = 0;
    }
    _dragState = NotDragging;
}

void LabelingGraphicsView::mouseDoubleClickEvent(QMouseEvent *event)
{
    QPointF mpos = mapToScene(event->pos());

    auto *box = centerOverlappedBox(mpos);
    if (!box) {
        return;
    }

    if (_selectedBoxItem)
        _selectedBoxItem->unselect();

    box->select();
    _selectedBoxItem = box;
}

void LabelingGraphicsView::mouseMoveEvent(QMouseEvent *event)
{
    QPointF mpos = mapToScene(event->pos());
    switch (_dragState) {
    case NewBox:
        setCursor(Qt::CrossCursor);
        if (mpos.x() > sceneRect().right())
            mpos.setX(sceneRect().right());
        if (mpos.y() > sceneRect().bottom())
            mpos.setY(sceneRect().bottom());
        _currentBoxItem->setSize(mpos - _dragOrigin);
        break;
    case MovingBox:
        setCursor(Qt::SizeAllCursor);
        _currentBoxItem->setCenter(mpos);
        break;
    case CopyingBox:
        setCursor(Qt::DragCopyCursor);
        _currentBoxItem->setCenter(mpos);
        break;
    case NotDragging:
    default: {
        GraphicsBoxItem *i = centerOverlappedBox(mpos);
        if (i) {
            setCursor(Qt::SizeAllCursor);
        } else
            setCursor(Qt::CrossCursor);
        break;
    }
    }
    QGraphicsView::mouseMoveEvent(event);
}

GraphicsBoxItem *LabelingGraphicsView::centerOverlappedBox(const QPointF &point)
{
    if (!scene())
        return 0;

    auto itemlist = scene()->items();
    foreach (QGraphicsItem *item, itemlist) {
        // We're pretty sure all items are GraphicsBoxItem
        auto *i = static_cast<GraphicsBoxItem *>(item);
        if (i->centerOverlaps(i->mapFromScene(point)))
            return i;
    }

    return 0;
}

void LabelingGraphicsView::scalingTime(qreal)
{
    qreal factor = 1.0 + qreal(_numScheduledScalings) / 300.0;
    scale(factor, factor);
}

void LabelingGraphicsView::animFinished()
{
    if (_numScheduledScalings > 0)
        _numScheduledScalings--;
    else
        _numScheduledScalings++;
    sender()->deleteLater();
}

void LabelingGraphicsView::wheelEvent(QWheelEvent *event)
{
    if (event->modifiers() == Qt::ControlModifier) {
        int numDegrees = event->delta() / 8;
        int numSteps = numDegrees / 15; // see QWheelEvent documentation
        _numScheduledScalings += numSteps;
        if (_numScheduledScalings * numSteps < 0) // if user moved the wheel in another direction, we reset previously scheduled scalings
            _numScheduledScalings = numSteps;

        QTimeLine *anim = new QTimeLine(350, this);
        anim->setUpdateInterval(20);

        connect(anim, &QTimeLine::valueChanged, this, &LabelingGraphicsView::scalingTime);
        connect(anim, &QTimeLine::finished, this, &LabelingGraphicsView::animFinished);
        anim->start();
    } else {
        QGraphicsView::wheelEvent(event);
    }
}


