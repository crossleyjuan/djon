#ifndef LOGGER_H_INCLUDED
#define LOGGER_H_INCLUDED

#include <string>

using namespace std;

class Logger {
    private:
        bool m_debug;
        bool m_info;
        bool m_warn;
        void* m_clazz;

    public:
        Logger(void* clazz);
        bool isDebug();
        bool isInfo();
        bool isWarn();
        void debug(string message);
        void error(string error);
        void error(exception ex);
        void info(string message);
        void warn(string warn);
};

Logger* getLogger(void* clazz);

#endif
