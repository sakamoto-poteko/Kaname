/**************************************************************************
 * Copyright (c) 2016 Afa.L Cheng <afa@afa.moe>
 ***************************************************************************/

#ifndef KANAME_H
#define KANAME_H

#include <QMainWindow>

namespace Ui {
class Kaname;
}

class Kaname : public QMainWindow
{
    Q_OBJECT

public:
    explicit Kaname(QWidget *parent = 0);
    ~Kaname();

private:
    Ui::Kaname *ui;
};

#endif // KANAME_H
