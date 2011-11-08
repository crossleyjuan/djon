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

#include "excel.h"
#include "util.h"
#include <stdlib.h>
#include <sstream>
#include <QFile>
#include <QByteArray>

Excel::Excel()
{
}

Excel::~Excel() {
}

ExcelCell::ExcelCell() {
    _col = -1;
    _row = -1;
    _type = STRING;
    _value = NULL;
}

ExcelCell::ExcelCell(const ExcelCell& orig) {
    _col = orig._col;
    _row = orig._row;
    _type = orig._type;
    _value = NULL;
    if (orig._value != NULL) {
        switch (_type) {
        case STRING:
            _value = strdup((char*)orig._value);
            break;
        case INT:
            _value = (int*)malloc(sizeof(int));
            memcpy(_value, orig._value, sizeof(int));
            break;
        }
    }
}

ExcelCell::~ExcelCell() {
    if (_value != NULL) {
//        free (_value);
    }
}

ExcelCell::ExcelCell(int row, int col) {
    _row = row;
    _col = col;
}

ExcelCell& ExcelCell::operator=(int value) {
    _type = INT;
    _value = malloc(sizeof(int));
    memcpy(_value, &value, sizeof(int));
    return *this;
}

ExcelCell& ExcelCell::operator=(const char* value) {
    _type = STRING;
    _value = strdup(value);
    return *this;
}

ExcelCell& Excel::operator ()(unsigned row, unsigned col) {
    if (_rows.size() < (row+1)) {
        std::vector< ExcelCell > row;
        _rows.push_back(row);
    }
    while (_rows[row].size () <= col) {
      ExcelCell v;

      _rows[row].push_back (v);       /* add any extra columns */
    }
    return _rows[row][col];
}

ExcelCell::operator int() {
    if (_value != NULL) {
        int* val = (int*)_value;
        return *val;
    } else {
        return 0;
    }
}

ExcelCell::operator char*() {
    return (char*)_value;
}

CELL_TYPE ExcelCell::type() {
    return _type;
}

char* removeInvalidCharacters(char* c) {
    char* res = (char*)malloc(strlen(c) + 1);
    memset(res, 0, strlen(c) + 1);
    for (int x = 0; x < strlen(c); x++) {
        res[x] = c[x];
        if (!isalpha(res[x])) {
            res[x] = ' ';
        }
    }
    return res;
}

void Excel::write() {
    QFile sheetFile(":/img/sheet.xml");
    sheetFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString sheetTemplate(sheetFile.readAll());
    sheetFile.close();

    QFile sharedFile(":/img/sharedStrings.xml");
    sharedFile.open(QIODevice::ReadOnly | QIODevice::Text);
    QString sharedTemplate(sharedFile.readAll());
    sharedFile.close();

    int row = 6;
    std::stringstream sharedStrings;
    std::stringstream sheet;
    int sharedStringIndex = 4;
    for (vector< vector<ExcelCell> >::iterator iterRow = _rows.begin(); iterRow != _rows.end(); iterRow++ ) {
        int col = 'A';
        vector<ExcelCell> cols = *iterRow;
        sheet << "<row r=\"" << row << "\" spans=\"1:3\" x14ac:dyDescent=\"0.25\">\n";
        for (vector<ExcelCell>::iterator iterCol = cols.begin(); iterCol != cols.end(); iterCol++) {
            ExcelCell cell = *iterCol;
            if (cell.type() == STRING) {
                char* val = (char*)cell;
                if (val != NULL) {
                    val = removeInvalidCharacters(val);
                    sharedStrings << "<si><t>" << val << "</t></si>\n";
                    sheet << "<c r=\"" << (char)col << row << "\" s=\"0\" t=\"s\">\n";
                    sheet << "<v>" << sharedStringIndex++ << "</v>";
                    sheet << "</c>";
                }
            } else {
                sheet << "<c r=\"" << (char)col << row << "\" s=\"3\">\n";
                int val = cell;
                sheet << "<v>" << val << "</v>";
                sheet << "</c>";
            }
            col++;
        }
        sheet << "</row>\n";
        row++;
    }

    std::string shared = sharedStrings.str();
    std::string ssheet = sheet.str();

    QString sharedResult = sharedTemplate.replace(QString("<<<SharedStrings>>>"), QString(shared.c_str()));
    QString sheetResult = sheetTemplate.replace(QString("<<<Rows>>>"), QString(ssheet.c_str()));

    writeFile("C:/workspace/personal/djon/MiTemplate/xl/sharedStrings.xml", sharedResult.toStdString().c_str(), false);
    writeFile("C:/workspace/personal/djon/MiTemplate/xl/worksheets/sheet1.xml", sheetResult.toStdString().c_str(), false);


}
