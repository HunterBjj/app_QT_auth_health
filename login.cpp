#include "login.h"
#include "ui_login.h"
#include "dockwidget.h"
#include "data.h"
#include <QDebug>


Login::Login(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::Login)
{
    ui->setupUi(this);
    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    qDebug() << QSqlDatabase::drivers();
    db.setDatabaseName("D:/3lab_project/2/QT-Login-Application-master/Alcometer.db");
    if (!db.open()) {
        QMessageBox::critical(0, "Ошибка", "Подключение к БД прервано");
        qDebug() << db.lastError();
    } else {
         qDebug() <<"Connection";
    }
}

Login::~Login(){
    db.close();
    delete ui;
}

void Login::on_signInButton_clicked()
{
    QString password = ui->password->text();
    QString login = ui->login->text();

    if (login.isEmpty()) {
        QMessageBox::critical(0, "Нет логина", "Пожалуйста, введите логин");
        return;
    }

    if (password.isEmpty()) {
        QMessageBox::critical(0, "Нет пароля", "Пожалуйста, введите пароль");
        return;
    }

    QSqlDatabase db = QSqlDatabase::database();
    if (!db.isOpen()) {
        QMessageBox::critical(0, "Ошибка", "Подключение к БД прервано");
        qDebug() << db.lastError();
        return;
    }

    QSqlQuery query(db);
    if (!query.prepare("SELECT login, password FROM user WHERE login = :login")) {
        qDebug() << query.lastError().text();
        return;
    }

    query.bindValue(":login", login);
    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return;
    }

    if (!query.first()) {
        QMessageBox::critical(0, "Ошибка", "Неправильный логин или пароль!");
        return;
    }

    QString databasePass = query.value(1).toString();
    QString staticSalt = "AxYh9huk#Md$";
    QString saltedPass = staticSalt + password;
    QByteArray encryptedInputPass = QCryptographicHash::hash(saltedPass.toUtf8(), QCryptographicHash::Sha256);
     qDebug() << encryptedInputPass.toHex();
    qDebug() << query.value(1).toString();
    if(login == "admin" and encryptedInputPass.toHex() == "40667b641039936adad1c0182adff080d6b3cd74f745e203ebe0eb88340f1e48")
    {
      Data = new DataForm(nullptr);
      Data->setWindowTitle("Данные о здоровье пользователей");
      Data->show();
      this->hide();
    }
    else if (encryptedInputPass.toHex() == databasePass) {
        qDebug() << query.lastError().text();
        // Создаем и показываем основную форму

        DockW = new DockWidget(nullptr);

        DockW->setWindowTitle("Осмотр сотрудника");
        // Подключаем сигнал loginSuccess к слоту onLoggedIn

        // Подключаем сигнал loginSuccess к слоту onLoggedIn
             if (!connect(this, &Login::loggedIn, DockW, &DockWidget::onLoggedIn)) {
                 qDebug() << "Failed to connect signal!";
             }
        emit loggedIn(login);
        DockW->show();
        //this->hide(); // Скрыть текущее окно Login

    } else {
        qDebug() << query.value(2).toString();
        qDebug() << query.lastError().text();
        QMessageBox::critical(0, "Ошибка", "Неверный логин или пароль!");
    }
}

void Login::on_checkBox_toggled(bool checked)
{
    if (checked) {
        ui->password->setEchoMode(QLineEdit::Normal);
    } else {
        ui->password->setEchoMode(QLineEdit::Password);
    }
}

void Login::on_signUpButton_clicked()
{
    RegWin = new Registration(this);
    RegWin->setWindowTitle("Регистрация");
    RegWin->show();
}
