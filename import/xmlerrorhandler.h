#ifndef XMLERRORHANDLER_H
#define XMLERRORHANDLER_H

#include <QXmlErrorHandler>

class XMLErrorHandler : public QXmlErrorHandler
{
public:
    explicit XMLErrorHandler();
    virtual bool warning(const QXmlParseException& exception);
    virtual bool error(const QXmlParseException& exception);
    virtual bool fatalError(const QXmlParseException& exception);
    virtual QString errorString() const;

signals:

public slots:

private:
    QXmlParseException* _exception;
};

#endif // XMLERRORHANDLER_H
