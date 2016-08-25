/**************************************************************************
 * Copyright (c) 2015 Afa.L Cheng <afa@afa.moe>
 *
 * Permission is hereby granted, free of charge, to any person obtaining
 * a copy of this software and associated documentation files (the
 * "Software"), to deal in the Software without restriction, including
 * without limitation the rights to use, copy, modify, merge, publish,
 * distribute, sublicense, and/or sell copies of the Software, and to
 * permit persons to whom the Software is furnished to do so, subject to
 * the following conditions:
 *
 * The above copyright notice and this permission notice shall be
 * included in all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
 * EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
 * MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
 * NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE
 * LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION
 * OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
 * WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
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

    void setImage(const QImage &image);
    void setActualBoxes(const QVector<QRect> &boxes);
    QVector<QRect> getMarkingBoxes() { return _actualBoxes; }

    void setMarkingBoxManager(MarkingBoxManager *mgr) { _boxmgr = mgr; }

    QColor getColor(int idx) { return COLOR_TABLE[idx % COLOR_TABLE_SIZE]; }
    QColor nextColor() { return COLOR_TABLE[_actualBoxes.size() % COLOR_TABLE_SIZE]; }

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
    QVector<QRect> _actualBoxes;  // coord relative to orig img

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
    void syncActualBoxToMgr();
    void syncActualBoxFromMgr();

    static const QColor COLOR_TABLE[];
};

#endif // MARKINGWIDGET_H
