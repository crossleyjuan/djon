#ifndef WORKSPACEREADER_H
#define WORKSPACEREADER_H

#include "inputstream.h"

class Workspace;

class WorkspaceReader
{
public:
    WorkspaceReader(InputStream* stream);

    Workspace* readWorkspace();

private:
    InputStream* _stream;
};

#endif // WORKSPACEREADER_H
