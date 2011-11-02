#include "workspacewriter.h"
#include "util.h"
#include "workspace.h"

#include <vector>
#include <project.h>

WorkspaceWriter::WorkspaceWriter(OutputStream* stream)
{
    _stream = stream;
}

void WorkspaceWriter::writeWorkspace(Workspace *workspace) {
    const std::vector<Project*>* projects = workspace->projects();
    _stream->writeInt(projects->size());

    for (std::vector<Project*>::const_iterator iter = projects->begin(); iter != projects->end(); iter++) {
        Project* project = *iter;
        _stream->writeString(project->projectFileName());
    }
}
