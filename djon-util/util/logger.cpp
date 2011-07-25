#include "logger.h"
#include "stringfunctions.h"

#include <iostream>
#include <stdarg.h>
#define PRINT(TYPE, CLAZZ, MESSAGE) \
    cout << TYPE << ": " << CLAZZ << ": " << MESSAGE << endl;

Logger* getLogger(void* clazz) {
    Logger* logger = new Logger(clazz);

    return logger;
}

Logger::Logger(void* clazz) {
    m_clazz = clazz;
    m_debug = true;
    m_info = true;
    m_warn = true;
}

void Logger::debug(string message) {
    PRINT("DEBUG", m_clazz, message);
}

void Logger::info(string message) {
    PRINT("INFO", m_clazz, message);
}


void Logger::warn(string message) {
    PRINT("WARN", m_clazz, message);
}


void Logger::error(string message) {
    cout << m_clazz << ":" << message << endl;
}

void Logger::error(exception ex) {
    cout << m_clazz << ":" << ex.what() << endl;
}

bool Logger::isDebug() {
    return m_debug;
}

bool Logger::isInfo() {
    return m_info;
}

bool Logger::isWarn() {
    return m_warn;
}
