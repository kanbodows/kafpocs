#include "spravkipochas.h"
#include "ui_spravkipochas.h"
#include "delegates/datedelegate.h"
#include "delegates/checkboxdelegate.h"
#include <QMessageBox>
#include <QFileDialog>
#include <qfiledialog.h>
#include <QFile>
#include <QDebug>

bool SpravkiPochas::isOpen = false;
SpravkiPochas::SpravkiPochas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SpravkiPochas)
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
//    dal_prepodcontrol = new Dal_studentsControl(this);
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    ui->dateEdit_end->setDate(QDate::currentDate());
    ui->tableView->setModel(dal_prepodcontrol->getSpravkiPochas(0, ui->dateEdit_start->date(), ui->dateEdit_end->date()));
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnHidden(1,true);
    ui->tableView->setColumnHidden(2,true);
    ui->tableView->setColumnHidden(3,true);
    ui->tableView->setColumnHidden(8,true);
    ui->tableView->setColumnHidden(9,true);
    ui->tableView->setColumnHidden(10,true);
    ui->tableView->setColumnHidden(11,true);
    ui->tableView->setColumnHidden(12,true);
    ui->tableView->setColumnHidden(13,true);
    ui->tableView->setColumnHidden(14,true);
    ui->tableView->setColumnHidden(15,true);
    ui->tableView->setColumnHidden(16,true);
    ui->tableView->setColumnHidden(17,true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    //    ui->tableView->setItemDelegateForColumn(9, new dateDelegate(ui->tableView));
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ui->tableView->addAction(ui->actionEdit);
    //    ui->tableView->addAction(ui->actionDelete);
    ui->pushButton_del->setVisible(false);
}

SpravkiPochas::~SpravkiPochas()
{
    isOpen = false;
    delete ui;
}

void SpravkiPochas::refreshData()
{
    if (!dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableView->setModel(dal_prepodcontrol->getSpravkiPochas(ui->comboBox_sotr->model()->index(ui->comboBox_sotr->currentIndex(),0).data().toInt(),
                                                                ui->dateEdit_start->date(),
                                                                ui->dateEdit_end->date()));
    if(ui->groupBox_search->isVisible())
    {
        if(ui->tableView->model()->rowCount()>0)
        {
            ui->label_result->setVisible(false);
            ui->label_naideno->setVisible(true);
            ui->label_naideno->setText("    Найдено: " + QString::number(ui->tableView->model()->rowCount()));
        }
        else
        {
            ui->label_naideno->setVisible(true);
            ui->label_result->setVisible(true);
            ui->label_naideno->setText("    Найдено: 0");
        }
    }
}

void SpravkiPochas::on_pushButton_add_clicked()
{
    try
    {
        AddOrEdit_SpravkaPochas *form = new AddOrEdit_SpravkaPochas(this);
        form->setWindowFlags(Qt::Window);
        form->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void SpravkiPochas::on_pushButton_update_clicked()
{
    this->refreshData();
}

void SpravkiPochas::on_pushButton_del_clicked()
{
    if (! ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::warning(this, tr("Ошибка удаления"), tr("Ни одной ведомость не выбрано"));
        return;
    }
    if (QMessageBox::warning(this, tr("Удаление записи"), tr("Вы уверены, что хотите удалить запись? \n Восстановить запись невозможно"),
                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        /*if (! dal_main->checkConnection())
        {
            QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
            return;
        }
        if (dal_prepodcontrol->deletePovyskKval(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().toInt()))
        {
            QMessageBox::information(this, tr("Удаление"), tr("Данные успешно удалены"));
        }
        else
        {
            QMessageBox::information(this, tr("Удаление"), tr("Не удалось удалить данные, попробуйте еще раз"));
        }*/
    }
    this->refreshData();
}

void SpravkiPochas::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        this->refreshData();
    }
    else
    {
        ui->groupBox_search->setVisible(true);
        ui->comboBox_sotr->setModel(dal_prepodcontrol->getcomboPrepod());
        ui->comboBox_sotr->setModelColumn(1);
        ui->comboBox_sotr->setCurrentIndex(-1);
    }
}

void SpravkiPochas::on_pushButton_find_clicked()
{
    this->refreshData();
}

void SpravkiPochas::on_pushButton_clear_clicked()
{
    ui->label_naideno->setVisible(false);
    ui->label_result->setVisible(false);
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ui->tableView->setModel(dal_prepodcontrol->getSpravkiPochas(0,
                                                                ui->dateEdit_start->date(),
                                                                ui->dateEdit_end->date()));
    ui->comboBox_sotr->setModel(dal_prepodcontrol->getcomboPrepod());
    ui->comboBox_sotr->setModelColumn(1);
    ui->comboBox_sotr->setCurrentIndex(-1);
    this->refreshData();
}

void SpravkiPochas::on_pushButton_edit_clicked()
{
    if (! ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите ведомость из таблицы"));
        return;
    }
    try
    {
        AddOrEdit_SpravkaPochas* form = new AddOrEdit_SpravkaPochas(this, true, ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().toInt());
        form->setWindowFlags(Qt::Window);
        form->exec();
    }
    catch (...)
    {
        return;
    }

    this->refreshData();
}

void SpravkiPochas::on_actionEdit_triggered()
{
    on_pushButton_edit_clicked();
}

void SpravkiPochas::on_actionDelete_triggered()
{
    on_pushButton_del_clicked();
}

void SpravkiPochas::on_tableView_doubleClicked(const QModelIndex &index)
{
    on_pushButton_edit_clicked();
}
