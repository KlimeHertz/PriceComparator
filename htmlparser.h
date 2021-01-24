#ifndef HTMLPARSER_H
#define HTMLPARSER_H

#include <QObject>
#include "qgumbonode.h"
#include "qgumbodocument.h"
#include "qgumboattribute.h"
#include "htmldonwloader.h"
#include <QUrl>
#include <QVector>

enum class Operation {MEAN, ADD};

struct ProdctCardData
{
   QString marque;
   QString price;
   float fprice;
   QString model;
   Sites origin;
};

typedef QVector<ProdctCardData> Cards;

class HTMLParser : public QObject
{
    Q_OBJECT
public:
    explicit HTMLParser(QObject *parent = nullptr);
    ~HTMLParser();
    QString GetSerchUrlForSite(Sites site, const QString &SearchQuery);
    void Search(QString const &SearchText);
    static HTMLParser *CreateParser();
    int GetNodeWithClassName(QGumboNodes const &nodes, const QString &classname);
signals:

public slots:
    void ParseHtmlPage (QString const &page , Sites const &selected);

private :
    static HTMLParser* mInstance;
    QGumboNode mRootNode;
    HTMLDonwloader *mDownLoader;
    Cards *mProdCards;
    QString mSerchtext;
    QVector<float> mPrices;
    QVector<float> mMeans;

private :
    float CalculateSiteMeanPrice(const float &Fprice, const Operation op);
    void CleanProducts();
};

#endif // HTMLPARSER_H
