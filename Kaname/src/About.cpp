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

#include "About.h"
#include "ui_About.h"

#include "Kaname_global.h"
#include "LabelDataFormatInterface.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

    ui->listComponent->addItem("Kaname");

    foreach (LabelDataFormatInterface *formatPlugin, __kanamePlugins.LabelDataFormatInterfaces) {
        QString pluginName = formatPlugin->name();
        _labelDataFormatInterfaces[pluginName] = formatPlugin;
        ui->listComponent->addItem(pluginName);
    }
}

About::~About()
{
    delete ui;
}

void About::on_listComponent_currentItemChanged(QListWidgetItem *current, QListWidgetItem *previous)
{
    Q_UNUSED(previous);
    if (!current)
        return;

    QString comName = current->text();

    if (comName == "Kaname") {
        QString detail("Kaname " KANAME_VERSION "\n"   // Name, Version
                       KANAME_COPYRIGHT "\n\n"          // Copyright
                       "Image Labeling Tool\n");        // Format description

        ui->lblDetail->setText(detail);
        return;
    }

    if (_labelDataFormatInterfaces.contains(comName)) {
        LabelDataFormatInterface *p = _labelDataFormatInterfaces[comName];
        QString detail = QString("%1 %2\n"  // Name, Version
                                 "%3\n\n"    // Copyright
                                 "%4\n"      // Format description
                                 "UID: %5").  // GUID
                arg(p->name()).arg(p->version()).arg(p->copyright()).
                arg(p->formatDescription()).arg(p->guid());
        ui->lblDetail->setText(detail);
        return;
    }
}
