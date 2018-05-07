#include "NetworkSystem.h"


NetworkSystem::NetworkSystem(QString username,QString password):
    m_username(username),m_password(password),m_boundary("-----------------------------7e12873b507e0"),m_baseUrl("http://cc.scu.edu.cn"),
    m_pCookieJar(new QNetworkCookieJar)
{
    setCookieJar(m_pCookieJar);
    m_request.setHeader(QNetworkRequest::UserAgentHeader, "Mozilla/5.0 AppleWebKit/537.36 (KHTML, like Gecko) Chrome/59.0.3053.3 Safari/537.36");
    connect(this, SIGNAL(finished(QNetworkReply*)), this, SLOT(replyComplished(QNetworkReply*)), Qt::UniqueConnection);

}

void NetworkSystem::setFileName(QString fileName) {
    m_fileName =fileName;
}

void NetworkSystem::setNewFileName(QString newFileName) {
    m_newFileName =newFileName;
}


bool NetworkSystem::login() {
    m_request.setUrl(m_baseUrl+"/G2S/ShowSystem/Login.ashx");
    m_request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");
    QByteArray data;
    data.append("LoginName="+m_username+
                "&Password="+m_password+
                "&isAutoLogin=0"
                "&isRemember=0"
                "&action=GetUserLoginInfo");

    QNetworkReply *pReply =post(m_request,data);
    QEventLoop loop;
    connect(pReply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    return pReply->error()==QNetworkReply::NoError;
}

bool NetworkSystem::upload() {
    m_request.setUrl(m_baseUrl+"/G2S/eWebEditor/aspx/upload.aspx?action=save&type=file&style=mini&cusdir=&skey=");
    m_request.setHeader(QNetworkRequest::ContentTypeHeader,"multipart/form-data; boundary="+m_boundary);

    QFile file(m_fileName);

    if(file.exists()) {
        if( !file.open(QIODevice::ReadOnly)) {
            QMessageBox::warning(nullptr, "warning", "打开传输的文件失败", QMessageBox::Yes);
            return false;
        }
        if(file.size()>=51200000) {
            QMessageBox::warning(nullptr, "warning", "文件超过限制的大小（48MB）", QMessageBox::Yes);
            return false;
        }
    }else {
        QMessageBox::warning(nullptr, "warning", "目标文件不存在", QMessageBox::Yes);
        return false;
    }
    QByteArray fileContent = file.readAll();

    QByteArray data;
    data.append("--"+m_boundary+"\r\n"+
                "Content-Disposition: form-data; name=\"MAX_FILE_SIZE\"\r\n\r\n51200000\r\n"+
                "--"+m_boundary+"\r\n"+
                "Content-Disposition: form-data; name=\"uploadfile\"; filename=\""+m_fileName+"\"\r\nContent-Type: application/octet-stream\r\n\r\n");
    data.append(fileContent);
    data.append("\r\n--"+m_boundary+"\r\n"+
                "Content-Disposition: form-data; name=\"originalfile\"\r\n\r\n"+m_fileName+"\r\n"
                "--"+m_boundary+"--");
    QNetworkReply *pReply =post(m_request,data);
    QEventLoop loop;
    connect(pReply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();

    return true;
}

void NetworkSystem::getNecessaryInfo(QString infoUrl) {
    m_request.setUrl(infoUrl);
    m_request.setHeader(QNetworkRequest::ContentTypeHeader,QVariant());

    QNetworkReply *pReply =get(m_request);
    QEventLoop loop;
    connect(pReply, SIGNAL(finished()), &loop, SLOT(quit()));
    loop.exec();
}

void NetworkSystem::deposit() {
    m_request.setUrl(m_baseUrl+"/G2S/StudentSpace/Homework/HomeworkService.ashx");
    m_request.setHeader(QNetworkRequest::ContentTypeHeader,"application/x-www-form-urlencoded");

    QByteArray data;
    data.append("OptType=SaveStudentHomework"
                "&answerinfo="
                "&HomeWorkID="+m_homeworkID+
                "&UserID="+m_userID+
                "&SAnswer="+m_SAnwer+
                "&stype=0");
    post(m_request, data);
}

void NetworkSystem::cover() {
    m_request.setUrl(m_baseUrl+"/G2S/StudentSpace/Exam/ExamService.ashx");

    QByteArray data;
    data.append("OptType=SubmitExam"
                "&info="
                "&ID="+m_homeworkID+
                "&uid="+m_userID);
    post(m_request, data);
}

void NetworkSystem::replyComplished(QNetworkReply* pReply) {
    QNetworkRequest request =pReply->request();
    QString str =gbk->toUnicode(pReply->readAll());

    QString url =request.url().toString();
    if( url.contains("Login.ashx") ) {

    }else if( url.contains("upload.aspx") ) {
        recordUploadInfo(str);
    }else if( url.contains("Dohomework") ) {
        recordNessaryInfo(str);
    }else if( url.contains("HomeworkService.ashx") ) {
//        qDebug() << "deposite:" << pReply->error();
    }else if( url.contains("ExamService.ashx") ) {
//        qDebug() << "cover:" << pReply->error();
    }
}

void NetworkSystem::recordUploadInfo(QString &str) {
    QString result =findAll(str, "'/G2S/eWebEditor/uploadfile/(.*)'", 1)[0];
    m_SAnwer =gbkURLCoding("<P><IMG border=0 src=\"/G2S/eWebEditor/sysimage/icon16/zip.gif\"><A target=_blank href=\"/G2S/eWebEditor/uploadfile/"+result+"\">"+m_newFileName+"</A></P>");
}


void NetworkSystem::recordNessaryInfo(QString &str) {
    m_homeworkID =findAll(str, "var HomeWorkID = \"(.*)\"", 1)[0];
    m_userID =findAll(str, "var UserID = \"(.*)\"", 1)[0];
}


