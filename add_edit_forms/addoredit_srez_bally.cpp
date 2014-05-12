#include "addoredit_srez_bally.h"
#include "ui_addoredit_srez_bally.h"
#include "delegates/readonlydelegate.h"
#include "delegates/spinboxdelegate.h"

addoredit_srez_bally::addoredit_srez_bally(QWidget *parent, int id_srez) :
    QDialog(parent),
    id_srez(id_srez),
    ui(new Ui::addoredit_srez_bally)
{
    ui->setupUi(this);
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    dal_studentscontrol = new Dal_studentsControl(this);
    QSqlQuery *querySrez = new QSqlQuery();
    querySrez->prepare("SELECT * FROM srez_view WHERE id_srez = " + QString::number(id_srez));
    querySrez->exec();
    querySrez->first();
    this->setWindowTitle("Баллы по срезу знаний в группе " + querySrez->value(2).toString());
    ui->tableWidget->horizontalHeaderItem(1)->setText(querySrez->value(3).toString());
    ui->tableWidget->setColumnHidden(6, true);
    ui->tableWidget->setItemDelegateForColumn(0, new readOnlyDelegate(ui->tableWidget));
    ui->tableWidget->setItemDelegateForColumn(1, new SpinBoxDelegate(ui->tableWidget));
    count = 5;
    if(!querySrez->value(4).isNull())
    {
        ui->tableWidget->horizontalHeaderItem(2)->setText(querySrez->value(4).toString());
        ui->tableWidget->setItemDelegateForColumn(2, new SpinBoxDelegate(ui->tableWidget));
    }
    else
    {
        ui->tableWidget->setColumnHidden(2, true);
        count--;
    }
    if(!querySrez->value(5).isNull())
    {
        ui->tableWidget->horizontalHeaderItem(3)->setText(querySrez->value(5).toString());
        ui->tableWidget->setItemDelegateForColumn(3, new SpinBoxDelegate(ui->tableWidget));
    }
    else
    {
        ui->tableWidget->setColumnHidden(3, true);
        count--;
    }
    if(!querySrez->value(6).isNull())
    {
        ui->tableWidget->horizontalHeaderItem(4)->setText(querySrez->value(6).toString());
        ui->tableWidget->setItemDelegateForColumn(4, new SpinBoxDelegate(ui->tableWidget));
    }
    else
    {
        ui->tableWidget->setColumnHidden(4, true);
        count--;
    }
    if(!querySrez->value(7).isNull())
    {
        ui->tableWidget->horizontalHeaderItem(5)->setText(querySrez->value(7).toString());
        ui->tableWidget->setItemDelegateForColumn(5, new SpinBoxDelegate(ui->tableWidget));
    }
    else
    {
        ui->tableWidget->setColumnHidden(5, true);
        count--;
    }
    QSqlQuery *queryStud = new QSqlQuery();
    queryStud->prepare("select * from srez_bally_view where s_group_id = " + querySrez->value(1).toString());
    queryStud->exec();
    queryStud->first();
    for (int i = 0; i < queryStud->size(); ++i)
    {
        ui->tableWidget->insertRow(i);
        ui->tableWidget->setItem(i, 0, new QTableWidgetItem(queryStud->value(2).toString()));
        ui->tableWidget->setItem(i, 1, new QTableWidgetItem(queryStud->value(3).toString()));
        ui->tableWidget->setItem(i, 2, new QTableWidgetItem(queryStud->value(4).toString()));
        ui->tableWidget->setItem(i, 3, new QTableWidgetItem(queryStud->value(5).toString()));
        ui->tableWidget->setItem(i, 4, new QTableWidgetItem(queryStud->value(6).toString()));
        ui->tableWidget->setItem(i, 5, new QTableWidgetItem(queryStud->value(7).toString()));
        ui->tableWidget->setItem(i, 6, new QTableWidgetItem(queryStud->value(0).toString()));
        queryStud->next();
    }
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

addoredit_srez_bally::~addoredit_srez_bally()
{
    delete ui;
}

void addoredit_srez_bally::on_pushButton_cancel_clicked()
{
    this->close();
}

void addoredit_srez_bally::on_pushButton_save_clicked()
{
    /*if (ui->comboBox_discipline_1->currentIndex() == -1 || ui->comboBox_gruppa->currentIndex() == -1)
    {
        ui->label_error->setVisible(true);
        return;
    }*/
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    QSqlQuery* deleteQuery = new QSqlQuery;
    deleteQuery->prepare("DELETE FROM is_srez_bally WHERE srez_id = " + QString::number(id_srez));
    deleteQuery->exec();
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i)
    {
        int mark2, mark3, mark4, mark5;
        if(count<2)
            mark2 = 0;
        else
            mark2 = ui->tableWidget->item(i, 2)->text().toInt();
        if(count<3)
            mark3 = 0;
        else
            mark3 = ui->tableWidget->item(i, 3)->text().toInt();
        if(count<4)
            mark4 = 0;
        else
            mark4 = ui->tableWidget->item(i, 4)->text().toInt();
        if(count<5)
            mark5 = 0;
        else
            mark5 = ui->tableWidget->item(i, 5)->text().toInt();

        dal_studentscontrol->addSrezBally(ui->tableWidget->item(i, 6)->text().toInt(),
                                          ui->tableWidget->item(i, 1)->text().toInt(),
                                          mark2,
                                          mark3,
                                          mark4,
                                          mark5,
                                          this->id_srez);

    }
    QMessageBox::information(this, tr("Информация"), tr("Записи успешно добавлены"));
    this->close();
}
