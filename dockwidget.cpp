#include "dockwidget.h"
#include "ui_dockwidget.h"
#include <QMessageBox>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>

QString thermometerDB;
QString temperatureDB;

DockWidget::DockWidget(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::DockWidget)
{
    ui->setupUi(this);
    // Установка QTextEdit только для чтения
    ui->temperature->setReadOnly(true);
    ui->presser->setReadOnly(true);
    ui->alcometr->setReadOnly(true);

        db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("D:/3lab_project/2/QT-Login-Application-master/Alcometer.db");
        if (!db.open()) {
            QMessageBox::critical(0, "Connection error", "Could not connect to the database");
            qDebug() << db.lastError();
        }

}


DockWidget::~DockWidget()
{
    delete ui;
}
void DockWidget::on_measurePressureButton_clicked()
{
    // Действие при нажатии кнопки "Измерение давления"

}

void DockWidget::on_alcotesterButton_clicked()
{
    // Действие при нажатии кнопки "Алкотестер"

}

QString user_login;
void DockWidget::onLoggedIn(const QString& login)
{
    user_login = login;
    qDebug() << "Logged in user:" << login;
    // Дополнительные действия с логином
}



void DockWidget::on_temperatureButton_2_clicked()
{
    QString thermometer = ui->temperature_2->toPlainText();
    if (thermometer.isEmpty()) {
        QMessageBox::critical(0, "Нет данных", "Пожалуйста, введите температуру");
        return;
    }
    bool ok;
    double value = thermometer.replace(",", ".").toDouble(&ok);
         if (value < 0) {
         QMessageBox::warning(this, "Проверка числа", "Число отрицательное.");
         }
        else if (ok) {
            qDebug() << "Text is a valid double:" << value;
             temperatureDB = thermometer;
        } else {
           QMessageBox::critical(0, "Ошибка", "Пожалуйста, введите число");
        }


}



void DockWidget::on_temperatureButton_clicked()
{


    // Получение списка доступных COM-портов
       QList<QSerialPortInfo> serialPorts = QSerialPortInfo::availablePorts();

       // Вывод информации о доступных портах
       qDebug() << "Доступные COM-порты:";
       for (const QSerialPortInfo &portInfo : serialPorts) {

           qDebug() << "Имя:" << portInfo.portName();
           qDebug() << "Описание:" << portInfo.description();
           qDebug() << "Производитель:" << portInfo.manufacturer();
           qDebug() << "Системное местоположение:" << portInfo.systemLocation();
           qDebug() << "-----------------------------------";
       }


       QSerialPort serial;
       if (!serialPorts.isEmpty()) {

           QSerialPortInfo selectedPort = serialPorts.first();
           qDebug() << selectedPort.portName();
           qDebug() << "Выбран порт:" << selectedPort.portName();
           qDebug() << "Описание:" << selectedPort.description();

           // Настройка параметров порта
           serial.setPort(selectedPort);
           serial.setBaudRate(QSerialPort::Baud9600);
           serial.setDataBits(QSerialPort::Data8);
           serial.setParity(QSerialPort::NoParity);
           serial.setStopBits(QSerialPort::OneStop);
           serial.setFlowControl(QSerialPort::NoFlowControl);

           // Открытие порта для чтения и записи
           if (serial.open(QSerialPort::ReadWrite)) {
               qDebug() << "Порт открыт успешно.";

                QByteArray sendData = "1";
               // Отправка данных
                   qint64 bytesWritten = serial.write(sendData);
                   if (bytesWritten == -1) {
                       qDebug() << QObject::tr("Ошибка при отправке данных: %1").arg(serial.errorString());
                   } else {
                       qDebug() << QObject::tr("Отправлено %1 байт данных: %2").arg(bytesWritten).arg(QString(sendData));
                   }

                     serial.waitForReadyRead(200); // Задержка в 500 миллисекунд (0.5 секунды)

                QByteArray data1;

               while(1) {
                   serial.waitForReadyRead(400);
                   serial.write(sendData);
                    QByteArray data = serial.readAll();
                    serial.waitForReadyRead(400);
                   if (!data.isEmpty()) {
                   qDebug() << "Прочитанные данные:" << data;
                   data1 = data;
                   break;
                   }
                   }

               if (!data1.isEmpty()) {
                    ui->temperature->append(QString::fromStdString(data1.toStdString()));
                    thermometerDB = data1;


                      }


               // Закрытие порта после использования
               serial.close();
               qDebug() << "Порт закрыт.";
           } else {
               qDebug() << "Не удалось открыть порт:" << serial.errorString();
           }
       } else {
           qDebug() << "Нет доступных COM-портов.";
       }

}


void DockWidget::on_send_clicked(){
    QSqlQuery query(db);
    if (temperatureDB .isEmpty()) {
        QMessageBox::critical(0, "Нет данных", "Пожалуйста, введите температуру");
        return;
    }
    if (thermometerDB.isEmpty()) {
        QMessageBox::critical(0, "Нет данных", "Пожалуйста, введите температуру!!!!");
        return;
    }
    bool ok;
    double temperatureValue = (temperatureDB.replace(",", ".")).toDouble(&ok);
     double thermometerValue = (thermometerDB.replace(",", ".")).toDouble();
    if (ok) {
            qDebug() << "Text is a valid double:";
        } else {
           QMessageBox::critical(0, "Ошибка", "Пожалуйста, введите число");

     }
   if (!query.prepare("INSERT INTO health (id_user, thermometer, temperature) VALUES ((SELECT id_user FROM user WHERE login = :login), :thermometer, :temperature)")) {
        qDebug() << query.lastError().text();
    } else {
         qDebug() << thermometerValue;
         qDebug() << temperatureValue;
        query.bindValue(":login", user_login);
        query.bindValue(":thermometer", thermometerValue);
        query.bindValue(":temperature", temperatureValue);
        if (!query.exec()) {
            qDebug() << query.lastError().text();
        } else {
            QMessageBox::information(0, "Успешно", "Измерение внесены в базу данных");
        }
    }
}
