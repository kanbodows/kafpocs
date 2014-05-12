#include "poruchenia.h"
#include "ui_poruchenia.h"
#include "delegates/datedelegate.h"
#include "delegates/word_wrap_delegate.h"
#include <QMessageBox>
bool Poruchenia::isOpen = false;
Poruchenia::Poruchenia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Poruchenia)
{
    ui->setupUi(this);
    if (!this->isOpen)
    {
        this->isOpen = true;
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
    ui->tableView_poruchenia->setModel(dal_prepodcontrol->getPorucheniya(0,""));
    ui->tableView_poruchenia->setColumnHidden(0,true);
    ui->tableView_poruchenia->setColumnHidden(1,true);
    ui->tableView_poruchenia->setColumnHidden(2,true);
    ui->tableView_poruchenia->setColumnHidden(3,true);
    ui->tableView_poruchenia->setColumnHidden(4,true);
    ui->tableView_poruchenia->setColumnHidden(7,true);
    ui->tableView_poruchenia->setColumnHidden(12,true);
    ui->tableView_poruchenia->setColumnHidden(13,true);
    ui->tableView_poruchenia->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_poruchenia->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_poruchenia->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView_poruchenia->verticalHeader()->hide();
    ui->tableView_poruchenia->setItemDelegateForColumn(6, new TextEditDelegate(ui->tableView_poruchenia));
    ui->tableView_poruchenia->setItemDelegateForColumn(11, new dateDelegate(ui->tableView_poruchenia));
    comboStatus = new QSqlTableModel(this);
    comboStatus->setTable("is_poruch_status");
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    this->status_id = 0;
    ui->tableView_poruchenia->addAction(ui->actionEdit);
    ui->tableView_poruchenia->addAction(ui->actionDelete);
}

Poruchenia::~Poruchenia()
{
    this->isOpen = false;
    delete ui;
}

void Poruchenia::on_pushButton_add_clicked()
{
    try
    {
        Addoredit_Poruchenie *form = new Addoredit_Poruchenie(this);
        form->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void Poruchenia::on_pushButton_update_clicked()
{
    this->refreshData();
}

void Poruchenia::refreshData()
{
    if (!dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableView_poruchenia->setModel(dal_prepodcontrol->getPorucheniya(this->status_id, ui->lineEdit_title->text()));
    if(ui->groupBox_search->isVisible())
    {
        if(ui->tableView_poruchenia->model()->rowCount()>0)
        {
            ui->label_result->setVisible(false);
            ui->label_naideno->setVisible(true);
            ui->label_naideno->setText("    Найдено: " + QString::number(ui->tableView_poruchenia->model()->rowCount()));
        }
        else
        {
            ui->label_naideno->setVisible(true);
            ui->label_result->setVisible(true);
            ui->label_naideno->setText("    Найдено: 0");
        }
    }
}

void Poruchenia::on_pushButton_del_clicked()
{
    if(ui->tableView_poruchenia->model()->index(ui->tableView_poruchenia->currentIndex().row(), 2).data().toInt() != dal_main->getCurrentEmployee())
    {
        QMessageBox::warning(this, tr("Ошибка удаления"), tr("Удалять может только менеджер поручения"));
        return;
    }
    if (! ui->tableView_poruchenia->model()->index(ui->tableView_poruchenia->currentIndex().row(), 0).data().toInt())
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
        if (dal_prepodcontrol->deletePoruchenie(ui->tableView_poruchenia->model()->index(ui->tableView_poruchenia->currentIndex().row(), 0).data().toInt()))
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

void Poruchenia::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);

        ui->lineEdit_title->clear();
        this->refreshData();
    }
    else
    {
        ui->groupBox_search->setVisible(true);
    }
}

void Poruchenia::on_pushButton_clear_clicked()
{
    this->status_id = 0;
    ui->label_naideno->setVisible(false);
    ui->label_result->setVisible(false);
    this->refreshData();
}

void Poruchenia::on_comboBox_status_activated(int index)
{
    this->status_id = ui->comboBox_status->model()->index(index,0).data().toInt();
}

void Poruchenia::on_pushButton_find_clicked()
{
    this->refreshData();
}

void Poruchenia::on_pushButton_edit_clicked()
{
    if (! ui->tableView_poruchenia->model()->index(ui->tableView_poruchenia->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите запись из таблицы"));
        return;
    }
    if(ui->tableView_poruchenia->model()->index(ui->tableView_poruchenia->currentIndex().row(), 2).data().toInt()==dal_main->getCurrentEmployee())
    {
        try
        {
            Addoredit_Poruchenie* form = new Addoredit_Poruchenie(this, true, ui->tableView_poruchenia->model()->index(ui->tableView_poruchenia->currentIndex().row(), 0).data().toInt());
            form->exec();
        }
        catch (...)
        {
            return;
        }
    }
    else
    {
        try
        {
            Edit_PoruchStatus* form = new Edit_PoruchStatus(this, true, ui->tableView_poruchenia->model()->index(ui->tableView_poruchenia->currentIndex().row(), 0).data().toInt());
            form->exec();
        }
        catch (...)
        {
            return;
        }
    }
    this->refreshData();   
}

void Poruchenia::on_tableView_poruchenia_doubleClicked(const QModelIndex &index)
{
    on_pushButton_edit_clicked();
}

void Poruchenia::on_actionEdit_triggered()
{
    on_pushButton_edit_clicked();
}

void Poruchenia::on_actionDelete_triggered()
{
    on_pushButton_del_clicked();
}

void Poruchenia::on_tableView_poruchenia_clicked(const QModelIndex &index)
{
if(ui->tableView_poruchenia->model()->index(ui->tableView_poruchenia->currentIndex().row(),2).data().toInt()==dal_main->getCurrentEmployee())
ui->pushButton_del->setEnabled(true);
else
ui->pushButton_del->setEnabled(false);
}
