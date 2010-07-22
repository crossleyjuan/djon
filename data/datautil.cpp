#include "datautil.h"
#include "util.h"

#include <map>
#include <sstream>
#include <string.h>

using namespace std;

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

hashmap* parseTextFormat(std::string text) {
    hashmap* mapValue = new hashmap();
    string name;
    string value;
    stringstream in(text);
    char* test = NULL;
    while (in.good()) {
        string element;
        readElement(element, in);
        if (element.length() > 0) {
            string name = element.substr(0, element.find_first_of(":"));
            string value = element.substr(element.find_first_of(":") + 1, element.length() - element.find_first_of(":"));
            while (value.find_first_not_of(" ") > 0) {
                value = value.erase(0, 1);
            }
            mapValue->insert(pair<string, string > (name, value));
        }
    }
    return mapValue;
}