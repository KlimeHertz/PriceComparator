#pragma once

#include <QObject>
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QDebug>

enum class Sites{JUMIA ,ELECBOUSFIHA ,MENAGERE ,TABTEL ,IRIS ,MSERI  ,ELECTROSERGH,ELECTOPLANET};

class HTMLDonwloader : public QObject
{
    Q_OBJECT
public:
    explicit HTMLDonwloader(QObject *parent = nullptr);
    ~HTMLDonwloader();
    static HTMLDonwloader* CreateDonwloader();
    void LoadUrl (QString const &Url , Sites const &selectedSite);

signals:
    void HTMlReady(QString const &HtmlBody , Sites const &selected);

public slots:
    void DomReady(QNetworkReply *Replay);

private:
    static HTMLDonwloader *mInstance;
    QNetworkAccessManager *mAccesManager;
    Sites mSelectedSite;
};

