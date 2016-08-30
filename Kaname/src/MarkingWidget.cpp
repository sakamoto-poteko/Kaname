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

#include <QRubberBand>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QResizeEvent>
#include <QCryptographicHash>
#include <QDebug>

#include "MarkingWidget.h"
#include "MarkingBoxManager.h"

MarkingWidget::MarkingWidget(QWidget *parent) : QFrame(parent),
  _draggingNewBox(false), _draggingCurrentBox(false),
  _actualBoxes(0), _actualToScaledRatio(0.), _isZooming(false),
  _boxmgr(0), _sizeControlWidget(0),
  _currentSelection(-1), _currentSelectionOld(-1), _currentHover(-1),
  _falseTriggerThresholdWidth(10), _falseTriggerThresholdHeight(10),
  _pointsOverlapThreshold(4)
{
    setCursor(Qt::CrossCursor);
    setMouseTracking(true);
    clear();
}

void MarkingWidget::setImage(const QImage &image, const QString &filename)
{
    if (!_boxmgr)
        return;

    clear();

    _draggingNewBox = false;

    _image = image;
    scaleToSizeControl();
    Hash128Result hash = imageHash(_image);
    _boxmgr->setBoxName(hash, filename);
    _actualBoxes = _boxmgr->getBoxesRef(hash);

    recaculateDisplayBoxes();
    update();

    emit boxesUpdated(*_actualBoxes);
}

void MarkingWidget::setMarkingBoxManager(MarkingBoxManager *mgr)
{
    _boxmgr = mgr;
//    auto bx = mgr->getAllBoxes();
}

void MarkingWidget::undo()
{
    if (!_actualBoxes || _actualBoxes->empty())
        return;

    _actualBoxes->removeLast();
    _displayBoxes.removeLast();
    update();
    emit boxesUpdated(*_actualBoxes);
}

void MarkingWidget::skip()
{
    if (!_actualBoxes)
        return;
    _actualBoxes->append(QRect());
    _displayBoxes.append(QRect());
    emit boxesUpdated(*_actualBoxes);
}

void MarkingWidget::clear()
{
    setCursor(Qt::ArrowCursor);
    _currentSelection = -1;
    _currentSelectionOld = -1;
    _currentHover = -1;
    _actualBoxes = 0;
    _displayBoxes.clear();
    _scaledImage = QImage();
    _image = QImage();
    _isZooming = false;

    update();
}

void MarkingWidget::selectNextBox()
{
    if (_currentSelection++ >= _actualBoxes->size()) {
        _currentSelection = -1;
    }
    update();
}

void MarkingWidget::clearBoxSelection()
{
    _currentSelection = -1;
    update();
}

void MarkingWidget::moveSelectedBoxLeft(int pixel)
{
    if (_currentSelection < 0 || _currentSelection >= _actualBoxes->size())
        return;

    QPoint center = _displayBoxes.at(_currentSelection).center();
    center.setX(center.x() - pixel);
    tryMoveBox(_currentSelection, center);
    update();
}

void MarkingWidget::moveSelectedBoxRight(int pixel)
{
    if (_currentSelection < 0 || _currentSelection >= _actualBoxes->size())
        return;

    QPoint center = _displayBoxes.at(_currentSelection).center();
    center.setX(center.x() + pixel);
    tryMoveBox(_currentSelection, center);
    update();
}

void MarkingWidget::moveSelectedBoxUp(int pixel)
{
    if (_currentSelection < 0 || _currentSelection >= _actualBoxes->size())
        return;

    QPoint center = _displayBoxes.at(_currentSelection).center();
    center.setY(center.y() - pixel);
    tryMoveBox(_currentSelection, center);
    update();
}

void MarkingWidget::moveSelectedBoxDown(int pixel)
{
    if (_currentSelection < 0 || _currentSelection >= _actualBoxes->size())
        return;

    QPoint center = _displayBoxes.at(_currentSelection).center();
    center.setY(center.y() + pixel);
    tryMoveBox(_currentSelection, center);
    update();
}

void MarkingWidget::enlarge2x()
{
    if (_scaledImage.isNull() || _image.isNull())
        return;
    _isZooming = true;
    QSize size = _scaledImage.size() * 2;
    _scaledImage = _image.scaled(size, Qt::KeepAspectRatio, Qt::FastTransformation);
    _actualToScaledRatio = _image.width() / static_cast<double>(size.width());
    recaculateDisplayBoxes();
//    repaint();

    setFixedSize(_scaledImage.size());
    update();
}

void MarkingWidget::shrink2x()
{
    if (_scaledImage.isNull() || _image.isNull())
        return;
    _isZooming = true;
    QSize size = _scaledImage.size() / 2;
    _scaledImage = _image.scaled(size, Qt::KeepAspectRatio, Qt::FastTransformation);
    _actualToScaledRatio = _image.width() / static_cast<double>(size.width());
    recaculateDisplayBoxes();
//    repaint();

    setFixedSize(_scaledImage.size());
    update();
}

void MarkingWidget::mouseDoubleClickEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        _currentSelection = getBoxIndex(e->pos());
    }
}

void MarkingWidget::mousePressEvent(QMouseEvent *e)
{
    if (e->button() == Qt::LeftButton) {
        auto pos = e->pos();
        int w = _scaledImage.width();
        int h = _scaledImage.height();

        if (pos.x() < 0 || pos.y() < 0 ||  pos.x() > w || pos.y() > h) {
            return;
        }

        if (_currentHover != -1 && pointsOverlap(pos, _displayBoxes.at(_currentHover).center())) {
            // mouse clicked when on center
            _currentSelectionOld = _currentSelection;
            _currentSelection = _currentHover;
            _draggingCurrentBox = true;
            return;
        } else {
            // else
            _dragOrigin = pos;
            _draggingNewBox = true;
            _dragBox = QRect(_dragOrigin, QSize());

            return;
        }
    }
}

void MarkingWidget::mouseMoveEvent(QMouseEvent *e)
{
    if (_draggingNewBox && _draggingCurrentBox)
        qFatal("Dragging current & new occured at the same time. Bug!");

    // dragging for creating new box
    if (_draggingNewBox /*&& keypressed */) {
        auto pos = e->pos();
        const int w = _scaledImage.width();
        const int h = _scaledImage.height();

        if (pos.x() < 0)
            pos.setX(0);
        if (pos.y() < 0)
            pos.setY(0);

        if (pos.x() > w)
            pos.setX(w - 2);
        if (pos.y() > h)
            pos.setY(h - 2);

        _dragBox = QRect(_dragOrigin, pos);
        update();

        emit mouseDraggingNewBox(calculateActualPoint(_dragOrigin), calculateActualPoint(pos));
        return;
    }

    // moving for creating new box
    if (_draggingCurrentBox /* && keypressed */) {
        tryMoveBox(_currentSelection, e->pos());
        update();
        emit mouseMovingBox(calculateActualPoint(_actualBoxes->at(_currentSelection).center()),
                            calculateActualPoint(e->pos()));
        return;
    }

    // nothing moving, marking hover box
    _currentHover = getBoxIndex(e->pos());
    if (_currentHover != -1) {
        setCursor(Qt::SizeAllCursor);
        update();
    } else {
        setCursor(Qt::CrossCursor); // Set back to cross
        update();
    }
}

void MarkingWidget::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);

    if (!_actualBoxes)
        return;

    if (_draggingNewBox) {
        _draggingNewBox = false;

        QRect normalizedBox = _dragBox.normalized();
        if (normalizedBox.width() < 5 || normalizedBox.height() < 5)    // Too small
            return;

        _displayBoxes.append(normalizedBox);
        _actualBoxes->append(calculateActualBox(normalizedBox));
    }

    if (_draggingCurrentBox) {
        _draggingCurrentBox = false;
        _currentSelection = _currentSelectionOld;
        _currentSelectionOld = -1;
    }

    update();
    emit boxesUpdated(*_actualBoxes);
}

void MarkingWidget::paintEvent(QPaintEvent *e)
{
    QFrame::paintEvent(e);
    QPainter painter(this);

    if (!_scaledImage.isNull()) {
        painter.drawImage(0, 0, _scaledImage);
    }

    if (_draggingNewBox) {
        painter.setPen(COLOR_TABLE[_displayBoxes.size() % COLOR_TABLE_SIZE]);
        painter.drawRect(_dragBox);
    }

    for(int i = 0; i < _displayBoxes.size(); ++i) {
        if (_currentHover != i && _currentSelection != i)
            painter.setPen(QPen(COLOR_TABLE[i % COLOR_TABLE_SIZE], 2));
        else
            painter.setPen(QPen(Qt::white, 2));

        QRect rect = _displayBoxes.at(i);
        painter.drawRect(rect);
        painter.drawEllipse(QRect(rect.center().x() - 1, rect.center().y() - 1, 2, 2));
    }
}

void MarkingWidget::resizeEvent(QResizeEvent *e)
{
    if (_isZooming)
        return;

    _scaledImage = _image.scaled(e->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    _actualToScaledRatio = (double)_image.width() / _scaledImage.width();
    recaculateDisplayBoxes();
    // it will update() autoly
}

void MarkingWidget::recaculateDisplayBoxes()
{
    if (!_actualBoxes)
        return;

    QList<QRect> displayBoxes;
    foreach (QRect rect, *_actualBoxes) {
        displayBoxes.append(calculateScaledBox(rect));
    }
    _displayBoxes = displayBoxes;
}

qint64 MarkingWidget::getBoxIndex(const QPoint &center)
{
    for (int i = 0; i < _displayBoxes.size(); ++i) {
        QPoint rectCenter = _displayBoxes.at(i).center();
        if (pointsOverlap(rectCenter, center))
            return i;
    }

    return -1;
}

///
/// \brief MarkingWidget::tryMoveBox
/// \return true if successful, false if out of the bound
///
bool MarkingWidget::tryMoveBox(int index, const QPoint &newCenter)
{
    if (index < 0 || index >= _actualBoxes->size())
        return false;

    QPoint dCenter = newCenter - _displayBoxes.at(index).center();
    QRect rect(_displayBoxes.at(index));
    rect.translate(dCenter.x(), dCenter.y());

    const int w = _scaledImage.width();
    const int h = _scaledImage.height();
    // Out of border
    if (rect.left() < 0)
        rect.moveLeft(0);
    if (rect.right() > w)
        rect.moveRight(w);
    if (rect.top() < 0)
        rect.moveTop(0);
    if (rect.bottom() > h)
        rect.moveBottom(h);

    _displayBoxes[index] = rect;
    (*_actualBoxes)[index] = calculateActualBox(rect);
    return true;
}

bool MarkingWidget::pointsOverlap(const QPoint &p1, const QPoint &p2)
{
    return (p1 - p2).manhattanLength() < _pointsOverlapThreshold;
}

void MarkingWidget::scaleToOriginalSize()
{
    if (_image.isNull())
        return;
    _isZooming = true;
    _scaledImage = _image;
    _actualToScaledRatio = 1;
    recaculateDisplayBoxes();
    repaint();

    setFixedSize(_scaledImage.size());
    update();
}

void MarkingWidget::scaleToSizeControl()
{
    if ( _image.isNull())
        return;
    _isZooming = false;
    _scaledImage = _image.scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    _actualToScaledRatio = static_cast<double>(_image.width()) / _scaledImage.width();
    recaculateDisplayBoxes();
    repaint();

    if (_sizeControlWidget) {
        resize(_sizeControlWidget->size());
        setFixedSize(QWIDGETSIZE_MAX, QWIDGETSIZE_MAX);
    }
    update();
}

const QColor MarkingWidget::COLOR_TABLE[] = {
    QColor(235,16,16),
    QColor(16,235,60),
    QColor(68,105,179),
    QColor(122,9,107),
    QColor(235,176,16),
    QColor(68,179,90),
    QColor(123,142,179),
    QColor(122,84,117),
    QColor(122,92,9),
    QColor(89,235,225),
    QColor(79,12,179),
    QColor(235,89,109),
    QColor(153,179,123),
    QColor(47,122,117),
    QColor(191,162,235),
    QColor(122,47,57),
    QColor(16,89,235),
    QColor(235,16,206),
    QColor(179,123,131)
};
