#include "addoredit_gak_bally.h"
#include "ui_addoredit_gak_bally.h"
#include "delegates/readonlydelegate.h"
#include "delegates/checkboxdelegate.h"
#include "delegates/spinboxdelegate.h"

addoredit_gak_bally::addoredit_gak_bally(QWidget *parent, int id_gak) :
    QDialog(parent),
    id_gak(id_gak),
    ui(new Ui::addoredit_gak_bally)
{
    ui->setupUi(this);
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    dal_studentscontrol = new Dal_studentsControl(this);
    ui->comboBox_spec->setModel(dal_studentscontrol->getSpec());
    ui->comboBox_spec->setModelColumn(1);
    ui->comboBox_spec->setCurrentIndex(-1);
    QSqlQuery *querySrez = new QSqlQuery();
    querySrez->prepare("SELECT discp1, discp2, discp3, discp4 FROM GAKView WHERE gak_id = " + QString::number(id_gak));
    querySrez->exec();
    querySrez->first();
    ui->tableWidget->horizontalHeaderItem(1)->setText(querySrez->value(0).toString());
    ui->tableWidget->horizontalHeaderItem(2)->setText(querySrez->value(1).toString());
    ui->tableWidget->horizontalHeaderItem(3)->setText(querySrez->value(2).toString());
    ui->tableWidget->horizontalHeaderItem(4)->setText(querySrez->value(3).toString());
    ui->tableWidget->setItemDelegateForColumn(0, new readOnlyDelegate(ui->tableWidget));
    ui->tableWidget->setItemDelegateForColumn(1, new SpinBoxDelegate(ui->tableWidget));
    ui->tableWidget->setItemDelegateForColumn(2, new SpinBoxDelegate(ui->tableWidget));
    ui->tableWidget->setItemDelegateForColumn(3, new SpinBoxDelegate(ui->tableWidget));
    ui->tableWidget->setItemDelegateForColumn(4, new SpinBoxDelegate(ui->tableWidget));
    ui->tableWidget->setItemDelegateForColumn(5, new checkboxdelegate(ui->tableWidget));
    ui->tableWidget->setColumnHidden(6, true);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->comboBox_group->setEnabled(false);
}

addoredit_gak_bally::~addoredit_gak_bally()
{
    delete ui;
}

void addoredit_gak_bally::on_pushButton_cancel_clicked()
{
    this->close();
}

void addoredit_gak_bally::on_pushButton_save_clicked()
{
    if (ui->comboBox_group->currentIndex() == -1)
    {
        ui->label_error->setVisible(true);
        return;
    }
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    QSqlQuery* deleteQuery = new QSqlQuery;
    deleteQuery->prepare("DELETE FROM is_gak_bally WHERE gak_id = " + QString::number(id_gak) + " AND group_id = " + ui->comboBox_group->model()->index(ui->comboBox_group->currentIndex(),0).data().toString());
    deleteQuery->exec();
    qDebug()<<deleteQuery->lastError();
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i)
    {

        bool r = dal_studentscontrol->addGakBally(ui->tableWidget->item(i, 6)->text().toInt(),
                                                  ui->tableWidget->item(i, 1)->text().toInt(),
                                                  ui->tableWidget->item(i, 2)->text().toInt(),
                                                  ui->tableWidget->item(i, 3)->text().toInt(),
                                                  ui->tableWidget->item(i, 4)->text().toInt(),
                                                  ui->tableWidget->item(i, 5)->text().toInt(),
                                                  this->id_gak,
                                                  ui->comboBox_group->model()->index(ui->comboBox_group->currentIndex(),0).data().toInt());

        qDebug()<<r, i;
    }
    QMessageBox::information(this, tr("Информация"), tr("Записи успешно добавлены"));
    this->close();
}



void addoredit_gak_bally::on_comboBox_group_activated(int index)
{
    if (ui->comboBox_group->currentIndex() == -1 )
        return;
    while (ui->tableWidget->rowCount() > 0)
        ui->tableWidget->removeRow(0);
    QSqlQueryModel *queryBally = new QSqlQueryModel();
    queryBally = dal_studentscontrol->getGakBally(0, ui->comboBox_group->model()->index(ui->comboBox_group->currentIndex(),0).data().toInt(), this->id_gak);
    if(queryBally->rowCount()>0)
    {
        for (int i = 0; i < queryBally->rowCount(); ++i)
        {
            ui->tableWidget->insertRow(i);
            ui->tableWidget->setItem(i, 0, new QTableWidgetItem(queryBally->record(i).value(6).toString()));
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem(queryBally->record(i).value(7).toString()));
            ui->tableWidget->setItem(i, 2, new QTableWidgetItem(queryBally->record(i).value(8).toString()));
            ui->tableWidget->setItem(i, 3, new QTableWidgetItem(queryBally->record(i).value(9).toString()));
            ui->tableWidget->setItem(i, 4, new QTableWidgetItem(queryBally->record(i).value(10).toString()));
            ui->tableWidget->setItem(i, 5, new QTableWidgetItem(queryBally->record(i).value(11).toString()));
            ui->tableWidget->setItem(i, 6, new QTableWidgetItem(queryBally->record(i).value(3).toString()));
        }
    }
    else
    {
        QSqlQueryModel *queryStud = new QSqlQueryModel();
        queryStud = dal_studentscontrol->getStudentGroup(ui->comboBox_group->model()->index(ui->comboBox_group->currentIndex(),0).data().toInt());
        for (int i = 0; i < queryStud->rowCount(); ++i)
        {
            ui->tableWidget->insertRow(i);
            ui->tableWidget->setItem(i, 0, new QTableWidgetItem(queryStud->record(i).value(1).toString()));
            ui->tableWidget->setItem(i, 1, new QTableWidgetItem());
            ui->tableWidget->setItem(i, 2, new QTableWidgetItem());
            ui->tableWidget->setItem(i, 3, new QTableWidgetItem());
            ui->tableWidget->setItem(i, 4, new QTableWidgetItem());
            ui->tableWidget->setItem(i, 5, new QTableWidgetItem());
            ui->tableWidget->setItem(i, 6, new QTableWidgetItem(queryStud->record(i).value(0).toString()));
        }
    }
}

void addoredit_gak_bally::on_comboBox_spec_activated(int index)
{
    ui->comboBox_group->setEnabled(true);
    ui->comboBox_group->setModel(dal_studentscontrol->getComboGroup(ui->comboBox_spec->model()->index(ui->comboBox_spec->currentIndex(),0).data().toInt()));
    ui->comboBox_group->setModelColumn(1);
    ui->comboBox_group->setCurrentIndex(-1);
}
