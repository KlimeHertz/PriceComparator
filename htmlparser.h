#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include <QObject>

class HTMLParser : public QObject
{
    Q_OBJECT
public:
    explicit HTMLParser(QObject *parent = nullptr);

signals:

public slots:
};

#endif // HTMLPARSER_H
