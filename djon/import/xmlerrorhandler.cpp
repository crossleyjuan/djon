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
