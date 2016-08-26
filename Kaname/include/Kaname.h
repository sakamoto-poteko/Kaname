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

#ifndef KANAME_H
#define KANAME_H

#include <QMainWindow>
#include <QLabel>
#include <MarkingBoxManager.h>

class AbstractImageSource;

namespace Ui {
class Kaname;
}

class Kaname : public QMainWindow
{
    Q_OBJECT

public:
    explicit Kaname(QWidget *parent = 0);
    ~Kaname();

private slots:
    void imageLoadStatusChanged(bool loaded);
    void markerBoxUpdated(const QVector<QRect> &boxes);

    void on_action_AddImages_triggered();
    void on_action_NextImage_triggered();
    void on_action_PreviousImage_triggered();
    void on_action_Save_triggered();
    void on_action_EditObjectNames_triggered();
    void on_action_About_triggered();
    void on_action_ClearImages_triggered();

private:
    Ui::Kaname *ui;
    AbstractImageSource *_imageSource;
    MarkingBoxManager _markingBoxMgr;
    QLabel *_permStatusLbl;

    void updatePermStatusText(const QString &status);
    void updateTempStatusText(const QString &status, int timeout = 2000);
    void getAndRenderImage();
    QColor getContrastColor(const QColor &background);
};

#endif // KANAME_H
