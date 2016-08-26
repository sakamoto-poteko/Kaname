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
#include "Kaname_global.h"
#include "LabelDataFormatInterface.h"
#include "ObjectNameEditor.h"
#include "About.h"

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
    connect(ui->action_ClearImages, SIGNAL(triggered()), ui->leImageName, SLOT(clear()));
    connect(ui->action_ClearImages, SIGNAL(triggered()), ui->leImageSize, SLOT(clear()));
    connect(ui->action_ClearImages, SIGNAL(triggered()), ui->leNextObj, SLOT(clear()));
    connect(ui->action_ClearImages, SIGNAL(triggered()), ui->listMarkedObjects, SLOT(clear()));
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
    ui->action_EditObjectNames->setEnabled(status);


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
        QString objname = ui->marker->getObjectName(i);
        QListWidgetItem *item = new QListWidgetItem(QString("(%1, %2), %3 x %4 - (%5)").
                                                    arg(rect.x(), 4, 10, QChar(' ')).
                                                    arg(rect.y(), 4, 10,  QChar(' ')).
                                                    arg(rect.width(), 4, 10,  QChar(' ')).
                                                    arg(rect.height(), 4, 10,  QChar(' ')).
                                                    arg(objname.isEmpty() ? tr("null") : objname));
        QColor boxColor = ui->marker->getColor(i);
        item->setBackgroundColor(boxColor);
        item->setTextColor(getContrastColor(boxColor));
        item->setFont(QFont("Monospace"));
        ui->listMarkedObjects->addItem(item);
    }

    QColor nextColor = ui->marker->nextColor();
    QColor conColor = getContrastColor(nextColor);
    QString ss = QString("background-color: rgb(%1, %2, %3);color: rgb(%4, %5, %6);").
            arg(nextColor.red()).arg(nextColor.green()).arg(nextColor.blue()).
            arg(conColor.red()).arg(conColor.green()).arg(conColor.blue());
    ui->leNextObj->setStyleSheet(ss);

    QString nextobjname = ui->marker->nextObjectName();
    ui->leNextObj->setText(nextobjname);
}

void Kaname::on_action_AddImages_triggered()
{
    QStringList files = QFileDialog::getOpenFileNames(this, tr("Choose images to mark"),
                                                  #ifndef NDEBUG
                                                      QString("H:/c83_enako"),
                                                  #else
                                                      QString(),
                                                  #endif
                                                      "Images (*.bmp *.png *.xpm *.jpg *.jpeg "
                                                      "*.gif *.xbm *.ppm *.pbm);;All files (*.*)");

    if (!files.isEmpty()) {
        _imageSource->addSources(files);
        updateTempStatusText(QString(tr("%1 files added.")).arg(files.size()), 2000);
    }
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
    QString fullname = _imageSource->getImageName();
    QString shortname(QFileInfo(fullname).fileName());
    ui->marker->setImage(img, fullname);
    ui->leImageName->setText(shortname);
    ui->leImageSize->setText(QString("%1 x %2").
                             arg(img.size().width()).
                             arg(img.size().height()));

    if (img.isNull()) {
        updateTempStatusText(tr("Invalid image"), 5000);
    } else {
        updateTempStatusText(tr("Image updated"));
    }
}

QColor Kaname::getContrastColor(const QColor &background)
{
    double gray = background.red() * 0.299 + background.green() * 0.587 + background.blue() * 0.114;
    if (gray > 130)
        return Qt::black;
    else
        return Qt::white;
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

void Kaname::on_action_Save_triggered()
{
    QStringList supportedFormats;
    foreach (auto formatInterface, __kanamePlugins.LabelDataFormatInterfaces) {
        supportedFormats.append(QString("%1 (%2)").
                                arg(formatInterface->formatName()).
                                arg(formatInterface->formatExtension()));
    }

    QString filename = QFileDialog::getSaveFileName(this, tr("Save"), QString(),
                                                    supportedFormats.join(QByteArray::fromStdString(";;")));
    if (filename.isEmpty())
        return;

    QString ext = filename.split('.').last();
    LabelDataFormatInterface *saver = __kanamePlugins.LabelDataFormatInterfaces["*." + ext];
    if (!saver) {
        throw("Failed to retrieve saving plugin. This is a bug.");
    }

    bool saved = saver->save(_markingBoxMgr.getAllBoxes(), filename, ui->marker->getLabelingObjectNames());

    if (saved)
        updateTempStatusText(tr("Labeling data saved."));
    else
        updateTempStatusText(tr("Labeling data failed to save."));
}

void Kaname::on_action_EditObjectNames_triggered()
{
    ObjectNameEditor editor(this);
    editor.setLabelingObjectNames(ui->marker->getLabelingObjectNames());

    if (editor.exec() == QDialog::Accepted) {
        QVector<QString> objnames = editor.getLabelingObjectNames();
        ui->marker->setLabelingObjectNames(objnames);
        updateTempStatusText(QString(tr("%1 objects in object list")).arg(objnames.size()));
    }
}

void Kaname::on_action_About_triggered()
{
    About about;
    about.exec();
}

void Kaname::on_action_ClearImages_triggered()
{
    _markingBoxMgr.clear();
    ui->leNextObj->setStyleSheet("");
}
