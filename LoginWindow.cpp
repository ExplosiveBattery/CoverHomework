#include "LoginWindow.h"
#include "ui_LoginWindow.h"
#include <QMessageBox>

LoginWindow::LoginWindow(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::LoginWindow),m_mainWindow(nullptr),m_usernameListWidget(new QListWidget(this)),
    m_drag(false)

{
    initSQL();
    ui->setupUi(this);
    m_usernameListWidget->setGeometry(0,0,0,0);
    setWindowFlags(Qt::FramelessWindowHint|Qt::WindowStaysOnTopHint);
    fillUsernameComboBox();
}

LoginWindow::~LoginWindow()
{
    delete m_networkSystem;
    delete m_mainWindow;
    delete ui;
}


void LoginWindow::initSQL(void) {

    QSqlDatabase db=QSqlDatabase::addDatabase("QSQLITE");
//    QSqlDatabase db=QSqlDatabase::addDatabase("SQLITECIPHER");
    db.setDatabaseName("information.db");
    db.setUserName("sa");
    db.setPassword("asdfjkl;");
    if(!db.open()){
//        qDebug() << "数据库连接失败" << db.lastError().driverText();
        exit(1);
    }
    m_pQuery =new QSqlQuery(db);
    SQLErrorDetect(m_pQuery,
                 "create table  if not exists Users ("
                 "Username varchar,"
                 "Password varchar,"
                 "Frequence int not null default 1 )", "创建用户表失败");
}


void LoginWindow::fillUsernameComboBox() {
    ui->comboBox_username->setEditable(true);
    ui->comboBox_username->lineEdit()->setPlaceholderText("请输入作业系统账号");
    SQLErrorDetect(m_pQuery, "select * from Users order by frequence desc", "获取账号列表失败");
    ui->comboBox_username->setModel(m_usernameListWidget->model());
    ui->comboBox_username->setView(m_usernameListWidget);
    if( m_pQuery->next() ) {
        QString tmpUsername =m_pQuery->value(0).toString();
        ui->lineEdit_password->setText(m_pQuery->value(1).toString());
        do {
            AccountItem *pAccountItem =new AccountItem();
            pAccountItem->setText(m_pQuery->value(0).toString());
            QListWidgetItem* widgetItem = new QListWidgetItem(m_usernameListWidget);
            m_usernameListWidget->setItemWidget(widgetItem, pAccountItem);
            connect( pAccountItem, SIGNAL(deleteAccount(QString)),this, SLOT(deleteAccount(QString)) );
            connect( pAccountItem, SIGNAL(showAccount(QString)),this, SLOT(showAccount(QString)) );
        }while(m_pQuery->next());
        ui->comboBox_username->setCurrentText(tmpUsername);
    }
}
//处理子控件QCombobox发出的show变更信号
void LoginWindow::showAccount(QString username) {
    SQLErrorDetect(m_pQuery, "select password from users where username='"+username+"'", "更改选择框账号时查询密码出错");
    m_pQuery->next();
    ui->comboBox_username->setCurrentText(username);
    ui->lineEdit_password->setText(m_pQuery->value(0).toString());
    ui->comboBox_username->hidePopup();
}
//处理子控件QCombobox发出的delelte删除信号
void LoginWindow::deleteAccount(QString username) {
    SQLErrorDetect(m_pQuery, "delete from users where username='"+username+"'", "删除账号时出错");
    QList<QListWidgetItem*> ItemPointList =m_usernameListWidget->findItems(".*",Qt::MatchRegExp);
    for(int i1=0; i1<ItemPointList.size(); ++i1) {
        AccountItem *pAccountItem =static_cast<AccountItem *>( m_usernameListWidget->itemWidget(ItemPointList[i1]));
        if(pAccountItem->text()==username) {
            delete ItemPointList[i1];
            ItemPointList.removeAt(i1);
            break;
        }
    }

    if(!ItemPointList.isEmpty()) {
        QString username=static_cast<AccountItem *>(m_usernameListWidget->itemWidget(ItemPointList.first()))->text();
        SQLErrorDetect(m_pQuery, "select password from users where username='"+username+"'", "因删除更改选择框账号时查询密码出错");
        m_pQuery->next();
        ui->comboBox_username->setCurrentText(username);
        ui->lineEdit_password->setText(m_pQuery->value(0).toString());
        ui->comboBox_username->showPopup();
    }else{
        ui->lineEdit_password->setText("");
        ui->comboBox_username->hidePopup();
    }

}

//下面三个鼠标响应事件是为了能够对窗口进行拖动
void LoginWindow::mousePressEvent(QMouseEvent *event)
{
    if (event->button() == Qt::LeftButton) {
         m_drag = true;
         m_windowPosition = event->globalPos() - this->pos();
         event->accept();
     }
}

void LoginWindow::mouseMoveEvent(QMouseEvent *event)
{
    if (m_drag && event->buttons() & Qt::LeftButton) {
        move(event->globalPos() - m_windowPosition);
        event->accept();
    }
}

void LoginWindow::mouseReleaseEvent(QMouseEvent *)
{
    m_drag = false;
}


void LoginWindow::on_pushButton_login_clicked()
{
    QString username =ui->comboBox_username->currentText();
    QString password =ui->lineEdit_password->text();
    if( !username.isEmpty() && !password.isEmpty() ) {

        m_networkSystem =new NetworkSystem(username, password);
        if( m_networkSystem->login() ){
            SQLErrorDetect(m_pQuery, "select * from Users where Username='"+username+"'", "用户名匹配超找失败");
            if( !m_pQuery->next() ) SQLErrorDetect(m_pQuery,"insert into Users values('"+username+"','"+password+"',1)", "增加用户账户记录失败");
            else SQLErrorDetect(m_pQuery,"update Users set Password='"+password+"',Frequence=Frequence+1 where Username='"+username+"'", "跟新用户账户数据失败");
            m_mainWindow =new MainWindow(nullptr, m_networkSystem);
            m_mainWindow->show();
            this->hide();
        }else {
            QMessageBox::warning(nullptr, "warning", "登录失败");
        }
    }
}


