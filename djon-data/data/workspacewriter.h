#ifndef WORKSPACEWRITER_H
#define WORKSPACEWRITER_H

class OutputStream;
class Workspace;

class WorkspaceWriter
{
public:
    WorkspaceWriter(OutputStream* stream);
    void writeWorkspace(Workspace* workspace);

private:

    OutputStream* _stream;
};

#endif // WORKSPACEWRITER_H
