#ifndef WORKSPACEREADER_H
#define WORKSPACEREADER_H

class Workspace;
class InputStream;

class WorkspaceReader
{
public:
    WorkspaceReader(InputStream* stream);

    Workspace* readWorkspace();

private:
    InputStream* _stream;
};

#endif // WORKSPACEREADER_H
