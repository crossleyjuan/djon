#include "import.h"
#include <QXmlSimpleReader>

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
    if (reader.parse(source)) {
        return handler->projects();
    } else {
        return NULL;
    }
}
