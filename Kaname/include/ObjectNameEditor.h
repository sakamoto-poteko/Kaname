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

#ifndef OBJECTNAMEEDITOR_H
#define OBJECTNAMEEDITOR_H

#include <QDialog>

namespace Ui {
class ObjectNameEditor;
}

class ObjectNameEditor : public QDialog
{
    Q_OBJECT

public:
    explicit ObjectNameEditor(QWidget *parent = 0);
    ~ObjectNameEditor();

    QVector<QString> getLabelingObjectNames();
    void setLabelingObjectNames(const QVector<QString> &objs);

private slots:
    void on_btnDel_clicked();

    void on_btnAdd_clicked();

private:
    Ui::ObjectNameEditor *ui;
};

#endif // OBJECTNAMEEDITOR_H
