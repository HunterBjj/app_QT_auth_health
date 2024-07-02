#ifndef LOGIN_H
#define LOGIN_H

#include <QMessageAuthenticationCode>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QObject>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMainWindow>
#include <QCryptographicHash>
#include "registration.h"
#include "dockwidget.h"
#include "dataform.h"


namespace Ui {
class Login;
}

class Login : public QMainWindow
{
    Q_OBJECT
    
public:
    QSqlDatabase db;
    explicit Login(QWidget *parent = 0);
    ~Login();
    
private slots:
    void on_signUpButton_clicked();
    void on_checkBox_toggled(bool checked);
    void on_signInButton_clicked();

private:
    Ui::Login *ui;
    Registration *RegWin;
    DockWidget *DockW;
    DataForm *Data;



signals:
    void loggedIn(const QString& login);
};

#endif // LOGIN_H
