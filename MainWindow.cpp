#include "MainWindow.h"
#include "ui_MainWindow.h"

MainWindow::MainWindow(QWidget *parent, NetworkSystem *networkSystem) :
    QMainWindow(parent),ui(new Ui::MainWindow),
    m_networkSystem(networkSystem)
{
    ui->setupUi(this);
    ui->lineEdit_file_dialog->setReadOnly(true);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::on_pushButton_file_dialog_clicked()
{
    QString fileName =QFileDialog::getOpenFileName(this,"打 开", "/", "files (*)");
    QString suffix =fileName.split(".",QString::SkipEmptyParts).last();
    if( SUPPORT_SUFFIX.contains(suffix, Qt::CaseInsensitive) ) {
        ui->lineEdit_file_dialog->setText(fileName);
        m_networkSystem->setFileName(fileName);
    }else
        QMessageBox::warning(this, "warning","请选择支持的文件名后缀");
}

void MainWindow::on_pushButton_send_clicked()
{
    QString infoUrl =ui->lineEdit_url->text();
    QString oldFileName =ui->lineEdit_file_dialog->text();
    if( oldFileName.isEmpty() ) {
        QMessageBox::warning(this, "warning", "请选择要发送的文件");
        return;
    }
    QString newFileName =ui->lineEdit_new_file_name->text();
    if(newFileName.isEmpty()) {
        newFileName=oldFileName.split("/",QString::SkipEmptyParts).last();
        ui->lineEdit_new_file_name->setText(newFileName);
    }


    //check url
    m_networkSystem->setNewFileName(newFileName);
    QRegExp rx("http://cc\\.scu\\.edu\\.cn/G2S/StudentSpace/Homework/Dohomework(\\d)?\\.aspx\\?WebID=(\\d)+&ID=(\\d)+");
   if( rx.exactMatch(infoUrl) && m_networkSystem->upload() ) {
        m_networkSystem->login();
        m_networkSystem->getNecessaryInfo(infoUrl);
        m_networkSystem->deposit();
        m_networkSystem->cover();
    }
}
