#ifndef CHART_H
#define CHART_H

#include <QWidget>
#include <QVector>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>

namespace Ui {
class Chart;
}

class Chart : public QWidget
{
    Q_OBJECT

public:
    explicit Chart(QWidget *parent = nullptr);
    ~Chart();
    void setData(const QVector<double>& temperatures, const QVector<double>& thermometers);
     void loadData();

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    Ui::Chart *ui;
    QVector<double> temperatures;
    QVector<double> thermometers;
    QStringList lineNames; // Поле для хранения названий линий
};

#endif // CHART_H
