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

#include "TxtLabelFormatPlugin.h"

#include <QFile>
#include <QTextStream>


bool TxtLabelFormatPlugin::save(const BoxManager &boxes, const QString &savePath)
{
    QFile file(savePath);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Truncate))
        return false;

    QTextStream fs(&file);
    // FIXME: Not Implemented
    throw;
//    foreach (/*QPair<QString, LabelingBoxList>*/auto imgBoxes, boxes) {
//        file << imgBoxes.first << " ";

//        foreach (const LabelingBox box, LabelingBoxList) {
////            file << box.aspectRatio
//        }
//    }
}

bool TxtLabelFormatPlugin::open(const QString &openPath, BoxManager *boxman, const QString &imgPath)
{
    return false;
}

QString TxtLabelFormatPlugin::formatDescription()
{
    return "Lingdong TXT format for annotating, designed by Qiang.Wang";
}

QString TxtLabelFormatPlugin::formatExtension()
{
    return "*.txt";
}

QString TxtLabelFormatPlugin::formatName()
{
    return "Txt";
}

QString TxtLabelFormatPlugin::name()
{
    return "TXT annotating format";
}

QString TxtLabelFormatPlugin::version()
{
    return TXT_LABEL_FORMAT_PLUGIN_VERSION;
}

QString TxtLabelFormatPlugin::copyright()
{
    return TXT_LABEL_FORMAT_PLUGIN_COPYRIGHT;
}

QString TxtLabelFormatPlugin::guid()
{
    return TXT_LABEL_FORMAT_PLUGIN_GUID;
}


