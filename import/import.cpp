#include "import.h"
#include "xmlerrorhanlder.h"
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
