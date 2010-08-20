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

