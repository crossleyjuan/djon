#include "workspacereader.h"

#include "projectreader.h"
#include "fileinputstream.h"
#include "debuginputstream.h"
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
            InputStream* stream = new FileInputStream(*fileName, pFile);
#ifndef QT_NO_DEBUG
            FileInputStream* fileStream = (FileInputStream*)stream;
            stream = new DebugInputStream(fileStream);
#endif
            ProjectReader* prReader = new ProjectReader(stream);
            Project* project = prReader->readProject();

            if (project != NULL) {
                workspace->addProject(project);
            }
            delete(prReader);
#ifndef QT_NO_DEBUG
            delete(fileStream);
#endif
            delete(stream);
        }
        delete(fileName);
    }
    return workspace;
}
