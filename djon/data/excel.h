#ifndef EXCEL_H
#define EXCEL_H

#include <vector>
#include <string.h>

enum CELL_TYPE {
    STRING,
    INT
};

class ExcelCell {
public:
    ExcelCell();
    ~ExcelCell();
    ExcelCell(int row, int col);
    ExcelCell(const ExcelCell& orig);

    ExcelCell& operator=(int value);
    ExcelCell& operator=(const char* value);
    operator int();
    operator char*();

    CELL_TYPE type();

private:
    int _row;
    int _col;
    void* _value;
    CELL_TYPE _type;
};

class Excel
{
public:
    Excel();
    ~Excel();
    ExcelCell& operator ()(unsigned row, unsigned col);
    void write();

private:
    std::vector< std::vector<ExcelCell> > _rows;
};

#endif // EXCEL_H
