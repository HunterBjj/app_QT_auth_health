#ifndef DATA_H
#define DATA_H
#include <QSqlDatabase>
#include <QMap>
#include <QVariant>

namespace Ui {
    class data;
}
class data
{
public:
    data();
    QMap<QString, QVariant> getHealthData(const QString& login);

private:
    QSqlDatabase db;

private:
    Ui::data *ui;
    //Вata *Data;
};

#endif // DATA_H
