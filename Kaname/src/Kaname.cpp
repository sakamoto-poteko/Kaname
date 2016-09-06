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
#include <QMessageBox>
#include <QGraphicsScene>

#include "Kaname.h"
#include "ui_Kaname.h"
#include "StillImageSource.h"
#include "Kaname_global.h"
#include "LabelDataFormatInterface.h"
#include "LabelingObjectDefinitionInterface.h"
#include "ObjectNameEditor.h"
#include "About.h"
#include "LabelingScene.h"

Kaname::Kaname(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Kaname), _boxManager(0), _labelingScene(0)
{
    ui->setupUi(this);
    ui->objectsVLayout->setAlignment(Qt::AlignTop);

    _nullScene = new QGraphicsScene;
    _boxManager = new BoxManager;
    _imageSource = new StillImageSource;
    _permStatusLbl = new QLabel;
    ui->statusBar->addPermanentWidget(_permStatusLbl);
    ui->labelingView->setScene(_nullScene);

    connect(ui->action_ClearImages, &QAction::triggered,    _imageSource,       &AbstractImageSource::clearSources);
    connect(ui->action_ClearImages, &QAction::triggered,    ui->leImageName,    &QLineEdit::clear);
    connect(ui->action_ClearImages, &QAction::triggered,    ui->leImageSize,    &QLineEdit::clear);
    connect(ui->action_ClearImages, &QAction::triggered,    ui->leNextObj,      &QLineEdit::clear);
    connect(ui->action_ClearSelection,  &QAction::triggered,ui->labelingView,   &LabelingGraphicsView::clearSelection);
    connect(ui->action_MoveDown,    &QAction::triggered,    ui->labelingView,   &LabelingGraphicsView::moveSelectedBoxDown);
    connect(ui->action_MoveUp,      &QAction::triggered,    ui->labelingView,   &LabelingGraphicsView::moveSelectedBoxUp);
    connect(ui->action_MoveRight,   &QAction::triggered,    ui->labelingView,   &LabelingGraphicsView::moveSelectedBoxRight);
    connect(ui->action_MoveLeft,    &QAction::triggered,    ui->labelingView,   &LabelingGraphicsView::moveSelectedBoxLeft);
    connect(ui->action_SelectNextObject,&QAction::triggered,ui->labelingView,   &LabelingGraphicsView::selectNextObject);
    connect(ui->action_Zoom100, &QAction::triggered,    ui->labelingView, &LabelingGraphicsView::scaleImgSize);
    connect(ui->action_ZoomIn,  &QAction::triggered,    ui->labelingView, &LabelingGraphicsView::scaleUp2x);
    connect(ui->action_ZoomOut, &QAction::triggered,    ui->labelingView, &LabelingGraphicsView::scaleDown2x);
    connect(ui->action_ZoomReset,   &QAction::triggered,ui->labelingView, &LabelingGraphicsView::scaleFitWindow);
    connect(ui->action_DeleteSelected,  &QAction::triggered,ui->labelingView,   &LabelingGraphicsView::deleteSelected);
    connect(_imageSource,   &AbstractImageSource::sourceStatusChanged,  this,   &Kaname::imageLoadStatusChanged);
    connect(_imageSource,   &AbstractImageSource::sourceChanged,        this,   &Kaname::imageSourceChanged);


    setButtonStatus(false);
//    setButtonStatus(true);

//    ObjectInfo oi, oj;
//    oi.aspectRatioSet = false;
//    oi.objectColor = Qt::green;
//    oi.objectName = "sucks";
//    oj.aspectRatioSet = true;
//    oj.aspectRatio = 1.;
//    oj.objectColor = Qt::yellow;
//    oj.objectName = "fuck";

//    populateObjectSelectionButtons(QList<ObjectInfo>{oi,oj});
}


Kaname::~Kaname()
{
    delete ui;
    delete _imageSource;
    delete _boxManager;
    delete _nullScene;
}

void Kaname::setButtonStatus(bool loaded)
{
    bool notloaded = !loaded;
    ui->action_AddImages->setEnabled(notloaded);
    ui->action_EditObjectNames->setEnabled(notloaded);
    ui->action_Open->setEnabled(notloaded);

    ui->action_ClearImages->setEnabled(loaded);
    ui->action_ClearSelection->setEnabled(loaded);
    ui->action_MoveDown->setEnabled(loaded);
    ui->action_MoveLeft->setEnabled(loaded);
    ui->action_MoveRight->setEnabled(loaded);
    ui->action_MoveUp->setEnabled(loaded);
    ui->action_NextImage->setEnabled(loaded);
    ui->action_NextObject->setEnabled(loaded);
    ui->action_PreviousImage->setEnabled(loaded);
    ui->action_Save->setEnabled(loaded);
    ui->action_SelectNextObject->setEnabled(loaded);
    ui->action_UndoCurrentMark->setEnabled(loaded);
    ui->action_Zoom100->setEnabled(loaded);
    ui->action_ZoomIn->setEnabled(loaded);
    ui->action_ZoomOut->setEnabled(loaded);
    ui->action_ZoomReset->setEnabled(loaded);
}

void Kaname::imageLoadStatusChanged(bool loaded)
{
    setButtonStatus(loaded);

    if (loaded) {
        updatePermStatusText(QString(tr("%1 files loaded.")).arg(_imageSource->sourceCount()));

        // load 1st image
        getAndRenderImage();
    }
}

void Kaname::imageSourceChanged(const QStringList &sources)
{
    _boxManager->clear();
    foreach (const QString &str, sources) {
        QString shortname(QFileInfo(str).fileName());
        (*_boxManager)[shortname];
    }
}

void Kaname::drawingNewBox(const QPoint &origin, const QPoint &current)
{
    updateTempStatusText(QString(tr("New Box: (%1, %2) -> (%3, %4)")).
                         arg(origin.x()).arg(origin.y()).
                         arg(current.x()).arg(current.y()),
                         100);
}

void Kaname::movingCurrentBox(const QPoint &old, const QPoint &current)
{
    updateTempStatusText(QString(tr("Moving Box: (%1, %2) -> (%3, %4)")).
                         arg(old.x()).arg(old.y()).
                         arg(current.x()).arg(current.y()),
                         100);
}

void Kaname::clearObjectSelectionButtons()
{
    foreach (ObjectSelectionButton *btn, _objSelectionButtons) {
        delete btn;
    }
    _objSelectionButtons.clear();
}

void Kaname::populateObjectSelectionButtons(const QList<ObjectInfo> &objs)
{
    for (int i = 0; i < objs.size(); ++i) {
        ObjectInfo info = objs.at(i);
        ObjectSelectionButton *btn = new ObjectSelectionButton(info, ui->objectsList);
        connect(btn, &ObjectSelectionButton::selected, ui->labelingView, &LabelingGraphicsView::setObjectInfo);
        connect(btn, &ObjectSelectionButton::selected, [this](const ObjectInfo &info) {
            QColor nextColor = info.objectColor;
            QColor conColor = ObjectSelectionButton::getContrastColor(nextColor);
            QString ss = QString("background-color: rgb(%1, %2, %3);color: rgb(%4, %5, %6);").
                    arg(nextColor.red()).arg(nextColor.green()).arg(nextColor.blue()).
                    arg(conColor.red()).arg(conColor.green()).arg(conColor.blue());
            ui->leNextObj->setStyleSheet(ss);
            ui->leNextObj->setText(info.objectName);
        });

        ui->objectsVLayout->addWidget(btn);
        _objSelectionButtons.append(btn);

        if (i < 9)
            btn->setShortcut(QKeySequence(Qt::Key_1 + i));
        if (i == 9)
            btn->setShortcut(QKeySequence(Qt::Key_0));
    }
    if (!_objSelectionButtons.empty()) {
        _objSelectionButtons.first()->click();
    }
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
        _lastDir = QFileInfo(files.first()).absoluteDir().absolutePath();

        _imageSource->addSources(files);
        updateTempStatusText(QString(tr("%1 files added.")).arg(files.size()), 2000);
        _imageSource->load();
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

    if (_labelingScene)
        delete _labelingScene;
    _labelingScene = new LabelingScene(img, shortname, _boxManager, this);
    ui->labelingView->setScene(_labelingScene);
    ui->labelingView->scaleFitWindow();

    ui->leImageName->setText(shortname);
    ui->leImageSize->setText(QString("%1 x %2").
                             arg(img.size().width()).
                             arg(img.size().height()));

    if (!_objSelectionButtons.empty()) {
        _objSelectionButtons.first()->click();
    }

    if (img.isNull()) {
        updateTempStatusText(tr("Invalid image"), 5000);
    } else {
        updateTempStatusText(tr("Image updated"));
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

void Kaname::on_action_Save_triggered()
{
    QStringList supportedFormats;
    QHash<QString, LabelDataFormatInterface *> formatMap;
    foreach (auto formatInterface, __kanamePlugins.LabelDataFormatInterfaces) {
        QString filter = QString("%1 (%2)").
                arg(formatInterface->formatName()).
                arg(formatInterface->formatExtension());
        supportedFormats.append(filter);
        formatMap[filter] = formatInterface;
    }

    QString selectedFilter;
    QString filename = QFileDialog::getSaveFileName(this, tr("Save"), _lastDir,
                                                    supportedFormats.join(QByteArray::fromStdString(";;")),
                                                    &selectedFilter);
    if (filename.isEmpty())
        return;

    QString ext = QFileInfo(filename).suffix();
    if (ext.isEmpty()) { // No plugin loaded
        QMessageBox::critical(this, tr("Unable to save"), tr("No format plugin loaded. Cannot save."));
        return;
    }

    LabelDataFormatInterface *saver = formatMap[selectedFilter];
    if (!saver) {
        throw("Failed to retrieve saving plugin. This is a bug.");
    }

    bool saved = saver->save(*_boxManager, filename);

    if (saved)
        updateTempStatusText(tr("Labeling data saved."));
    else
        updateTempStatusText(tr("Labeling data failed to save."));
}

void Kaname::on_action_About_triggered()
{
    About about;
    about.exec();
}

void Kaname::on_action_ClearImages_triggered()
{
    ui->leNextObj->setStyleSheet("");
    _boxManager->clear();
    ui->labelingView->setScene(_nullScene);
    delete _labelingScene;
    _labelingScene = 0;
}

void Kaname::on_action_Open_triggered()
{
    QStringList supportedFormats;
    QHash<QString, LabelDataFormatInterface *> formatMap;
    foreach (auto formatInterface, __kanamePlugins.LabelDataFormatInterfaces) {
        QString filter = QString("%1 (%2)").
                arg(formatInterface->formatName()).
                arg(formatInterface->formatExtension());
        supportedFormats.append(filter);
        formatMap[filter] = formatInterface;
    }

    QString selectedFilter;
    QString filename = QFileDialog::getOpenFileName(this, tr("Save"),
                                                #ifndef NDEBUG
                                                    QString("H:/c83_enako"),
                                                #else
                                                    _lastDir,
                                                #endif
                                                    supportedFormats.join(QByteArray::fromStdString(";;")),
                                                    &selectedFilter);
    if (filename.isEmpty())
        return;

    LabelDataFormatInterface *opener = formatMap[selectedFilter];
    if (!opener) {
        throw("Failed to retrieve open plugin. This is a bug.");
    }

    BoxManager boxMgr;
    auto savedData = opener->open(filename, &boxMgr);

    if (!savedData) {
        QMessageBox::warning(this, tr("Empty dataset"), tr("Unable to load data."));
        updateTempStatusText("Empty dataset");
        return;
    } else {
        QStringList imgFiles;
        QList<QString> imgs = boxMgr.keys();
        QDir dir = QFileInfo(filename).dir();
        std::for_each(imgs.constBegin(), imgs.constEnd(), [&imgFiles, &dir](const QString &str){
            imgFiles << dir.filePath(str);
        });
        _imageSource->addSources(imgFiles);
        *_boxManager = boxMgr;
        _imageSource->load();
        updateTempStatusText(tr("Labeling data loaded."));
    }
}


void Kaname::resizeEvent(QResizeEvent *event)
{
    Q_UNUSED(event);
    ui->labelingView->scaleFitWindow();
}

void Kaname::on_action_EditObjectNames_triggered()
{
    QStringList supportedFormats;
    QHash<QString, LabelingObjectDefinitionInterface *> formatMap;
    foreach (auto formatInterface, __kanamePlugins.LabelObjectDefinitionInterfaces) {
        QString filter = QString("%1 (%2)").
                arg(formatInterface->formatName()).
                arg(formatInterface->formatExtension());
        supportedFormats.append(filter);
        formatMap[filter] = formatInterface;
    }

    QString selectedFilter;
    QString filename = QFileDialog::getOpenFileName(this, tr("Save"),
                                                #ifndef NDEBUG
                                                    QString("H:/c83_enako"),
                                                #else
                                                    _lastDir,
                                                #endif
                                                    supportedFormats.join(QByteArray::fromStdString(";;")),
                                                    &selectedFilter);
    if (filename.isEmpty())
        return;

    auto opener = formatMap[selectedFilter];
    if (!opener) {
        throw("Failed to retrieve open plugin. This is a bug.");
    }

    ObjectInfoList objInfoList = opener->open(filename);
    populateObjectSelectionButtons(objInfoList);
}
