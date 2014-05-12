#include "sotrudniki.h"
#include "ui_sotrudniki.h"
#include <QMessageBox>
#include"delegates/checkboxdelegate.h"
bool Sotrudniki::isOpen = false;
Sotrudniki::Sotrudniki(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Sotrudniki)
{
    ui->setupUi(this);
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
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    this->Sotr="";
    this->vozrast=0;
    this->doljnost = 0;
    SotrModels = dal_prepodcontrol->getSotrModels(this->Sotr, this->doljnost, "");
    ui->tableView_sotrudniki->setModel(SotrModels);
    ui->tableView_sotrudniki->setColumnHidden(0,true);
    ui->tableView_sotrudniki->setColumnHidden(5,true);
    ui->tableView_sotrudniki->setColumnHidden(6,true);
    ui->tableView_sotrudniki->setColumnHidden(7,true);
    ui->tableView_sotrudniki->setColumnHidden(8,true);
    ui->tableView_sotrudniki->setColumnHidden(10,true);
    ui->tableView_sotrudniki->setColumnHidden(12,true);
    ui->tableView_sotrudniki->setColumnHidden(13,true);
    ui->tableView_sotrudniki->setColumnHidden(14,true);
    ui->tableView_sotrudniki->setColumnHidden(15,true);
    ui->tableView_sotrudniki->setColumnHidden(16,true);
    ui->tableView_sotrudniki->setColumnHidden(18,true);
    ui->tableView_sotrudniki->setColumnHidden(19,true);
    ui->tableView_sotrudniki->setColumnHidden(20,true);
    ui->tableView_sotrudniki->setColumnHidden(21,true);
    ui->tableView_sotrudniki->setColumnHidden(22,true);
    ui->tableView_sotrudniki->setColumnHidden(23,true);
    ui->tableView_sotrudniki->setColumnHidden(24,true);
    ui->tableView_sotrudniki->setColumnHidden(25,true);
    ui->tableView_sotrudniki->setColumnHidden(26,true);
    ui->tableView_sotrudniki->setColumnHidden(27,true);
    ui->tableView_sotrudniki->setItemDelegateForColumn(17, new checkboxdelegate(ui->tableView_sotrudniki));
    ui->tableView_sotrudniki->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_sotrudniki->horizontalHeader()->setStretchLastSection(true);
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ui->tableView_sotrudniki->addAction(ui->action_podrobno);
    ui->tableView_sotrudniki->addAction(ui->actionDelete);
}

Sotrudniki::~Sotrudniki()
{
    isOpen = false;
    delete ui;
}

void Sotrudniki::refreshData()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableView_sotrudniki->setModel(dal_prepodcontrol->getSotrModels(this->Sotr, this->doljnost, ui->comboBox_sostav->currentText()));

    if(this->vidim==true)
    {
        if(ui->tableView_sotrudniki->model()->rowCount()>0)
        {
            this->naideno.append("                                   Найдено: " + QString::number(ui->tableView_sotrudniki->model()->rowCount()));
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

void Sotrudniki::on_comboBox_doljnost_activated(int index)
{
    this->doljnost = ui->comboBox_doljnost->model()->index(index,0).data().toInt();
    qDebug()<<this->doljnost;
}

void Sotrudniki::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);        
        ui->label_naideno->setVisible(false);
        this->refreshData();
    }
    else
    {
        if (! dal_main->checkConnection())
        {
            QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
            return;
        }
        ui->groupBox_search->setVisible(true);
        ui->comboBox_doljnost->setModel(dal_prepodcontrol->getDoljnost());
        ui->comboBox_doljnost->setModelColumn(1);
        ui->comboBox_doljnost->setCurrentIndex(-1);
        this->doljnost = 0;
        this->Sotr = "";
        ui->lineEdit_FIO->clear();
        ui->comboBox_sostav->setCurrentIndex(-1);
    }
}

void Sotrudniki::on_pushButton_find_clicked()
{
    this->vidim=true;
    this->Sotr = ui->lineEdit_FIO->text();
    this->refreshData();
}


void Sotrudniki::on_pushButton_del_clicked()
{ if(ui->tableView_sotrudniki->model()->index(ui->tableView_sotrudniki->currentIndex().row(),0).data().toInt()==dal_main->getCurrentEmployee())
    {
    this->vidim = false;
    if (! ui->tableView_sotrudniki->model()->index(ui->tableView_sotrudniki->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::warning(this, tr("Ошибка удаления"), tr("Ни одной записи не выбрано"));
        return;
    }
    if (QMessageBox::warning(this, tr("Удаление записи"), tr("Вы уверены, что хотите удалить запись? \n Восстановить запись невозможно"),
                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        if (! dal_main->checkConnection())
        {
            QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
            return;
        }
        if (dal_prepodcontrol->deleteSotr(ui->tableView_sotrudniki->model()->index(ui->tableView_sotrudniki->currentIndex().row(), 0).data().toInt()))
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
    else
        QMessageBox::information(this,tr("Внимание"),tr("Вы не можете удалить не свою запись"));
}

void Sotrudniki::on_pushButton_update_clicked()
{
    this->vidim=false;
    this->refreshData();
}

void Sotrudniki::on_pushButton_add_clicked()
{
    try
    {
        addoreditform = new AddOrEditSotrudnik(this, INSERT);
        addoreditform->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void Sotrudniki::on_pushButton_edit_clicked()
{
    if(ui->tableView_sotrudniki->model()->index(ui->tableView_sotrudniki->currentIndex().row(),0).data().toInt()==dal_main->getCurrentEmployee())
        {
    if (! ui->tableView_sotrudniki->model()->index(ui->tableView_sotrudniki->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите запись из таблицы"));
        return;
    }
    int id = ui->tableView_sotrudniki->model()->index(ui->tableView_sotrudniki->currentIndex().row(), 0).data().toInt();
    try
    {
        addoreditform = new AddOrEditSotrudnik(this, EDIT, id);
        addoreditform->exec();
    }
    catch (...)
    {
        return;
    }

    this->refreshData();
    }
    else
        QMessageBox::information(this,tr("Внимание"),tr("Вы не можете редактировать не свою запись"));

}

void Sotrudniki::on_action_podrobno_triggered()
{
    if (! ui->tableView_sotrudniki->model()->index(ui->tableView_sotrudniki->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите запись из таблицы"));
        return;
    }
    int id = ui->tableView_sotrudniki->model()->index(ui->tableView_sotrudniki->currentIndex().row(), 0).data().toInt();
    try
    {
        addoreditform = new AddOrEditSotrudnik(this, EDIT, id, VIEWSV);
        addoreditform->exec();
    }
    catch (...)
    {
        return;
    }

    this->refreshData();
}

void Sotrudniki::on_pushButton_clear_clicked()
{
    this->Sotr = "";
    this->doljnost = 0;
    ui->comboBox_doljnost->setCurrentIndex(-1);
    ui->comboBox_sostav->setCurrentIndex(-1);
    ui->lineEdit_FIO->clear();
    ui->label_naideno->clear();

    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    if (!dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableView_sotrudniki->setModel(dal_prepodcontrol->getSotrModels(this->Sotr, this->doljnost, ""));
}

void Sotrudniki::on_actionDelete_triggered()
{
        on_pushButton_del_clicked();
}

void Sotrudniki::on_tableView_sotrudniki_doubleClicked(const QModelIndex &index)
{
        on_pushButton_edit_clicked();
}

void Sotrudniki::on_tableView_sotrudniki_clicked(const QModelIndex &index)
{

    if(ui->tableView_sotrudniki->model()->index(ui->tableView_sotrudniki->currentIndex().row(),0).data().toInt()==dal_main->getCurrentEmployee())
    {
        ui->pushButton_del->setEnabled(true);
        ui->pushButton_edit->setEnabled(true);
    }
    else
    {
        ui->pushButton_del->setEnabled(false);
        ui->pushButton_edit->setEnabled(false);
    }
}
