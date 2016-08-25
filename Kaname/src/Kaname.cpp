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

#include <QFileDialog>

#include "Kaname.h"
#include "ui_Kaname.h"

#include "StillImageSource.h"

Kaname::Kaname(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Kaname)
{
    ui->setupUi(this);
    _imageSource = new StillImageSource();
    _permStatusLbl = new QLabel;
    ui->statusBar->addPermanentWidget(_permStatusLbl);

    ui->marker->setMarkingBoxManager(&_markingBoxMgr);

    connect(ui->action_ClearImages, SIGNAL(triggered()), _imageSource, SLOT(clearSources()));
    connect(ui->action_ClearImages, SIGNAL(triggered()), ui->marker, SLOT(clear()));
    connect(ui->action_LoadImages, SIGNAL(triggered()), _imageSource, SLOT(load()));
    connect(ui->action_UndoCurrentMark, SIGNAL(triggered()), ui->marker, SLOT(undo()));
    connect(ui->action_NextObject, SIGNAL(triggered()), ui->marker, SLOT(skip()));
    connect(ui->marker, SIGNAL(boxesUpdated(QVector<QRect>)), this, SLOT(markerBoxUpdated(QVector<QRect>)));
    connect(_imageSource, SIGNAL(sourceStatusChanged(bool)), this, SLOT(imageLoadStatusChanged(bool)));

}

Kaname::~Kaname()
{
    delete ui;
    delete _imageSource;
}

void Kaname::imageLoadStatusChanged(bool loaded)
{
    bool status = !loaded;
    ui->action_AddImages->setEnabled(status);
    ui->action_LoadImages->setEnabled(status);


    if (loaded) {
        updatePermStatusText(QString(tr("%1 files loaded.")).arg(_imageSource->sourceCount()));

        // load 1st image
        getAndRenderImage();
    }
}

void Kaname::markerBoxUpdated(const QVector<QRect> &boxes)
{
    ui->listMarkedObjects->clear();

    for (int i = 0; i < boxes.size(); ++i) {
        QRect rect = boxes.at(i);
        QListWidgetItem *item = new QListWidgetItem(QString("(%1, %2), %3 x %4").
                                                    arg(rect.x(), 4, 10, QChar('0')).
                                                    arg(rect.y(), 4, 10,  QChar('0')).
                                                    arg(rect.width(), 4, 10,  QChar('0')).
                                                    arg(rect.height(), 4, 10,  QChar('0')));
        QColor boxColor = ui->marker->getColor(i);
        item->setBackgroundColor(boxColor);

        double gray = boxColor.red() * 0.299 + boxColor.green() * 0.587 + boxColor.blue() * 0.114;
        if (gray > 130)
            item->setTextColor(Qt::black);
        else
            item->setTextColor(Qt::white);
        ui->listMarkedObjects->addItem(item);
    }
    QPalette pal;
    pal.setColor(ui->leNextObj->backgroundRole(), ui->marker->nextColor());
    ui->leNextObj->setPalette(pal);
}


void Kaname::on_action_AddImages_triggered()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Choose images to mark"),
                                                  #ifndef NDEBUG
                                                      QString("H:/(C83) (COSPLAY) [ENAKO] 虚空のカナリア"),
                                                  #else
                                                      QString(),
                                                  #endif
                                                      "Images (*.bmp *.png *.xpm *.jpg *.jpeg "
                                                      "*.gif *.xbm *.ppm *.pbm);;All files (*.*)");

    if (!files.isEmpty()) {
        _imageSource->addSources(files);
    }

    updateTempStatusText(QString(tr("Adding %1 files...")).arg(files.size()));
}

void Kaname::updatePermStatusText(const QString &status)
{
    _permStatusLbl->setText(status);
}

void Kaname::updateTempStatusText(const QString &status, int timeout)
{
    ui->statusBar->showMessage(status, timeout);
}

void Kaname::getAndRenderImage()
{
    QImage img = _imageSource->getImage();
    if (img.isNull()) {
        updateTempStatusText(tr("Invalid image"));
        return;
    } else {
        // Set img
        ui->marker->setImage(img);

        // Set boxes
        QString filename(QFileInfo(_imageSource->getImageName()).fileName());
        ui->leImageName->setText(filename);
        ui->leImageSize->setText(QString("%1 x %2").
                                 arg(img.size().width()).
                                 arg(img.size().height()));
    }
}

void Kaname::on_action_NextImage_triggered()
{
    if (_imageSource->moveNext()) {
        getAndRenderImage();
    } else {
        updateTempStatusText(tr("This is the last image"), 5000);
    }
}

void Kaname::on_action_PreviousImage_triggered()
{
    if (_imageSource->movePrevious()) {
        getAndRenderImage();
    } else {
        updateTempStatusText(tr("This is the first image"), 5000);
    }
}
