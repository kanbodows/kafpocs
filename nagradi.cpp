#include "nagradi.h"
#include "ui_nagradi.h"
#include "delegates/datedelegate.h"
#include <QMessageBox>

bool Nagradi::isOpen = false;
Nagradi::Nagradi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Nagradi)
{
    ui->setupUi(this);
    if (!isOpen)
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
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    ui->tableView_nagradi->setModel(dal_prepodcontrol->getNagradi(""));
    ui->tableView_nagradi->setColumnHidden(0,true);
    ui->tableView_nagradi->setColumnHidden(1,true);
    ui->tableView_nagradi->setColumnHidden(5,true);
    ui->tableView_nagradi->setItemDelegateForColumn(4, new dateDelegate(ui->tableView_nagradi));
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ui->tableView_nagradi->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_nagradi->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_nagradi->verticalHeader()->hide();
//    ui->tableView_nagradi->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView_nagradi->addAction(ui->actionEdit);
    ui->tableView_nagradi->addAction(ui->actionDelete);
}

Nagradi::~Nagradi()
{
    isOpen = false;
    delete ui;
}

void Nagradi::refreshData()
{
    if (!dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableView_nagradi->setModel(dal_prepodcontrol->getNagradi(ui->lineEdit_sotr->text()));
    if(ui->groupBox_search->isVisible())
    {
        if(ui->tableView_nagradi->model()->rowCount()>0)
        {
            ui->label_result->setVisible(false);
            ui->label_naideno->setVisible(true);
            ui->label_naideno->setText("    Найдено: " + QString::number(ui->tableView_nagradi->model()->rowCount()));
        }
        else
        {
            ui->label_naideno->setVisible(true);
            ui->label_result->setVisible(true);
            ui->label_naideno->setText("    Найдено: 0");
        }
    }
}

void Nagradi::on_pushButton_add_clicked()
{
    try
    {
        Addoredit_Nagradi *form = new Addoredit_Nagradi(this);
        form->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void Nagradi::on_pushButton_edit_clicked()
{
    if(ui->tableView_nagradi->model()->index(ui->tableView_nagradi->currentIndex().row(),1).data().toInt()==dal_main->getCurrentEmployee())
    {
        ui->pushButton_del->setEnabled(true);
        ui->pushButton_edit->setEnabled(true);
    if (! ui->tableView_nagradi->model()->index(ui->tableView_nagradi->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите запись из таблицы"));
        return;
    }
    try
    {
        Addoredit_Nagradi* form = new Addoredit_Nagradi(this, true, ui->tableView_nagradi->model()->index(ui->tableView_nagradi->currentIndex().row(), 0).data().toInt());
        form->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
    }
    else
    {
        ui->pushButton_del->setEnabled(false);
        ui->pushButton_edit->setEnabled(false);
        QMessageBox::information(this,tr("Внимание"),tr("Вы не можете редактировать не свою запись"));
    }
}

void Nagradi::on_pushButton_del_clicked()
{
    if(ui->tableView_nagradi->model()->index(ui->tableView_nagradi->currentIndex().row(),1).data().toInt()==dal_main->getCurrentEmployee())
    {
        ui->pushButton_del->setEnabled(true);
        ui->pushButton_edit->setEnabled(true);
    if (! ui->tableView_nagradi->model()->index(ui->tableView_nagradi->currentIndex().row(), 0).data().toInt())
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
        if (dal_prepodcontrol->deleteNagradi(ui->tableView_nagradi->model()->index(ui->tableView_nagradi->currentIndex().row(), 0).data().toInt()))
        {
            QMessageBox::information(this, tr("Удаление"), tr("Данные успешно удалены"));
        }
        else
        {
            QMessageBox::information(this, tr("Удаление"), tr("Не удалось удалить данные, попробуйте еще раз"));
        }
    }
    this->refreshData();
}
    else
    {
        ui->pushButton_del->setEnabled(false);
        ui->pushButton_edit->setEnabled(false);
        QMessageBox::information(this,tr("Внимание"),tr("Вы не можете удалить не свою запись"));
    }
}

void Nagradi::on_pushButton_update_clicked()
{
    this->refreshData();
}

void Nagradi::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        ui->lineEdit_sotr->clear();
        this->refreshData();
    }
    else
    {
        ui->groupBox_search->setVisible(true);
    }
}

void Nagradi::on_pushButton_find_clicked()
{
    this->refreshData();
}

void Nagradi::on_pushButton_clear_clicked()
{
    ui->lineEdit_sotr->clear();
    ui->label_naideno->setVisible(false);
    ui->label_result->setVisible(false);
    this->refreshData();
}

void Nagradi::on_tableView_nagradi_doubleClicked(const QModelIndex &index)
{
    on_pushButton_edit_clicked();
}

void Nagradi::on_actionEdit_triggered()
{
    on_pushButton_edit_clicked();
}

void Nagradi::on_actionDelete_triggered()
{
    on_pushButton_del_clicked();
}

void Nagradi::on_tableView_nagradi_clicked(const QModelIndex &index)
{
    if(ui->tableView_nagradi->model()->index(ui->tableView_nagradi->currentIndex().row(),1).data().toInt()==dal_main->getCurrentEmployee())
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
