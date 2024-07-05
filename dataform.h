#ifndef DATAFORM_H
#define DATAFORM_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QSqlQueryModel>
#include <QSqlQueryModel>
#include "chart.h"

namespace Ui {
class DataForm;
}

class DataForm : public QWidget
{
    Q_OBJECT

public:
    explicit DataForm(QWidget *parent = nullptr);
    ~DataForm();
    void setLogin(const QString& login);

private slots:
    void on_download_clicked();
    void on_delete_row_clicked();
    void on_chart_clicked();


private:
    Ui::DataForm *ui;
    QSqlDatabase db;
    QString userLogin;
    QSqlQueryModel *model;
    Chart *charts;
    void loadData();
     bool loopRunning; // Флаг для управления циклом
};

#endif // DATAFORM_H
