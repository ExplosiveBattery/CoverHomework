#ifndef LOGINWINDOW_H
#define LOGINWINDOW_H

#include "MainWindow.h"
#include "AccountItem.h"
#include <QDialog>
#include <QSqlQuery>
#include <QListWidget>
#include <QMouseEvent>
#include "NetworkSystem.h"

bool SQLErrorDetect (QSqlQuery *pQuery, const QString strCommand,const char *info);

namespace Ui {
class LoginWindow;
}

class LoginWindow : public QDialog
{
    Q_OBJECT

public:
    explicit LoginWindow(QWidget *parent = 0);
    ~LoginWindow();
protected:
    void mousePressEvent(QMouseEvent *event);
    void mouseMoveEvent(QMouseEvent *event);
    void mouseReleaseEvent(QMouseEvent *);
private:
    void fillUsernameComboBox(void);
    void initSQL(void);

private slots:
    void on_pushButton_login_clicked();
    void deleteAccount(QString username);
    void showAccount(QString username);

private:
    Ui::LoginWindow *ui;
    bool m_drag;QPoint m_windowPosition; //实现鼠标拖动功能
    QSqlQuery *m_pQuery;
    QListWidget *m_usernameListWidget;

    NetworkSystem *m_networkSystem;
    MainWindow *m_mainWindow;
};

#endif // LOGINWINDOW_H
