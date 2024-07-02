#include "registration.h"
#include "ui_registration.h"
#include <QCryptographicHash>
#include <QMessageBox>
#include <QRandomGenerator>
#include <QSqlError>
#include <QSqlQuery>
#include <QDebug>

Registration::Registration(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Registration)
{
    ui->setupUi(this);
    if (!QSqlDatabase::contains("AlcometerConnection")) {
          QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE", "AlcometerConnection");
          qDebug() << QSqlDatabase::drivers();
          db.setDatabaseName("D:/3lab_project/2/QT-Login-Application-master/Alcometer.db");
          if (!db.open()) {
              QMessageBox::critical(0, "Connection error", "Could not connect to the database");
              qDebug() << db.lastError();
          } else {
               qDebug() <<"Connection";
          }
      }
}

Registration::~Registration()
{
    delete ui;
}

QString getRandomString(){
   const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
   const int randomStringLength = 12;
   QString randomString;
   for(int i = 0; i < randomStringLength; ++i)
   {
     int index = QRandomGenerator::global()->generate() % possibleCharacters.length();
     QChar nextChar = possibleCharacters.at(index);
     randomString.append(nextChar);
   }
   return randomString;
}

void Registration::on_signUpButton_clicked()
{
    QSqlDatabase db = QSqlDatabase::database("AlcometerConnection");

    if (!db.isOpen()) {
        if (!db.open()) {
            QMessageBox::critical(0, "Connection error", "Could not connect to the database");
            qDebug() << db.lastError();
            return;
        }
    }

    QString username = ui->usernameField->text();
    QString password = ui->passwordField->text();
    QString passwordRepeat = ui->passRepeatField->text();
    QString login = ui->login_2->text();

    QSqlQuery query(db);

    /*-----------------Проверка на пустоту введенных данных--------------*/

    if (passwordRepeat.isEmpty()) {
        QMessageBox::critical(0, "Нет пароля", "Введите пароль");
        return;
    }
    if (username.isEmpty()) {
        QMessageBox::critical(0, "Нет ФИО", "Введите ФИО");
        return;
    }
    if (password.isEmpty()) {
        QMessageBox::critical(0, "Нет пароля", "Введите пароль");
        return;
    }
    if (login.isEmpty()) {
        QMessageBox::critical(0, "Нет логина", "Введите логин");
        return;
    }
    if (password != passwordRepeat) {
        QMessageBox::critical(0, "Пароли не совпадают", "Повторите пароль");
        return;
    }

    /*------------------------Проверка логина---------------------------------------------*/

    query.prepare("SELECT login FROM user WHERE login = :login");
    query.bindValue(":login", login);

    if (!query.exec()) {
        qDebug() << query.lastError().text();
        return;
    }

    if (query.next()) {
        QString foundLogin = query.value(0).toString(); // Получаем найденный логин из результата запроса
        QMessageBox::critical(0, "Ошибка", "Пользователь с логином '" + foundLogin + "' уже существует");
        return;
    }

    /*---------------Проверка пароля ------------------------------*/

    // Хеширование пароля
       QString staticSalt = "AxYh9huk#Md$";
       QString message = staticSalt + password;
       QByteArray encryptedPass = QCryptographicHash::hash(message.toUtf8(), QCryptographicHash::Sha256);

       // Вставка данных в БД
       query.prepare("INSERT INTO user (login, password, fullname) VALUES (:login, :password, :fullname)");
       query.bindValue(":login", login);
       query.bindValue(":password", encryptedPass.toHex());
       query.bindValue(":fullname", username);
       if (!query.exec()) {
           qDebug() << query.lastError().text();
           QMessageBox::critical(0, "Ошибка", "Из-за ошибки ваша регистрация не была завершена!");
           return;
       } else {
           QMessageBox::information(0, "Регистрация успешна завершена", "Вы зарегистрированы");
           close();
       }
}

void Registration::on_cancelButton_clicked()
{
    close();
}
