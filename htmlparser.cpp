#include "htmlparser.h"
#define debuging

HTMLParser * HTMLParser::mInstance = nullptr;

HTMLParser *HTMLParser::CreateParser()
{
   if (mInstance == nullptr) mInstance = new HTMLParser();
   return mInstance;
}

int HTMLParser::GetNodeWithClassName(const QGumboNodes &nodes , QString const &classname)
{
    for (const auto& node: nodes)
    {
      qDebug()<<node.innerText();
      if (node.classList().contains(classname))
          return 1;
      GetNodeWithClassName(node.children(),classname);
    }
    return 0;
}

HTMLParser::HTMLParser(QObject *parent) : QObject(parent)
{
  mDownLoader = HTMLDonwloader::CreateDonwloader();
  mProdCards = new Cards();
  connect(mDownLoader,&HTMLDonwloader::HTMlReady,this,&HTMLParser::ParseHtmlPage);
}

HTMLParser::~HTMLParser()
{
  delete mDownLoader;
  delete mProdCards;
  delete mInstance;
}

QString HTMLParser::GetSerchUrlForSite(Sites site , QString const &SearchQuery)
{
    QString SearchLink= "";

    switch (site)
    {
    case Sites::JUMIA :
        SearchLink = "https://www.jumia.ma/catalog/?q=" + SearchQuery;
        break;
    case Sites::MENAGERE :
        SearchLink = "https://menagere.ma/jolisearch?s=" + SearchQuery;
      break;
    case Sites::ELECBOUSFIHA :
        SearchLink = "https://electrobousfiha.com/recherche?cat_id=all&controller=search&s=" + SearchQuery;
      break;
    case Sites::ELECTOPLANET :
        SearchLink = "https://www.electroplanet.ma/recherche?q=" + SearchQuery;
      break;
    case Sites::ELECTROSERGH :
        SearchLink = "http://electroserghini.com/recherche?controller=search&orderby=position&orderway=desc&search_query=" + SearchQuery;
      break;
    case Sites::TABTEL :
        SearchLink = "https://tabtel.ma/fr/recherche?controller=search&search_query=" + SearchQuery;
      break;
    case Sites::IRIS :
        SearchLink = "https://www.iris.ma/recherche?controller=search&&search_query=" + SearchQuery;
      break;
    case Sites::MSERI :
        SearchLink = "https://www.maseri.ma/recherche?controller=search&s=" + SearchQuery;
      break;
    }
    return  SearchLink;
}

void HTMLParser::Search(const QString &SearchText)
{
    mSerchtext = SearchText;
    mDownLoader->LoadUrl(GetSerchUrlForSite(Sites::JUMIA,SearchText),Sites::JUMIA);
}

void HTMLParser::ParseHtmlPage(const QString &page, const Sites &selected)
{
    /* init */
    QGumboNodes PrdCards;
    ProdctCardData ProdCardData;
    /* init end */
    auto doc = QGumboDocument::parse(page);
    mRootNode = doc.rootNode();

    switch (selected)
    {
    case Sites::JUMIA :
        PrdCards = mRootNode.getElementsByClassName("info");
        for (const auto& card: PrdCards)
        {
         ProdCardData.marque = card.getElementsByClassName("name").at(0).innerText();
         ProdCardData.model = ProdCardData.marque;
         ProdCardData.price = card.getElementsByClassName("prc").at(0).innerText().trimmed().simplified().replace(" ","").replace(",",".");
         ProdCardData.origin = selected;
         ProdCardData.fprice = ProdCardData.price.replace("MAD","").replace("Dhs","").replace("Dh","").toFloat();
         CalculateSiteMeanPrice(ProdCardData.fprice,Operation::ADD);
         mProdCards->push_back(ProdCardData);
        }
        mMeans.push_back(CalculateSiteMeanPrice(0.0f,Operation::MEAN));
        mDownLoader->LoadUrl(GetSerchUrlForSite(Sites::ELECTOPLANET,mSerchtext),Sites::ELECTOPLANET);
        break;
    case Sites::MENAGERE :
        PrdCards = mRootNode.getElementsByClassName("product-container");
        for (const auto& card: PrdCards)
        {
         ProdCardData.marque = card.getElementsByClassName("product-name").at(0).innerText();
         ProdCardData.model = ProdCardData.marque;
         ProdCardData.price = card.getElementsByClassName("price").at(0).innerText().trimmed().replace(QChar(160),QChar(32)).simplified().replace(" ","").replace(",",".");
         ProdCardData.origin = selected;
         ProdCardData.fprice = ProdCardData.price.replace("MAD","").replace("Dhs","").replace("Dh","").toFloat();
         CalculateSiteMeanPrice(ProdCardData.fprice,Operation::ADD);
         mProdCards->push_back(ProdCardData);
        }
        mMeans.push_back(CalculateSiteMeanPrice(0.0f,Operation::MEAN));
        mDownLoader->LoadUrl(GetSerchUrlForSite(Sites::TABTEL,mSerchtext),Sites::TABTEL);
      break;
    case Sites::MSERI :
        PrdCards = mRootNode.getElementsByClassName("product-miniature");
        for (const auto& card: PrdCards)
        {
         ProdCardData.marque = card.getElementsByClassName("product-name").at(0).getElementsByTagName(HtmlTag::A).at(0).innerText().trimmed();
         ProdCardData.model = ProdCardData.marque;
         ProdCardData.price = card.getElementsByClassName("product-price").at(0).innerText().trimmed().replace(QChar(160),QChar(32)).simplified().replace(" ","").replace(",",".");
         ProdCardData.origin = selected;
         ProdCardData.fprice = ProdCardData.price.replace("MAD","").replace("Dhs","").replace("Dh","").toFloat();
         CalculateSiteMeanPrice(ProdCardData.fprice,Operation::ADD);
         mProdCards->push_back(ProdCardData);
        }
        mMeans.push_back(CalculateSiteMeanPrice(0.0f,Operation::MEAN));
        CleanProducts();
        //mDownLoader->LoadUrl(GetSerchUrlForSite(Sites::MENAGERE,mSerchtext),Sites::MENAGERE);
      break;
    case Sites::ELECBOUSFIHA :
        PrdCards = mRootNode.getElementsByClassName("product-info");
        for (const auto& card: PrdCards)
        {
         ProdCardData.marque = card.getElementsByClassName("product-title").at(0).getElementsByTagName(HtmlTag::A).at(0).innerText();
         ProdCardData.model = ProdCardData.marque;
         ProdCardData.price = card.getElementsByClassName("price").at(0).innerText().trimmed().replace(QChar(160),QChar(32)).simplified().replace(" ","").replace(",",".");
         ProdCardData.origin = selected;
         ProdCardData.fprice = ProdCardData.price.replace("MAD","").replace("Dhs","").replace("Dh","").toFloat();
         CalculateSiteMeanPrice(ProdCardData.fprice,Operation::ADD);
         mProdCards->push_back(ProdCardData);
        }
        mMeans.push_back(CalculateSiteMeanPrice(0.0f,Operation::MEAN));
        mDownLoader->LoadUrl(GetSerchUrlForSite(Sites::ELECTROSERGH,mSerchtext),Sites::ELECTROSERGH);
      break;
    case Sites::ELECTOPLANET :
       PrdCards = mRootNode.getElementsByClassName("item-inner");
       for (const auto& card: PrdCards)
       {
        ProdCardData.marque = card.getElementsByClassName("brand").at(0).innerText();
        ProdCardData.model = card.getElementsByClassName("ref").at(1).innerText();
        ProdCardData.price = card.getElementsByClassName("price").at(0).innerText().trimmed().replace(QChar(160),QChar(32)).simplified().replace(" ","").replace(",",".");
        ProdCardData.origin = selected;
        ProdCardData.fprice = ProdCardData.price.replace("MAD","").replace("Dhs","").replace("Dh","").toFloat();
        CalculateSiteMeanPrice(ProdCardData.fprice,Operation::ADD);
        mProdCards->push_back(ProdCardData);
       }
       mMeans.push_back(CalculateSiteMeanPrice(0.0f,Operation::MEAN));
       mDownLoader->LoadUrl(GetSerchUrlForSite(Sites::ELECBOUSFIHA,mSerchtext),Sites::ELECBOUSFIHA);
      break;
    case Sites::ELECTROSERGH :
        PrdCards = mRootNode.getElementsByClassName("pro_second_box");
        for (const auto& card: PrdCards)
        {
         ProdCardData.marque = card.getElementsByClassName("product-name").at(0).innerText();
         ProdCardData.model = ProdCardData.marque;
         ProdCardData.price = card.getElementsByClassName("product-price").at(0).innerText().trimmed().replace(QChar(160),QChar(32)).simplified().replace(" ","").replace(",",".");
         ProdCardData.origin = selected;
         ProdCardData.fprice = ProdCardData.price.replace("MAD","").replace("Dhs","").replace("Dh","").toFloat();
         CalculateSiteMeanPrice(ProdCardData.fprice,Operation::ADD);
         mProdCards->push_back(ProdCardData);
        }
        mMeans.push_back(CalculateSiteMeanPrice(0.0f,Operation::MEAN));
        mDownLoader->LoadUrl(GetSerchUrlForSite(Sites::MENAGERE,mSerchtext),Sites::MENAGERE);
      break;
    case Sites::TABTEL :
        PrdCards = mRootNode.getElementsByClassName("right-block");
        for (const auto& card: PrdCards)
        {
         ProdCardData.marque = card.getElementsByClassName("product-name").at(0).innerText().trimmed();
         ProdCardData.model = ProdCardData.marque;
         ProdCardData.price = card.getElementsByClassName("product-price").at(0).innerText().trimmed().simplified().replace(" ","").replace(",",".");
         ProdCardData.origin = selected;
         ProdCardData.fprice = ProdCardData.price.replace("MAD","").replace("Dhs","").replace("Dh","").toFloat();
         CalculateSiteMeanPrice(ProdCardData.fprice,Operation::ADD);
         mProdCards->push_back(ProdCardData);
        }
        mMeans.push_back(CalculateSiteMeanPrice(0.0f,Operation::MEAN));
        mDownLoader->LoadUrl(GetSerchUrlForSite(Sites::IRIS,mSerchtext),Sites::IRIS);
      break;
    case Sites::IRIS :
        PrdCards = mRootNode.getElementsByClassName("center-block");
        for (const auto& card: PrdCards)
        {
         ProdCardData.marque = card.getElementsByClassName("product-name").at(0).innerText().trimmed();
         ProdCardData.model = ProdCardData.marque;
         ProdCardData.price = card.getElementsByClassName("product-price").at(0).innerText().trimmed().simplified().replace(" ","").replace(",",".");
         ProdCardData.origin = selected;
         ProdCardData.fprice = ProdCardData.price.replace("MAD","").replace("Dhs","").replace("Dh","").toFloat();
         CalculateSiteMeanPrice(ProdCardData.fprice,Operation::ADD);
         mProdCards->push_back(ProdCardData);
        }
        mMeans.push_back(CalculateSiteMeanPrice(0.0f,Operation::MEAN));
        mDownLoader->LoadUrl(GetSerchUrlForSite(Sites::MSERI,mSerchtext),Sites::MSERI);
      break;
    }
}

float HTMLParser::CalculateSiteMeanPrice(const float &Fprice , const Operation op)
{
  float numberPrice = 0.0f;
  int size = 0;

  if (op == Operation::ADD)
  {
      mPrices.push_back(Fprice);
  }
  else if ( op == Operation::MEAN)
  {
    size = mPrices.size();
    for(const auto &price : mPrices)
    {
        numberPrice += price;
    }
    mPrices.clear();
    if (size > 0) return numberPrice/size;
  }
  return numberPrice;
}

void HTMLParser::CleanProducts()
{
  float globalmean = 0.0f;
  int i = mMeans.size();
   /* calculating global mean*/
  for (const auto &mean : mMeans)
  {
   if (mean == 0.0f) i--;
   globalmean += mean;
  }
  globalmean = globalmean /i ;
   /* cleaning the prods from undesired products */
  for (int i(0);i<mProdCards->size();i++)
  {
    if (mProdCards->at(i).fprice > static_cast<float>(globalmean * 1.2f) || mProdCards->at(i).fprice < static_cast<float>(globalmean * 0.8f) )
    {
       qDebug()<<mProdCards->at(i).fprice;
       mProdCards->removeAt(i);
    }
  }

#ifdef debuging
       for (int i(0);i<mProdCards->size();i++)
       {
         qDebug()<<"model : "<<mProdCards->at(i).model<<"price : "<<mProdCards->at(i).price<<"marque : "<<mProdCards->at(i).marque;
       }
#endif
}
