#include <QCoreApplication>
#include "htmlparser.h"

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    HTMLParser *prs = HTMLParser::CreateParser();
    prs->Search("samsung a20");
    return a.exec();
}
