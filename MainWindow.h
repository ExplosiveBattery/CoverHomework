#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include "NetworkSystem.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent , NetworkSystem *networkSystem);
    ~MainWindow();

private slots:
    void on_pushButton_file_dialog_clicked();

    void on_pushButton_send_clicked();

private:
    Ui::MainWindow *ui;
    NetworkSystem *m_networkSystem;
    const QStringList SUPPORT_SUFFIX={"rar", "zip", "pdf", "doc", "xls", "ppt", "chm", "hlp", "pptx", "docx", "xlsx"};
};

#endif // MAINWINDOW_H
