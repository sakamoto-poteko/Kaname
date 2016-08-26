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

#include "ObjectNameEditor.h"
#include "ui_ObjectNameEditor.h"

ObjectNameEditor::ObjectNameEditor(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ObjectNameEditor)
{
    ui->setupUi(this);
}

ObjectNameEditor::~ObjectNameEditor()
{
    delete ui;
}

QVector<QString> ObjectNameEditor::getLabelingObjectNames()
{
    QVector<QString> vec;
    for (int i = 0; i < ui->listWidget->count(); ++i) {
        QString text = ui->listWidget->item(i)->text();
        if (text.isEmpty() || text == tr("(null)"))
            continue;
        else
            vec.append(text);
    }
    return vec;
}

void ObjectNameEditor::setLabelingObjectNames(const QVector<QString> &objs)
{
    foreach (QString obj, objs) {
        QListWidgetItem *item = new QListWidgetItem(obj);
        item->setFlags(item->flags() | Qt::ItemIsEditable);
        ui->listWidget->addItem(item);
    }
}

void ObjectNameEditor::on_btnDel_clicked()
{
    delete ui->listWidget->takeItem(ui->listWidget->currentRow());
}

void ObjectNameEditor::on_btnAdd_clicked()
{
    QListWidgetItem *item = new QListWidgetItem(tr("(null)"));
    item->setFlags(item->flags() | Qt::ItemIsEditable);
    ui->listWidget->addItem(item);
    ui->listWidget->setCurrentItem(item);
}
