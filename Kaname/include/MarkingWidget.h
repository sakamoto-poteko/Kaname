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

#ifndef MARKINGWIDGET_H
#define MARKINGWIDGET_H

#include <QFrame>
#include <QCryptographicHash>
#include "Hash128Result.h"

#define COLOR_TABLE_SIZE    19

class MarkingBoxManager;

class MarkingWidget : public QFrame
{
    Q_OBJECT
public:
    explicit MarkingWidget(QWidget *parent = 0);

    void setImage(const QImage &image, const QString &filename);
    QList<QRect> getMarkingBoxes() { return *_actualBoxes; }

    void setMarkingBoxManager(MarkingBoxManager *mgr);
    void setLabelingObjectNames(const QVector<QString> &objnames) { _objectNames = objnames; }
    QVector<QString> getLabelingObjectNames() { return _objectNames;}

    QColor getColor(int idx) { return COLOR_TABLE[idx % COLOR_TABLE_SIZE]; }
    QColor nextColor() { return COLOR_TABLE[_displayBoxes.size() % COLOR_TABLE_SIZE]; }
    QString getObjectName(int idx)
    {
        return idx < _objectNames.size() ? _objectNames.at(idx) : QString();
    }
    QString nextObjectName()
    {
        return _displayBoxes.size() >= _objectNames.size() ? QString() :
                                                             _objectNames.at(_displayBoxes.size());
    }

    quint32 getFalseTriggerThresholdWidth() const
    {
        return _falseTriggerThresholdWidth;
    }
    void setFalseTriggerThresholdWidth(const quint32 &falseTriggerThresholdWidth)
    {
        _falseTriggerThresholdWidth = falseTriggerThresholdWidth;
    }

    quint32 getFalseTriggerThresholdHeight() const
    {
        return _falseTriggerThresholdHeight;
    }
    void setFalseTriggerThresholdHeight(const quint32 &falseTriggerThresholdHeight)
    {
        _falseTriggerThresholdHeight = falseTriggerThresholdHeight;
    }

    qint32 getPointsOverlapThreshold() const
    {
        return _pointsOverlapThreshold;
    }
    void setPointsOverlapThreshold(const qint32 &pointsOverlapThreshold)
    {
        _pointsOverlapThreshold = pointsOverlapThreshold;
    }

    void setSizeControlWidget(QWidget *sizeControlWidget)
    {
        _sizeControlWidget = sizeControlWidget;
    }

signals:
    void boxesUpdated(QList<QRect> boxes);
    void mouseDraggingNewBox(QPoint origin, QPoint current);
    void mouseMovingBox(QPoint oldCenter, QPoint currentCenter);

public slots:
    void undo();
    void skip();
    void clear();
    void selectNextBox();
    void clearBoxSelection();
    void moveSelectedBoxLeft(int pixel = 1);
    void moveSelectedBoxRight(int pixel = 1);
    void moveSelectedBoxUp(int pixel = 1);
    void moveSelectedBoxDown(int pixel = 1);
    // scale a scaled image
    void enlarge2x();
    void shrink2x();
    void scaleToOriginalSize();
    // scale to widget size
    void scaleToSizeControl();

protected:
    void mouseDoubleClickEvent(QMouseEvent *e);
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *e);

private:
    QPoint _dragOrigin;
    bool _draggingNewBox;
    bool _draggingCurrentBox;

    QRect _dragBox;
    QList<QRect> _displayBoxes;  // coord to display box
    QList<QRect> *_actualBoxes;   // coord relative to orig img
    QVector<QString> _objectNames;

    QImage _image;
    QImage _scaledImage;
    double _actualToScaledRatio;
    bool   _isZooming;

    MarkingBoxManager *_boxmgr;

    QWidget *_sizeControlWidget;

    qint64  _currentSelection;
    qint64  _currentSelectionOld;
    qint64  _currentHover;
    ///
    /// \brief _falseTriggerThresholdWidth Default 10
    ///
    quint32 _falseTriggerThresholdWidth;
    ///
    /// \brief _falseTriggerThresholdHeight Default 10
    ///
    quint32 _falseTriggerThresholdHeight;

    qint32 _pointsOverlapThreshold;

    QPoint calculateActualPoint(const QPoint &point)
    {
        return point * _actualToScaledRatio;
    }

    QPoint calculateDisplayPoint(const QPoint &point)
    {
        return point / _actualToScaledRatio;
    }

    QRect calculateActualBox(const QRect &scaledBox)
    {
        return QRect(scaledBox.left() * _actualToScaledRatio,
                     scaledBox.top() * _actualToScaledRatio,
                     scaledBox.width() * _actualToScaledRatio,
                     scaledBox.height() * _actualToScaledRatio);
    }

    QRect calculateScaledBox(const QRect &actualBox)
    {
        return QRect(actualBox.left() / _actualToScaledRatio,
                     actualBox.top() / _actualToScaledRatio,
                     actualBox.width() / _actualToScaledRatio,
                     actualBox.height() / _actualToScaledRatio);
    }

    void recaculateDisplayBoxes();
    qint64 getBoxIndex(const QPoint &center);
    bool tryMoveBox(int index, const QPoint &newCenter);
    bool pointsOverlap(const QPoint &p1, const QPoint &p2);

    static Hash128Result imageHash(const QImage &img)
    {
        QByteArray ary((const char *)img.constBits(), img.bytesPerLine() * img.height());
        QByteArray result = QCryptographicHash::hash(ary, QCryptographicHash::Md5);
        return Hash128Result(result);
    }

    static const QColor COLOR_TABLE[];
};

#endif // MARKINGWIDGET_H
