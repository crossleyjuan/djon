#ifndef IMPORT_H
#define IMPORT_H

#include "allneticimporthandler.h"
#include <vector>

enum IMPORT_TYPE {
    ALLNETIC_FILE
};

class Template;

std::vector<Project*>* import(Template* defaultTemplate, string* defaultStatus, string* fileName, IMPORT_TYPE fileType);

#endif // IMPORT_H
