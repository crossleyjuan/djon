// *********************************************************************************************************************
// file:    import.cpp
// author:  Juan Pablo Crossley (crossleyjuan@gmail.com)
// created: Aug/9/2010
// updated: Nov/7/2011
// license: GPL 2
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

#include "import.h"
#include "xmlerrorhandler.h"
#include <QXmlSimpleReader>
#include <QMessageBox>

std::vector<Project*>* import(Template* defaultTemplate, string* defaultStatus, string* fileName, IMPORT_TYPE fileType) {
    AllneticImportHandler* handler = NULL;
    if (fileType == ALLNETIC_FILE) {
        handler = new AllneticImportHandler(defaultTemplate, defaultStatus);
    } else {
        return NULL;
    }
    QXmlSimpleReader reader;
    QFile* file = new QFile(QString(fileName->c_str()));
    QXmlInputSource* source = new QXmlInputSource(file);
    reader.setContentHandler(handler);
    XMLErrorHandler* error = new XMLErrorHandler();
    reader.setErrorHandler(error);
    if (reader.parse(source)) {
        return handler->projects();
    } else {
        QMessageBox box;
        box.setText(error->errorString());
        box.exec();
        return NULL;
    }
}
