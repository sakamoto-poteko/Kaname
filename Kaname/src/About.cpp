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
#include "LabelingObjectDefinitionInterface.h"

About::About(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::About)
{
    ui->setupUi(this);

    ui->listComponent->addItem("Kaname");

    foreach (LabelDataFormatInterface *formatPlugin, __kanamePlugins.LabelDataFormatInterfaces) {
        QString pluginName = formatPlugin->name();
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(pluginName);
        item->setToolTip(formatPlugin->guid());
        ui->listComponent->addItem(item);
    }
    foreach (LabelingObjectDefinitionInterface *formatPlugin, __kanamePlugins.LabelObjectDefinitionInterfaces) {
        QString pluginName = formatPlugin->name();
        QListWidgetItem *item = new QListWidgetItem;
        item->setText(pluginName);
        item->setToolTip(formatPlugin->guid());
        ui->listComponent->addItem(item);
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
    QUuid guid = QUuid(current->toolTip());

    if (comName == "Kaname") {
        QString detail("Kaname " KANAME_VERSION "\n"   // Name, Version
                       KANAME_COPYRIGHT "\n\n"          // Copyright
                       "Image Labeling Tool\n");        // Format description

        ui->lblDetail->setText(detail);
        return;
    }

    if (__kanamePlugins.LabelDataFormatInterfaces.contains(guid)) {
        LabelDataFormatInterface *ldfi = __kanamePlugins.LabelDataFormatInterfaces[guid];
        if (ldfi) {
            QString detail = QString("%1 %2\n"  // Name, Version
                                     "%3\n\n"    // Copyright
                                     "%4\n"      // Format description
                                     "UID: %5").  // GUID
                    arg(ldfi->name()).arg(ldfi->version()).arg(ldfi->copyright()).
                    arg(ldfi->formatDescription()).arg(ldfi->guid());
            ui->lblDetail->setText(detail);
            return;
        }
    }

    if (__kanamePlugins.LabelObjectDefinitionInterfaces.contains(guid)) {
        LabelingObjectDefinitionInterface *lodi = __kanamePlugins.LabelObjectDefinitionInterfaces[guid];
        if (lodi) {
            QString detail = QString("%1 %2\n"  // Name, Version
                                     "%3\n\n"    // Copyright
                                     "%4\n"      // Format description
                                     "UID: %5").  // GUID
                    arg(lodi->name()).arg(lodi->version()).arg(lodi->copyright()).
                    arg(lodi->formatDescription()).arg(lodi->guid());
            ui->lblDetail->setText(detail);
            return;
        }
    }
}
