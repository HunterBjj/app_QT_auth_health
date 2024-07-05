#ifndef DOCKWIDGET_H
#define DOCKWIDGET_H

#include <QSqlDatabase>
#include <QObject>
#include <QSqlError>
#include <QSqlQuery>
#include <QDockWidget>


namespace Ui {
class DockWidget;
}

class DockWidget : public QDockWidget
{
    Q_OBJECT

public:
    explicit DockWidget(QWidget *parent = nullptr);
    ~DockWidget();

public slots:
    void onLoggedIn(const QString& login);

private:
    Ui::DockWidget *ui;
    QSqlDatabase db; // Добавлен член данных для базы данных
    QString currentUser;
    bool loopRunning; // Флаг для управления циклом


private slots:
    void on_measurePressureButton_clicked();
    void on_alcotesterButton_clicked();
    void on_temperatureButton_clicked();
    void on_temperatureButton_2_clicked();
    void on_send_clicked();

};

#endif // DOCKWIDGET_H
