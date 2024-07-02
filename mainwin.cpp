#include "mainwin.h"
#include "ui_mainwin.h"
#include <QMessageBox>

mainWin::mainWin(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::mainwin)
{
    ui->setupUi(this);
}

mainWin::~mainWin()
{
    delete ui;
}

void mainWin::on_measurePressureButton_clicked()
{
    QMessageBox::information(this, "Измерение давления", "Измерение давления началось!");
}

void mainWin::on_alcotesterButton_clicked()
{
    QMessageBox::information(this, "Алкотестер", "Алкотестер активирован!");
}

void mainWin::on_temperatureButton_clicked()
{
    QMessageBox::information(this, "Температура", "Измерение температуры началось!");
}
