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

#define COLOR_TABLE_SIZE    42

class MarkingBoxManager;

class MarkingWidget : public QFrame
{
    Q_OBJECT
public:
    explicit MarkingWidget(QWidget *parent = 0);

    void setImage(const QImage &image, const QString &filename);
    QVector<QRect> getMarkingBoxes() { return *_actualBoxes; }

    void setMarkingBoxManager(MarkingBoxManager *mgr) { _boxmgr = mgr; }
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

signals:
    void boxesUpdated(QVector<QRect> boxes);

public slots:
    void undo();
    void skip();
    void clear();

protected:
    void mousePressEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void paintEvent(QPaintEvent *e);
    void resizeEvent(QResizeEvent *e);

private:
    QPoint _dragOrigin;
    bool _dragging;

    QRect _dragBox;
    QVector<QRect> _displayBoxes;  // coord to display box
    QVector<QRect> *_actualBoxes;   // coord relative to orig img
    QVector<QString> _objectNames;

    QImage _image;
    QImage _scaledImage;
    double _actualToScaledRatio;

    MarkingBoxManager *_boxmgr;

    QRect calculateActualBox(QRect scaledBox)
    {
        return QRect(scaledBox.left() * _actualToScaledRatio,
                     scaledBox.top() * _actualToScaledRatio,
                     scaledBox.width() * _actualToScaledRatio,
                     scaledBox.height() * _actualToScaledRatio);
    }

    QRect calculateScaledBox(QRect actualBox)
    {
        return QRect(actualBox.left() / _actualToScaledRatio,
                     actualBox.top() / _actualToScaledRatio,
                     actualBox.width() / _actualToScaledRatio,
                     actualBox.height() / _actualToScaledRatio);
    }

    void recaculateDisplayBoxes();


    static const QColor COLOR_TABLE[];
};

#endif // MARKINGWIDGET_H
