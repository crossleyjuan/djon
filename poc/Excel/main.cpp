// *********************************************************************************************************************
// file:
// author: Juan Pablo Crossley (crossleyjuan@gmail.com)
// created:
// updated:
// license:
// 
// This file is part of the djon project, all the source code is licensed under the terms of the GPL license,
// the application and libraries are provided as-is and free of use under the terms explained in the file COPYING.GPL
// the authors are not responsible for any problem that this program, or any of its libraries, may cause.
// Its authors create this application in order to make the world a better place to live, but you should use it on
// your own risks.
// 
// Also, be adviced that, the GPL license force the committers to ensure this application will be free of use, thus
// if you do any modification you will be required to provide it for free unless you use it for personal use (you may 
// charge yourself if you want), bare in mind that you will be required to provide a copy of the license terms that ensures
// this program will be open sourced and all its derivated work will be too.
// *********************************************************************************************************************

#include <QtCore/QCoreApplication>
#include "excel.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);

    FILE *f = fopen ("/home/cross/temp/salida.xls", "wb");

    Excel miniexcel;

    miniexcel(0,0) = "Item1:";
    miniexcel(1,0) = "Item2:";
    miniexcel(2,0) = "Sum = ";
    miniexcel(2,0).setBorder(BORDER_LEFT | BORDER_TOP | BORDER_BOTTOM);
    miniexcel(2,0).setAlignament(ALIGN_CENTER);


    miniexcel(0,1) = 10;
    miniexcel(1,1) = 20;
    miniexcel(1,1).setShaded(true);
    miniexcel(2,1) = (double)miniexcel(0,1) + (double)miniexcel(1,1);
    miniexcel(2,1).setBorder(BORDER_RIGHT | BORDER_TOP | BORDER_BOTTOM);

    miniexcel.Write(f);


    return 0;
}
