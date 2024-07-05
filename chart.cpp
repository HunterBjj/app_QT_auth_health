#include "chart.h"
#include "ui_chart.h"
#include <QPainter>

Chart::Chart(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Chart)
{

    ui->setupUi(this);
    loadData(); // Загрузка данных при создании виджета
}

Chart::~Chart()
{
    delete ui;
    //db.close();
}

void Chart::setData(const QVector<double>& temperatures, const QVector<double>& thermometers)
{

    this->temperatures = temperatures;
    this->thermometers = thermometers;
    update(); // Перерисовка виджета
}

void Chart::loadData()
{

    QSqlDatabase db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("D:/3lab_project/2/QT-Login-Application-master/Alcometer.db");

    if (!db.open()) {
        qDebug() << "Failed to connect to the database:" << db.lastError().text();
        return;
    }

    QSqlQuery query(db);
    query.prepare("SELECT temperature, thermometer FROM health");

    if (!query.exec()) {
        qDebug() << "Failed to execute query:" << query.lastError().text();
        return;
    }

    temperatures.clear();
    thermometers.clear();

    while (query.next()) {
        temperatures.append(query.value("temperature").toDouble());
        thermometers.append(query.value("thermometer").toDouble());
    }

    db.close(); // Закрываем соединение с базой данных

    update(); // Перерисовка виджета с новыми данными
}


void Chart::paintEvent(QPaintEvent *event)
{

    QPainter painter(this);

    // Установить белый цвет фона
    painter.fillRect(rect(), Qt::white);

    if (temperatures.isEmpty() || thermometers.isEmpty()) {
        return;
    }

    // Задать отступы и размер графика
    int leftMargin = 50;
    int rightMargin = 20;
    int topMargin = 20;
    int bottomMargin = 50;



    int width = this->width() - leftMargin - rightMargin;
    int height = this->height() - topMargin - bottomMargin;

    // Нарисовать оси
    painter.drawLine(leftMargin, topMargin, leftMargin, height + topMargin);
    painter.drawLine(leftMargin, height + topMargin, width + leftMargin, height + topMargin);

    // Подпись осей
        painter.drawText(leftMargin - 30, topMargin - 10, "");
        painter.drawText(width + leftMargin - 10, height + topMargin + 30, "");

    // Найти минимальные и максимальные значения температуры и термометра
    double minTemp = *std::min_element(temperatures.begin(), temperatures.end());
    double maxTemp = *std::max_element(temperatures.begin(), temperatures.end());
    double minTherm = *std::min_element(thermometers.begin(), thermometers.end());
    double maxTherm = *std::max_element(thermometers.begin(), thermometers.end());

    // Подпись оси Y сверху
      QString yAxisLabel = "°С"; // Название оси Y
      QRect yAxisRect(leftMargin - 50, topMargin, 30, height); // Расположение и размер области для текста
      // Устанавливаем черный цвет для ручки (текста)
      painter.setPen(Qt::black);
      painter.drawText(yAxisRect, Qt::AlignRight | Qt::AlignTop, yAxisLabel);

    // Нарисовать графики
       painter.setRenderHint(QPainter::Antialiasing); // Улучшение отображения линий
       painter.setPen(Qt::blue);
       for (int i = 1; i < temperatures.size(); ++i) {
           int x1 = leftMargin + (i - 1) * (width / (temperatures.size() - 1));
           int y1 = topMargin + height - (temperatures[i - 1] - minTemp) * height / (maxTemp - minTemp);
           int x2 = leftMargin + i * (width / (temperatures.size() - 1));
           int y2 = topMargin + height - (temperatures[i] - minTemp) * height / (maxTemp - minTemp);
           painter.drawLine(x1, y1, x2, y2);


       }

       // Подпись числовых значений на оси Y (Temperature)
          painter.setPen(Qt::black); // Черный цвет для подписей
          QFont font = painter.font();
          font.setPointSize(8); // Устанавливаем размер шрифта
          painter.setFont(font);

          for (int i = 0; i < temperatures.size(); ++i) {
              int x = leftMargin - 30; // Смещение для числовых значений от оси Y
              int y = topMargin + height - (temperatures[i] - minTemp) * height / (maxTemp - minTemp);
              QString label = QString::number(temperatures[i]);

              painter.drawText(QRect(x, y - 10, 30, 20), Qt::AlignRight | Qt::AlignVCenter, label);
          }

       painter.setPen(Qt::red);
       for (int i = 1; i < thermometers.size(); ++i) {
           int x1 = leftMargin + (i - 1) * (width / (thermometers.size() - 1));
           int y1 = topMargin + height - (thermometers[i - 1] - minTherm) * height / (maxTherm - minTherm);
           int x2 = leftMargin + i * (width / (thermometers.size() - 1));
           int y2 = topMargin + height - (thermometers[i] - minTherm) * height / (maxTherm - minTherm);
           painter.drawLine(x1, y1, x2, y2);



}

       // Подпись названий линий на графике
          painter.setPen(Qt::blue); // Синий цвет для подписей графика температуры
          painter.drawText(leftMargin, height + topMargin + 35, "-Градусник");

          painter.setPen(Qt::red); // Красный цвет для подписей графика термометров
          painter.drawText(leftMargin, height + topMargin + 25, "-Термометр");

}
void Chart::resizeEvent(QResizeEvent *event)
{
    QWidget::resizeEvent(event);
    update(); // Перерисовка виджета при изменении размера
}
