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
