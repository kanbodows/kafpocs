#include "add_or_edit_procentovka.h"
#include "ui_add_or_edit_procentovka.h"

#include <QDebug>
#include <QMessageBox>
add_or_edit_procentovka::add_or_edit_procentovka(QWidget *parent, int select, int id) :
    QDialog(parent),
    ui(new Ui::add_or_edit_procentovka)
{
    ui->setupUi(this);
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    dal_studentsControl = new Dal_studentsControl(this);
    ui->comboBox_student->setModel(dal_studentsControl->getStudentineit(id));
    ui->comboBox_student->setModelColumn(1);
    this->id_studenta = ui->comboBox_student->model()->index(0,0).data().toInt();
    this->FIOStudenta = ui->comboBox_student->model()->index(0,1).data().toString();
    this->days = 7;



}

add_or_edit_procentovka::~add_or_edit_procentovka()
{
    delete ui;
}

void add_or_edit_procentovka::on_pushButtonSformGrafProc_clicked()
{
ui->pushButtonSformGrafProc->setEnabled(false);
    this->datas = ui->dateEdit_data->date();
    for(int i=0; i < 12; i++)
    {
    addRMToTableWidget(this->datas,
                       this->FIOStudenta,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       0,
                       this->id_studenta);
    this->datas = ui->dateEdit_data->date().addDays(days);
    this->days += 7;
    }
}

void add_or_edit_procentovka::addRMToTableWidget(QDate dates, QString FioStud, int vved, int analTreb, int konstr, int docum, int eksperement, int ekonomich, int zakl, int rabotaPP, int prezentation, int id_students)
{
    int row = ui->tableWidget->rowCount();//кол-во записей в table
    ui->tableWidget->insertRow(row);//добавим новую запись
    //добавление новых элементов
    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(dates.toString("yyyy-MM-dd")));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(FioStud));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(QString::number(vved)));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(analTreb)));
    ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(konstr)));
    ui->tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(docum)));
    ui->tableWidget->setItem(row, 6, new QTableWidgetItem(QString::number(eksperement)));
    ui->tableWidget->setItem(row, 7, new QTableWidgetItem(QString::number(ekonomich)));
    ui->tableWidget->setItem(row, 8, new QTableWidgetItem(QString::number(zakl)));
    ui->tableWidget->setItem(row, 9, new QTableWidgetItem(QString::number(rabotaPP)));
    ui->tableWidget->setItem(row, 10, new QTableWidgetItem(QString::number(prezentation)));
    ui->tableWidget->setItem(row, 11, new QTableWidgetItem(QString::number(id_students)));
}

void add_or_edit_procentovka::on_pushButton_addToDatabase_clicked()
{
    int rowcount = ui->tableWidget->rowCount();
    qDebug()<<"rowcount"<<rowcount;
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    for(int i=0; i < rowcount; i++)
    {
        dal_studentsControl->addProcentovka(ui->tableWidget->item(i,11)->text().toInt(),
                                            ui->tableWidget->item(i,0)->text(),
                                            ui->tableWidget->item(i,2)->text().toInt(),
                                            ui->tableWidget->item(i,3)->text().toInt(),
                                            ui->tableWidget->item(i,4)->text().toInt(),
                                            ui->tableWidget->item(i,5)->text().toInt(),
                                            ui->tableWidget->item(i,6)->text().toInt(),
                                            ui->tableWidget->item(i,7)->text().toInt(),
                                            ui->tableWidget->item(i,8)->text().toInt(),
                                            ui->tableWidget->item(i,9)->text().toInt(),
                                            ui->tableWidget->item(i,10)->text().toInt());
    }
    this->close();
}
