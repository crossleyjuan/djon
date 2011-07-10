#include "workspacereader.h"

#include "projectreader.h"
#include "inputstream.h"
#include "workspace.h"
#include <string>

WorkspaceReader::WorkspaceReader(InputStream* stream)
{
    _stream = stream;
}

Workspace* WorkspaceReader::readWorkspace() {
    int projects =_stream->readInt();

    Workspace* workspace = new Workspace(_stream->fileName());
    for (int x = 0; x < projects; x++) {
        std::string* fileName = _stream->readString();

        FILE* pFile;

        pFile = fopen(fileName->c_str(), "rb");
        if (pFile != NULL) {
            InputStream* stream = new InputStream(*fileName, pFile);

            ProjectReader* prReader = new ProjectReader(stream);
            Project* project = prReader->readProject();

            workspace->addProject(project);

            delete(prReader);
            delete(stream);
        }
        delete(fileName);
    }
    return workspace;
}
