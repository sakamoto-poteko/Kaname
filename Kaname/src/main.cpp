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

#include <QApplication>
#include <QDir>
#include <QPluginLoader>
#include "Kaname.h"
#include "LabelDataFormatInterface.h"
#include "LabelingObjectDefinitionInterface.h"
#include "Kaname_global.h"

static void loadplugins()
{
    QDir pluginsDir(qApp->applicationDirPath());
#if defined(Q_OS_WIN)
#ifndef NDEBUG
    if (pluginsDir.dirName().toLower() == "debug" || pluginsDir.dirName().toLower() == "release")
        pluginsDir.cdUp();
#endif
#elif defined(Q_OS_MAC)
    if (pluginsDir.dirName() == "MacOS") {
        pluginsDir.cdUp();
        pluginsDir.cdUp();
        pluginsDir.cdUp();
    }
#endif
    pluginsDir.cd("plugins");
    foreach (QString fileName, pluginsDir.entryList(QDir::Files)) {
        QPluginLoader pluginLoader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = pluginLoader.instance();
        if (plugin) {
            LabelDataFormatInterface *ldfint = qobject_cast<LabelDataFormatInterface *>(plugin);
            if (ldfint) {
                __kanamePlugins.LabelDataFormatInterfaces[QUuid(ldfint->guid())] = ldfint;
                continue;
            }
            LabelingObjectDefinitionInterface *lodint = qobject_cast<LabelingObjectDefinitionInterface *>(plugin);
            if (lodint) {
               __kanamePlugins.LabelObjectDefinitionInterfaces[QUuid(lodint->guid())] = lodint;
               continue;
            }
        }
    }
}

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    loadplugins();

    Kaname w;
    w.showMaximized();
    return a.exec();
}

KanamePlugins __kanamePlugins;
