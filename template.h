#ifndef TEMPLATE_H
#define TEMPLATE_H
#include <string>
#include <vector>

using namespace std;

class Template
{
private:
    std::string* _name;
    std::string* _description;
    std::vector<string*>* _statusList;
    std::string* _closedStatus;
    std::vector<string*>* _subTaskList;

public:
    Template(std::string* name, std::string* description, std::vector<string*>* statusList, std::vector<string*>* subTaskList);

    std::string* name();
    std::string* description();
    std::string* closedStatus();
    std::vector<string*>* statusList();
    std::vector<string*>* subTaskList();
};

#endif // TEMPLATE_H
