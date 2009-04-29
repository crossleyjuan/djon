#include "fileutils.h"
#include <fstream>
#include <sstream>

int getdir (string dir, vector<string> &files, string extension)
{
    DIR *dp;
    struct dirent *dirp;
    if((dp  = opendir(dir.c_str())) == NULL) {
        cout << "Error(" << errno << ") opening " << dir << endl;
        return errno;
    }

    string fileextension = "." + extension;

    while ((dirp = readdir(dp)) != NULL) {
        string currentFile (dirp->d_name);
        if (currentFile.find(fileextension) != string::npos) {
            files.push_back(currentFile);
        }
    }
    closedir(dp);
    return 0;
}

void readElement(string& readed, stringstream &str) {

    int c;
    do {
        c = str.get();
        if (c != ';') {
            if ((readed.length() != 0) || ((c != 13) && (c != 10))) {
                if (str.good()) {
                    char str[2];
                    str[0] = c;
                    str[1] = '\0';
                    readed += str;
                } else {
                    break;
                }
            }
        }
    } while (c != ';');
}

hashmap* readFile(string fileName) {
    ifstream ifs;

    ifs.open(fileName.c_str(), ifstream::in);

    stringbuf* buffer = new stringbuf();

    while (ifs.good()) {
        ifs >> buffer;
    }
    ifs.close();

    string str = buffer->str();
    hashmap* mapValue = new hashmap();
    string name;
    string value;
    stringstream in (str);
    while (in.good()) {
        string element;
        readElement(element, in);
        if (element.length() > 0) {
            string name = element.substr(0, element.find_first_of(":"));
            string value = element.substr(element.find_first_of(":") + 1, element.length() - element.find_first_of(":"));
            while (value.find_first_not_of(" ") > 0) {
                value = value.erase(0, 1);
            }
            mapValue->insert(pair<string, string>(name, value));
        }
    }
    return mapValue;
}

