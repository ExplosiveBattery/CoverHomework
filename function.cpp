#include <QSqlQuery>
#include <QSqlError>
//#include <QDebug>
#include <QTextCodec>
#include <QRegExp>

QTextCodec *gbk = QTextCodec::codecForName("GB18030");

bool SQLErrorDetect (QSqlQuery *pQuery, const QString strCommand,const char *info) {
    if (!pQuery->exec(strCommand)){
        QSqlError lastError =pQuery->lastError();
//        qDebug() << info  << " : "<< lastError.driverText();
        return false;
    }
     return true;
}

QString gbkURLCoding(QString str)
{
    QString result ="";
//    QByteArray data =gbk->fromUnicode(str);
    QByteArray data;data.append(str);
    data =data.toPercentEncoding();
    return data;
}


//作用：匹配返回正则表达式中第capPos个(...)，同时第四个惨呼指定了过滤
QStringList findAll(const QString text, const QString regexp, int capPos, const QString removeRules) {
    QStringList resultSet;
    QRegExp rx(regexp);rx.setMinimal(true);
    QRegExp rxRemove(removeRules);rxRemove.setMinimal(true);
    int pos = 0;
    while ((pos = rx.indexIn(text, pos)) != -1){
        pos += rx.matchedLength();
        QString result =(removeRules=="")? rx.cap(capPos) : rx.cap(capPos).remove(rxRemove);
        resultSet << result;
    }
    return resultSet;
}
