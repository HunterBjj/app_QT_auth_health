#ifndef MAINWIN_H
#define MAINWIN_H

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
#include <QWidget>

namespace Ui {
class mainwin;  // Убедитесь, что это имя совпадает с именем в ui_mainwin.h
}

class mainWin : public QWidget
{
    Q_OBJECT

public:
    explicit mainWin(QWidget *parent = nullptr);
    ~mainWin();

private slots:
    void on_measurePressureButton_clicked();
    void on_alcotesterButton_clicked();
    void on_temperatureButton_clicked();

private:
    Ui::mainwin *ui;
};

#endif // MAINWIN_H
