#include "dataform.h"
//#include "chart.h"
#include "ui_dataform.h"
#include <QFileDialog>
#include <QMessageBox>
#include <QFile>

DataForm::DataForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DataForm),
     loopRunning(false) // Инициализация флага
{
    ui->setupUi(this);
    // Инициализация базы данных
    db = QSqlDatabase::addDatabase("QSQLITE");
    db.setDatabaseName("D:/3lab_project/2/QT-Login-Application-master/Alcometer.db");
    if (!db.open()) {
        qDebug() << "Failed to connect to the database:" << db.lastError();
    } else {
        qDebug() << "Connected to the database.";
    }
    // Инициализация модели
       model = new QSqlQueryModel(this);
    // Установка модели для QTableView
        ui->tableView->setModel(model);

        // Загрузка данных
         loadData();
}

DataForm::~DataForm()
{
    delete ui;
}

void DataForm::setLogin(const QString& login)
{
    userLogin = login;
    loadData();
}

void DataForm::on_download_clicked(){
    QString fileName = QFileDialog::getSaveFileName(this, "Сохранить как CSV", QDir::homePath(), "CSV Files (*.csv)");

       if (fileName.isEmpty())
           return;

       QFile file(fileName);
       if (!file.open(QFile::WriteOnly | QFile::Text)) {
           QMessageBox::critical(this, "Ошибка", "Не удалось создать файл CSV");
           return;
       }

       QTextStream out(&file);

       // Записываем заголовки столбцов
       QStringList headers;
       for (int col = 0; col < model->columnCount(); ++col) {
           headers.append(model->headerData(col, Qt::Horizontal).toString());
       }
       out << headers.join(",") << "\n";

       // Записываем данные
       for (int row = 0; row < model->rowCount(); ++row) {
           QStringList rowData;
           for (int col = 0; col < model->columnCount(); ++col) {
               QString data = model->data(model->index(row, col)).toString().replace('.', ',');
               // Если данные содержат запятую, заключаем их в двойные кавычки
               if (data.contains(',')) {
                   data = "\"" + data + "\"";
               }
               rowData.append(data);
           }
           out << rowData.join(",") << "\n";
       }

       file.close();
       QMessageBox::information(this, "Экспорт в CSV", "Данные успешно экспортированы в файл CSV:\n" + fileName);
   }


void DataForm::loadData()
{
    if (!model) {
           qDebug() << "Model is not initialized.";
           return;
       }

    QSqlQuery query(db);
       query.prepare("SELECT user.fullname, health.temperature, health.thermometer "
                     "FROM health "
                     "JOIN user ON health.id_user = user.id_user");

       if (!query.exec()) {
           qDebug() << "Failed to execute query:" << query.lastError();
           return;
       }
       while (query.next()) {
              qDebug() << "Full Name:" << query.value("fullname").toString();
              qDebug() << "Temperature:" << query.value("temperature").toDouble();
              qDebug() << "Thermometer:" << query.value("thermometer").toDouble();
          }
       // Установка данных модели
       model->setQuery(query);




       // Настройка заголовков столбцов
       model->setHeaderData(0, Qt::Horizontal, tr("ФИО"));
       model->setHeaderData(1, Qt::Horizontal, tr("Градусник"));
       model->setHeaderData(2, Qt::Horizontal, tr("Термометр"));

       ui->tableView->resizeColumnsToContents();
}

void DataForm::on_delete_row_clicked()
{
    QItemSelectionModel *selectionModel = ui->tableView->selectionModel();
        QModelIndexList selectedIndexes = selectionModel->selectedRows();

        // Перебираем индексы в обратном порядке, чтобы корректно удалить строки
        for (int i = selectedIndexes.count() - 1; i >= 0; --i) {
            QModelIndex index = selectedIndexes.at(i);
            int row = index.row();

            // Получаем значения для удаления (например, ID пользователя или другие идентификаторы)
            QString fullname = model->index(row, 0).data().toString();
            double temperature = model->index(row, 1).data().toDouble();
            double thermometer = model->index(row, 2).data().toDouble();

            // Выполняем запрос на удаление из базы данных
            QSqlQuery query(db);
            query.prepare("DELETE FROM health WHERE temperature = :temperature AND thermometer = :thermometer");
            query.bindValue(":temperature", temperature);
            query.bindValue(":thermometer", thermometer);

            if (!query.exec()) {
                qDebug() << "Failed to delete row:" << query.lastError();
            } else {
                qDebug() << "Row deleted successfully.";
                // Удаляем строку из модели QTableView
                model->removeRow(row);
            }
        }

        // Обновляем данные в модели после удаления
        loadData();
}

void DataForm::on_chart_clicked(){
    charts = new Chart(nullptr);
    charts->setWindowTitle("График измерений");
    charts->resize(1000, 800); // Устанавливаем размер окна графика
    // Загрузка данных в график
    charts->show();
    this->hide();
}

