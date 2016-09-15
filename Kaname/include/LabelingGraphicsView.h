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

#ifndef LABELINGGRAPHICSVIEW_H
#define LABELINGGRAPHICSVIEW_H

#include <QGraphicsView>

#include "GraphicsBoxItem.h"
#include "ObjectInfo.h"

class LabelingGraphicsView : public QGraphicsView
{
public:
    LabelingGraphicsView(QWidget *parent = 0);

    int boxThreshold() const
    {
        return _boxThreshold;
    }
    void setBoxThreshold(int boxThreshold)
    {
        _boxThreshold = boxThreshold;
    }

    QString currentBoxObjName() const
    {
        return _currentBoxObjName;
    }

    void setCurrentBoxObjName(const QString &currentBoxObjName)
    {
        _currentBoxObjName = currentBoxObjName;
    }

    QColor currentBoxColor() const
    {
        return _currentBoxColor;
    }

    void setCurrentBoxColor(const QColor &currentBoxColor)
    {
        _currentBoxColor = currentBoxColor;
    }

    bool currentAspectRatioSet() const
    {
        return _currentAspectRatioSet;
    }

    void setCurrentAspectRatioSet(bool currentAspectRatioSet)
    {
        _currentAspectRatioSet = currentAspectRatioSet;
    }

    double currentAspectRatio() const
    {
        return _currentAspectRatio;
    }

    void setCurrentAspectRatio(double currentAspectRatio)
    {
        _currentAspectRatio = currentAspectRatio;
    }

public slots:
    void scaleUp2x();
    void scaleDown2x();
    void scaleImgSize();
    void scaleFitWindow();
    void selectNextObject();
    void clearSelection();
    void moveSelectedBoxDown();
    void moveSelectedBoxUp();
    void moveSelectedBoxLeft();
    void moveSelectedBoxRight();
    void growSelectedBoxHorizentally();
    void growSelectedBoxVertically();
    void shrinkSelectedBoxHorizentally();
    void shrinkSelectedBoxVertically();
    void deleteSelected();
    void setObjectInfo(const ObjectInfo &info);

protected:
    virtual void mousePressEvent(QMouseEvent *event);
    virtual void mouseReleaseEvent(QMouseEvent *event);
    virtual void mouseDoubleClickEvent(QMouseEvent *event);
    virtual void mouseMoveEvent(QMouseEvent *event);
    virtual void wheelEvent(QWheelEvent *event);

    enum DragState {
        NotDragging,
        NewBox,
        MovingBox,
        CopyingBox
    };

    DragState   _dragState;
    QPointF     _dragOrigin;
    GraphicsBoxItem *_currentBoxItem;
    GraphicsBoxItem *_selectedBoxItem;

    GraphicsBoxItem *centerOverlappedBox(const QPointF &point);
    int _numScheduledScalings;

protected slots:
    void scalingTime(qreal x);
    void animFinished();

private:
    int _boxThreshold;      // 50px by default, press shift to cancel this threshold
    QString _currentBoxObjName;
    QColor  _currentBoxColor;
    bool    _currentAspectRatioSet;
    double  _currentAspectRatio;
};

#endif // LABELINGGRAPHICSVIEW_H
