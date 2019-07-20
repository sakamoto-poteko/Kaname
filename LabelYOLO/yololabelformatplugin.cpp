
#include <QFile>
#include <QFileInfo>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>

#include "yololabelformatplugin.h"


YoloLabelFormatPlugin::YoloLabelFormatPlugin(QObject *parent) : QObject(parent)
{
}

static int getObjId(QVector<QString> &objectNames, const QString &objName)
{
    int i = 0;
    foreach (auto name, objectNames) {
        if (name == objName) {
            return i;
        } else {
            ++i;
        }
    }

    objectNames.append(objName);
    return i;
}

bool YoloLabelFormatPlugin::save(const BoxManager &boxes, const QString &savePath)
{
    QFile file(savePath);
    auto path = QFileInfo(file).absoluteDir();

    if (!file.open(QIODevice::ReadWrite | QIODevice::Truncate))
        return false;

    QFileInfo train_txt_fileinfo(path, "train.txt");
    QFile train_txt(train_txt_fileinfo.absoluteFilePath());
    if (!train_txt.open(QIODevice::ReadWrite | QIODevice::Truncate))
        return false;

    QFileInfo objnames_txt_fileinfo(path, "obj.names");
    QFile objnames_txt(objnames_txt_fileinfo.absoluteFilePath());
    if (!objnames_txt.open(QIODevice::ReadWrite | QIODevice::Truncate))
        return false;

    QVector<QString> trainTxt;
    QVector<QString> objectNames;

    auto keys = boxes.keys();
    foreach (QString imgFilename, keys) {
        LabelingBoxList boxList = boxes[imgFilename];
        QSize imgSize = boxes.getImageSize(imgFilename);

        auto imgname = QFileInfo(imgFilename).completeBaseName().append(".txt");
        QFileInfo imgfi(path, imgname);
        QFile imgtxt(imgfi.absoluteFilePath());
        if (!imgtxt.open(QIODevice::ReadWrite | QIODevice::Truncate))
            return false;

        for (int i = 0; i < boxList.size(); ++i) {
            const LabelingBox& box = boxList.at(i);

            QRectF rect = box.position;
            auto center = rect.center();

            auto objId = getObjId(objectNames, box.objName);

            if (imgSize.isEmpty()) {
                QMessageBox::critical(nullptr, tr("Saving Error"), "View all images before saving yolo format!");
                return false;
            }

            auto x = center.x() / imgSize.width();
            auto width = rect.width() / imgSize.width();
            auto y = center.y() / imgSize.height();
            auto height = rect.height() / imgSize.height();

            auto str = QString("%1 %2 %3 %4 %5")
                    .arg(objId)
                    .arg(x)
                    .arg(y)
                    .arg(width)
                    .arg(height);

            QTextStream stream(&imgtxt);
            stream << str << endl;
        }

        imgtxt.close();

        trainTxt.append(imgFilename);
    }

    QTextStream trainStr(&train_txt);
    foreach (auto train, trainTxt) {
        trainStr << train << endl;
    }
    train_txt.close();

    QTextStream objStr(&objnames_txt);
    foreach (auto objectName, objectNames) {
        objStr << objectName << endl;
    }
    objnames_txt.close();

    return true;
}

bool YoloLabelFormatPlugin::open(const QString &openPath, BoxManager *boxMan, const QString &imgPath)
{
    return false;
}

QString YoloLabelFormatPlugin::formatDescription()
{
    return "YOLO darknet annotation format";
}

QString YoloLabelFormatPlugin::formatExtension()
{
    return "yolo_placeholder.txt";
}

QString YoloLabelFormatPlugin::formatName()
{
    return "YOLO";
}

QString YoloLabelFormatPlugin::name()
{
    return "YOLO darknet annotation format";
}

QString YoloLabelFormatPlugin::version()
{
    return YOLO_LABEL_FORMAT_PLUGIN_VERSION;
}

QString YoloLabelFormatPlugin::copyright()
{
    return YOLO_LABEL_FORMAT_PLUGIN_COPYRIGHT;
}

QString YoloLabelFormatPlugin::guid()
{
    return YOLO_LABEL_FORMAT_PLUGIN_GUID;
}
