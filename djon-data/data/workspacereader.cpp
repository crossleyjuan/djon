// *********************************************************************************************************************
// file:
// author: Juan Pablo Crossley (crossleyjuan@gmail.com)
// created:
// updated:
// license:
// 
// This file is part of the djon project, all the source code is licensed under the terms of the GPL license,
// the application and libraries are provided as-is and free of use under the terms explained in the file COPYING.GPL
// the authors are not responsible for any problem that this program, or any of its libraries, may cause.
// Its authors create this application in order to make the world a better place to live, but you should use it on
// your own risks.
// 
// Also, be adviced that, the GPL license force the committers to ensure this application will be free of use, thus
// if you do any modification you will be required to provide it for free unless you use it for personal use (you may 
// charge yourself if you want), bare in mind that you will be required to provide a copy of the license terms that ensures
// this program will be open sourced and all its derivated work will be too.
// *********************************************************************************************************************

#include "workspacereader.h"

#include "projectreader.h"
#include "util.h"
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
