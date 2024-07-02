#include "data.h"
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include "ui_data.h"

data::data()


{

    db = QSqlDatabase::addDatabase("QSQLITE");
        db.setDatabaseName("D:/3lab_project/2/QT-Login-Application-master/Alcometer.db");
        if (!db.open()) {
            qDebug() << "Failed to connect to the database:" << db.lastError();
        }
}


QMap<QString, QVariant> data::getHealthData(const QString& login)
{
    QMap<QString, QVariant> healthData;
    QSqlQuery query(db);
    query.prepare("SELECT user.fullname, health.давление, health.permill, health.temperature"
                  "FROM health "
                  "JOIN user ON health.id_user = user.id_user "
                  "WHERE user.login = :login "
                  "ORDER BY health.id_health DESC LIMIT 1");
    query.bindValue(":login", login);
    if (!query.exec() || !query.first()) {
        qDebug() << "Failed to retrieve health data:" << query.lastError();
        return healthData;
    }
    healthData["fullname"] = query.value("fullname");
    healthData["давление"] = query.value("давление");
    healthData["permill"] = query.value("permill");
    healthData["temperature"] = query.value("temperature");
    return healthData;
}
