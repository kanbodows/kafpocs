#include "trudoustroistvovipusknikov.h"
#include "ui_trudoustroistvovipusknikov.h"
#include "delegates/datedelegate.h"
#include "delegates/checkboxdelegate.h"
#include<QMessageBox>
bool TrudoustroistvoVipusknikov::isOpen = false;
TrudoustroistvoVipusknikov::TrudoustroistvoVipusknikov(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::TrudoustroistvoVipusknikov)
{
    ui->setupUi(this);
    ui->tableView_TrudoustrVipuskniki->addAction(ui->actionEditRec);
    this->count=0;
    if (! isOpen)
    {
        isOpen = true;
    }
    else
    {
        QMessageBox::warning(this, tr("Ошибка"), tr("Окно уже открыто"));
        throw std::exception();
    }
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        this->isOpen = false;
        throw std::exception();
    }
    this->vidim=false;
    dal_studentsControl = new Dal_studentsControl(this);
    this->Student="";
    this->Group=0;

    TrudoVipusknikModels = dal_studentsControl->getTrudoVipusk(this->Student, this->Group);

    ui->tableView_TrudoustrVipuskniki->setModel(TrudoVipusknikModels);
    ui->tableView_TrudoustrVipuskniki->setColumnHidden(0,true);
    ui->tableView_TrudoustrVipuskniki->setColumnHidden(1,true);
    ui->tableView_TrudoustrVipuskniki->setColumnHidden(2,true);
    ui->tableView_TrudoustrVipuskniki->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_TrudoustrVipuskniki->horizontalHeader()->setStretchLastSection(true);
    // ui->tableView_TrudoustrVipuskniki->setItemDelegateForColumn(8, new dateDelegate(ui->tableView_TrudoustrVipuskniki));
    ui->tableView_TrudoustrVipuskniki->setItemDelegateForColumn(6, new checkboxdelegate(ui->tableView_TrudoustrVipuskniki));
    //  ui->tableView_TrudoustrVipuskniki->addAction(ui->action_aboutVip);
    //ui->tableView_TrudoustrVipuskniki->addAction(ui->actionDel);
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ui->tableView_TrudoustrVipuskniki->addAction(ui->actionEditRec);
    ui->tableView_TrudoustrVipuskniki->addAction(ui->actionDelete);
}

TrudoustroistvoVipusknikov::~TrudoustroistvoVipusknikov()
{
    isOpen = false;
    delete ui;
}

void TrudoustroistvoVipusknikov::refreshData()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableView_TrudoustrVipuskniki->setModel(dal_studentsControl->getTrudoVipusk(this->Student, this->Group));

    if(this->vidim==true)
    {
        if(ui->tableView_TrudoustrVipuskniki->model()->rowCount()>0)
        {
            this->naideno.append("                                   Найдено: " + QString::number(ui->tableView_TrudoustrVipuskniki->model()->rowCount()));
            ui->label_result->setVisible(false);
            ui->label_naideno->setVisible(true);
            ui->label_naideno->setText(this->naideno);
            this->naideno.clear();
        }
        else
        {
            ui->label_naideno->setVisible(true);
            this->naideno.append("                                   Найдено: 0");
            ui->label_result->setVisible(true);
            ui->label_naideno->setText(this->naideno);
            this->naideno.clear();
        }
    }
}

void TrudoustroistvoVipusknikov::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        this->Student="";
        this->Group=0;
        ui->lineEdit_student->clear();
        ui->comboBox_group->setCurrentIndex(-1);
        this->refreshData();
    }
    else
    {
        ui->groupBox_search->setVisible(true);
    }
    on_pushButton_clear_clicked();
}

void TrudoustroistvoVipusknikov::on_pushButton_clear_clicked()
{
    this->Student="";
    this->Group=0;
    ui->lineEdit_student->clear();
    ui->comboBox_group->setCurrentIndex(-1);
    ui->label_naideno->setVisible(false);
    ui->label_result->setVisible(false);
    this->vidim=false;
    this->refreshData();
}

void TrudoustroistvoVipusknikov::on_pushButton_find_clicked()
{
    this->vidim=true;
    this->Student = ui->lineEdit_student->text();
    this->Group = this->Group;
    this->refreshData();
}

void TrudoustroistvoVipusknikov::on_comboBox_group_activated(int index)
{
    this->Group = ui->comboBox_group->model()->index(ui->comboBox_group->currentIndex(),0).data().toInt();
}

void TrudoustroistvoVipusknikov::on_pushButton_del_clicked()
{
    if (! ui->tableView_TrudoustrVipuskniki->model()->index(ui->tableView_TrudoustrVipuskniki->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::information(this, tr("Информация"), tr("Ни одной записи не выбрано"));
        return;
    }
    if (QMessageBox::information(this, tr("Информация"), tr("        Вы уверены, что хотите удалить запись? \n Все данные связанные с записью также будут удалены"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        if (! dal_main->checkConnection())
        {
            QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
            return;
        }
        if (dal_studentsControl->deleteTrudoVipusk(ui->tableView_TrudoustrVipuskniki->model()->index(ui->tableView_TrudoustrVipuskniki->currentIndex().row(), 0).data().toInt()))
        {
            this->refreshData();
            QMessageBox::information(this, tr("Удаление"), tr("Данные успешно удалены"));
        }
        else
        {
            QMessageBox::information(this, tr("Удаление"), tr("Не удалось удалить данные, попробуйте еще раз"));
            this->refreshData();
        }
    }
    else
    {
        this->refreshData();
        return;
    }
}

void TrudoustroistvoVipusknikov::on_pushButton_update_clicked()
{
    this->vidim=false;
    this->refreshData();
}

void TrudoustroistvoVipusknikov::on_pushButton_add_clicked()
{
    try
    {
        AddOrEditTrudoVipform = new AddOrEditTrudoVip(this, INSERT);
        AddOrEditTrudoVipform->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void TrudoustroistvoVipusknikov::on_pushButton_edit_clicked()
{
    if (! ui->tableView_TrudoustrVipuskniki->model()->index(ui->tableView_TrudoustrVipuskniki->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите запись из таблицы"));
        return;
    }
    int id = ui->tableView_TrudoustrVipuskniki->model()->index(ui->tableView_TrudoustrVipuskniki->currentIndex().row(), 0).data().toInt();
    try
    {
        AddOrEditTrudoVipform = new AddOrEditTrudoVip(this, EDIT, id);
        AddOrEditTrudoVipform->exec();
    }
    catch (...)
    {
        return;
    }

    this->refreshData();
}

void TrudoustroistvoVipusknikov::on_actionEditRec_triggered()
{
    on_pushButton_edit_clicked();
}

void TrudoustroistvoVipusknikov::on_actionDelete_triggered()
{
    on_pushButton_del_clicked();
}

void TrudoustroistvoVipusknikov::on_tableView_TrudoustrVipuskniki_doubleClicked(const QModelIndex &index)
{
    on_pushButton_edit_clicked();
}
