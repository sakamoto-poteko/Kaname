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

#include <cassert>

#include <QRubberBand>
#include <QMouseEvent>
#include <QPaintEvent>
#include <QPainter>
#include <QResizeEvent>
#include <QDebug>

#include "MarkingWidget.h"
#include "MarkingBoxManager.h"

MarkingWidget::MarkingWidget(QWidget *parent) : QFrame(parent),
  _dragging(false), _actualBoxes(0), _boxmgr(0)
{
}

void MarkingWidget::setImage(const QImage &image, const QString &filename)
{
    if (!_boxmgr)
        return;

    clear();

    setCursor(Qt::CrossCursor);
    _dragging = false;

    _image = image;
    _scaledImage = image.scaled(width(), height(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    _actualToScaledRatio = static_cast<double>(_image.width()) / _scaledImage.width();
    _boxmgr->setBoxName(_image.cacheKey(), filename);
    _actualBoxes = _boxmgr->getBoxesRef(_image.cacheKey());
    update();

    recaculateDisplayBoxes();
    update();

    emit boxesUpdated(*_actualBoxes);
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
    _actualBoxes = 0;
    _displayBoxes.clear();
    _scaledImage = QImage();
    _image = QImage();

    update();
}

void MarkingWidget::mousePressEvent(QMouseEvent *e)
{
    auto pos = e->pos();
    int w = _scaledImage.width();
    int h = _scaledImage.height();

    if (pos.x() < 0 || pos.y() < 0 ||  pos.x() > w || pos.y() > h) {
        return;
    }

    // else
    _dragOrigin = pos;
    _dragging = true;
    _dragBox = QRect(_dragOrigin, QSize());
}

void MarkingWidget::mouseMoveEvent(QMouseEvent *e)
{
    auto pos = e->pos();
    int w = _scaledImage.width();
    int h = _scaledImage.height();

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
}

void MarkingWidget::mouseReleaseEvent(QMouseEvent *e)
{
    Q_UNUSED(e);
    if (!_dragging)
        return;

    if (!_actualBoxes)
        return;

    QRect normalizedBox = _dragBox.normalized();
    _displayBoxes.append(normalizedBox);
    _actualBoxes->append(calculateActualBox(normalizedBox));
    _dragging = false;
    update();
    emit boxesUpdated(*_actualBoxes);
}

void MarkingWidget::paintEvent(QPaintEvent *e)
{
    Q_UNUSED(e)
    QPainter painter(this);

    if (!_scaledImage.isNull()) {
        painter.drawImage(0, 0, _scaledImage);
    }

    if (_dragging) {
        painter.setPen(COLOR_TABLE[_displayBoxes.size() % COLOR_TABLE_SIZE]);
        painter.drawRect(_dragBox);
    }

    for(int i = 0; i < _displayBoxes.size(); ++i) {
        painter.setPen(COLOR_TABLE[i % COLOR_TABLE_SIZE]);
        QRect rect = _displayBoxes.at(i);
        painter.drawRect(rect);
    }
}

void MarkingWidget::resizeEvent(QResizeEvent *e)
{
    _scaledImage = _image.scaled(e->size(), Qt::KeepAspectRatio, Qt::SmoothTransformation);
    _actualToScaledRatio = (double)_image.width() / _scaledImage.width();
    recaculateDisplayBoxes();
}

void MarkingWidget::recaculateDisplayBoxes()
{
    if (!_actualBoxes)
        return;

    QVector<QRect> displayBoxes;
    foreach (QRect rect, *_actualBoxes) {
        displayBoxes.append(calculateScaledBox(rect));
    }
    _displayBoxes = displayBoxes;
}

const QColor MarkingWidget::COLOR_TABLE[] = {
    QColor(51,0,0),
    QColor(115,57,57),
    QColor(115,15,0),
    QColor(230,180,172),
    QColor(255,115,64),
    QColor(76,34,19),
    QColor(204,143,102),
    QColor(128,108,96),
    QColor(140,75,0),
    QColor(242,162,0),
    QColor(51,34,0),
    QColor(255,238,0),
    QColor(102,97,26),
    QColor(191,188,143),
    QColor(48,51,38),
    QColor(143,191,48),
    QColor(33,51,13),
    QColor(41,102,0),
    QColor(198,242,182),
    QColor(0,230,0),
    QColor(0,217,0),
    QColor(57,115,80),
    QColor(0,191,153),
    QColor(163,217,213),
    QColor(29,109,115),
    QColor(0,194,242),
    QColor(0,41,51),
    QColor(0,102,153),
    QColor(153,180,204),
    QColor(64,140,255),
    QColor(67,73,89),
    QColor(0,34,255),
    QColor(35,49,140),
    QColor(29,16,64),
    QColor(177,163,217),
    QColor(213,128,255),
    QColor(204,0,255),
    QColor(128,0,119),
    QColor(191,48,124),
    QColor(191,143,169),
    QColor(64,32,45),
    QColor(229,57,80)
};
