/**************************************************************************
 * Copyright (c) 2016 Afa.L Cheng <afa@afa.moe>
 ***************************************************************************/

#include "Kaname.h"
#include "ui_Kaname.h"

Kaname::Kaname(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Kaname)
{
    ui->setupUi(this);
}

Kaname::~Kaname()
{
    delete ui;
}
