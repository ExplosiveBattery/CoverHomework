#ifndef NETWORKSYSTEM_H
#define NETWORKSYSTEM_H

#include <QString>
//Network
#include <QNetworkAccessManager>
#include <QNetworkRequest>
#include <QNetworkCookieJar>
#include <QNetworkReply>
#include <QEventLoop>
#include <QTextCodec>
//MessageBox
#include <QMessageBox>
//File IO
#include <QFile>

extern QTextCodec *gbk;

QString gbkURLCoding(QString str);
QStringList findAll(const QString text, const QString regexp, int capPos, const QString removeRules="");

class NetworkSystem: public QNetworkAccessManager
{
    Q_OBJECT
public:
    NetworkSystem(QString username,QString password);
    void setFileName(QString fileName);
    void setNewFileName(QString newFileName);
    bool login(void);       //登录
    void getNecessaryInfo(QString infoUrl);//获取必要信息
    bool upload(void);      //上传
    void deposit(void);     //暂存（暂存上传内容）
    void cover(void);       //提交（使用暂存的内容来提交）
protected slots:
    void replyComplished(QNetworkReply* pReply);
private:
    void recordNessaryInfo(QString&);
    void recordUploadInfo(QString &str);
private:
    //Necessary info
    QString m_boundary;
    QString m_fileName, m_newFileName;
    QString m_username, m_password;
    QString m_homeworkID, m_userID, m_SAnwer;
    //Network operator
    QString m_baseUrl;
    QNetworkRequest m_request;
    QNetworkCookieJar *m_pCookieJar;
};

#endif // NETWORKSYSTEM_H
