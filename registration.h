#ifndef Registration_H
#define Registration_H

#include <QMessageAuthenticationCode>
#include <QDialog>
#include <QCryptographicHash>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QObject>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>
#include <QMainWindow>
#include <QRandomGenerator>

namespace Ui {
    class Registration;
}

class Registration : public QDialog
{
    Q_OBJECT

public:
    explicit Registration(QWidget *parent = 0);
    ~Registration();

private:
    Ui::Registration *ui;

signals:
    void DockW(const QString& login);

private slots:
    void on_cancelButton_clicked();
    void on_signUpButton_clicked();
};

#endif // Registration_H
