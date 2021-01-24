#include "htmldonwloader.h"

HTMLDonwloader * HTMLDonwloader::mInstance = nullptr;

HTMLDonwloader *HTMLDonwloader::CreateDonwloader()
{
  if (mInstance == nullptr) mInstance = new HTMLDonwloader();
   return  mInstance;
}

HTMLDonwloader::HTMLDonwloader(QObject *parent) : QObject(parent)
{
 mAccesManager = new QNetworkAccessManager(this);
 connect(mAccesManager,&QNetworkAccessManager::finished,this,&HTMLDonwloader::DomReady);
}

HTMLDonwloader::~HTMLDonwloader()
{
    delete mAccesManager;
}


void HTMLDonwloader::LoadUrl(const QString &Url , const Sites &selectedSite)
{
 QNetworkRequest Req = QNetworkRequest(QUrl(Url));
 mSelectedSite = selectedSite;
 Req.setSslConfiguration(QSslConfiguration::defaultConfiguration());
 mAccesManager->get(Req);
}

void HTMLDonwloader::DomReady(QNetworkReply *Replay)
{
  QByteArray HtmlArray = Replay->readAll();
  QString HtmlText = "";
   HtmlText = QString::fromUtf8(HtmlArray.data(),HtmlArray.size());
  qDebug()<<Replay->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt();
  //qDebug()<<HtmlText;
  emit HTMlReady(HtmlText, mSelectedSite);
}
