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

#include "Local.h"
#include "util.h"
#include <fstream>
#include <sstream>

using namespace std;

void retrieveLocalFile(std::string outputFile) {
    const char* address = readConfValue("updater-address");

    std::string input(address);

    ifstream ifs;
    ofstream ofs;

    ifs.open(input.c_str(), ifstream::in | ios_base::binary);
    ofs.open(outputFile.c_str(), ios_base::out | ios_base::binary);

    while (!ifs.eof()) {
        stringbuf* buffer = new stringbuf();
        ifs >> buffer;
        ofs << buffer;
    }

    ifs.close();
    ofs.close();
}

