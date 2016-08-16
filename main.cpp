/**************************************************************************
 * Copyright (c) 2016 Afa.L Cheng <afa@afa.moe>
 ***************************************************************************/

#include "Kaname.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    Kaname w;
    w.show();

    return a.exec();
}
