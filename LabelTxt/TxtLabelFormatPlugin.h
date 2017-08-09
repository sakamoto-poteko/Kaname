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

#ifndef TXTLABELFORMATPLUGIN_H
#define TXTLABELFORMATPLUGIN_H

#define TXT_LABEL_FORMAT_PLUGIN_GUID "{DB1CD163-B5B0-482B-B610-1598213A71F4}"
#define TXT_LABEL_FORMAT_PLUGIN_VERSION "0.9.0"
#define TXT_LABEL_FORMAT_PLUGIN_COPYRIGHT "Copyright (c) 2017, Afa.L Cheng <afa@afa.moe>"

#include <QObject>

#include "LabelDataFormatInterface.h"

class TxtLabelFormatPlugin : public QObject, LabelDataFormatInterface
{
    Q_OBJECT
    Q_PLUGIN_METADATA(IID "moe.afa.utils.Kaneme.LabelDataFormatInterface" FILE "LabelTxt.json")
    Q_INTERFACES(LabelDataFormatInterface)
public:
    virtual bool save(const BoxManager &boxes, const QString &savePath) Q_DECL_OVERRIDE;
    virtual bool open(const QString &openPath, BoxManager *boxman, const QString &imgPath) Q_DECL_OVERRIDE;

    QString formatDescription() Q_DECL_OVERRIDE;
    QString formatExtension() Q_DECL_OVERRIDE;
    QString formatName() Q_DECL_OVERRIDE;
    QString name() Q_DECL_OVERRIDE;
    QString version() Q_DECL_OVERRIDE;
    QString copyright() Q_DECL_OVERRIDE;
    QString guid() Q_DECL_OVERRIDE;
};

#endif // TXTLABELFORMATPLUGIN_H
