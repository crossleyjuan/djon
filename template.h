#ifndef TEMPLATE_H
#define TEMPLATE_H
#include <string>
#include <vector>

using namespace std;

class Template
{
private:
    std::string m_name;
    std::vector<string>* m_statusList;
    std::vector<string>* m_jobList;

public:
    Template(std::string name, std::vector<string>* statusList, std::vector<string>* jobList);

    std::string name();
    std::vector<string>* statusList();
    std::vector<string>* jobList();
};

#endif // TEMPLATE_H
