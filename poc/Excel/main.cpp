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
