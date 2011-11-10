// *********************************************************************************************************************
// file:    xmlerrorhandler.cpp
// author:  Juan Pablo Crossley (crossleyjuan@gmail.com)
// created: Aug/9/2010
// updated: Nov/7/2011
// license: GPL 2
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

#include "xmlerrorhandler.h"
#include <QObject>
#include <sstream>
#include <string>

XMLErrorHandler::XMLErrorHandler()
{
    _exception = NULL;
}

bool XMLErrorHandler::warning(const QXmlParseException& exception) {
    _exception = new QXmlParseException(exception);
    return true;
}

bool XMLErrorHandler::error(const QXmlParseException& exception) {
    _exception = new QXmlParseException(exception);
    return true;
}

bool XMLErrorHandler::fatalError(const QXmlParseException& exception) {
    _exception = new QXmlParseException(exception);
    return true;
}

QString XMLErrorHandler::errorString() const {
    std::stringstream ss;
    ss << "Error: ";
    ss << _exception->message().toStdString() << "\n";
    ss << "Column Number:" << _exception->columnNumber() << "\n";
    ss << "Line Number:" << _exception->lineNumber() << "\n";
    ss << "publicId:" << _exception->publicId().toStdString() << "\n";
    ss << "systemId:" << _exception->systemId().toStdString() << "\n";
    std::string errorMessage = ss.str();
    return QString(errorMessage.c_str());
}
