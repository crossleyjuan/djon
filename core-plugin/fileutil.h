#ifndef FILEUTIL_H
#define FILEUTIL_H

#include "core-defs.h"
#include <QList>

class FileUtil
{
public:
    FileUtil();
    void addMapping(Map map);
    Map readMapping(Task* task, QList<CoreProject> projects, QList<CoreTask> tasks);
    CorePluginSettings readSettings();
};

#endif // FILEUTIL_H
