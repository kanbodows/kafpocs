#include "vedomostikursovye.h"
#include "ui_vedomostikursovye.h"
#include "delegates/datedelegate.h"
#include "delegates/checkboxdelegate.h"
#include <QMessageBox>
#include <QFileDialog>
#include <qfiledialog.h>
#include <QFile>
#include <QDebug>

bool VedomostiKursovye::isOpen = false;
VedomostiKursovye::VedomostiKursovye(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::VedomostiKursovye)
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
    dal_studentsControl = new Dal_studentsControl(this);
    ui->dateEdit_end->setDate(QDate::currentDate());
    ui->tableView_vedom->setModel(dal_studentsControl->getVedomostiKursovye(0, 0, ui->dateEdit_start->date(), ui->dateEdit_end->date()));
    ui->tableView_vedom->setColumnHidden(0,true);
    ui->tableView_vedom->setColumnHidden(1,true);
    ui->tableView_vedom->setColumnHidden(2,true);
    ui->tableView_vedom->setColumnHidden(3,true);
    ui->tableView_vedom->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_vedom->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_vedom->setItemDelegateForColumn(9, new dateDelegate(ui->tableView_vedom));
    ui->tableView_vedom->setItemDelegateForColumn(10, new checkboxdelegate(ui->tableView_vedom));
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ui->tableView_vedom->addAction(ui->actionEdit);
    //    ui->tableView_vedom->addAction(ui->actionDelete);
    ui->pushButton_del->setVisible(false);
    ui->tableView_kursovye->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_kursovye->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_kursovye->verticalHeader()->hide();
    ui->tableView_kursovye->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

VedomostiKursovye::~VedomostiKursovye()
{
    isOpen = false;
    delete ui;
}

void VedomostiKursovye::refreshData()
{
    if (!dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableView_vedom->setModel(dal_studentsControl->getVedomostiKursovye(ui->comboBox_gruppa->model()->index(ui->comboBox_gruppa->currentIndex(),0).data().toInt(),
                                                                            ui->comboBox_discipline->model()->index(ui->comboBox_discipline->currentIndex(),0).data().toInt(),
                                                                            ui->dateEdit_start->date(),
                                                                            ui->dateEdit_end->date()));
    if(ui->groupBox_search->isVisible())
    {
        if(ui->tableView_vedom->model()->rowCount()>0)
        {
            ui->label_result->setVisible(false);
            ui->label_naideno->setVisible(true);
            ui->label_naideno->setText("    Найдено: " + QString::number(ui->tableView_vedom->model()->rowCount()));
        }
        else
        {
            ui->label_naideno->setVisible(true);
            ui->label_result->setVisible(true);
            ui->label_naideno->setText("    Найдено: 0");
        }
    }
}

void VedomostiKursovye::on_pushButton_add_clicked()
{
    try
    {
        AddOrEdit_VedomostiKursovye *form = new AddOrEdit_VedomostiKursovye(this);
        form->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void VedomostiKursovye::on_pushButton_update_clicked()
{
    this->refreshData();
}

void VedomostiKursovye::on_pushButton_del_clicked()
{
    if (! ui->tableView_vedom->model()->index(ui->tableView_vedom->currentIndex().row(), 0).data().toInt())
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
        if (dal_prepodcontrol->deletePovyskKval(ui->tableView_vedom->model()->index(ui->tableView_vedom->currentIndex().row(), 0).data().toInt()))
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

void VedomostiKursovye::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        this->refreshData();
    }
    else
    {
        ui->groupBox_search->setVisible(true);
        ui->comboBox_gruppa->setModel(dal_studentsControl->getComboGroup(0));
        ui->comboBox_gruppa->setModelColumn(1);
        ui->comboBox_gruppa->setCurrentIndex(-1);
        ui->comboBox_discipline->setModel(dal_studentsControl->getDiscipl());
        ui->comboBox_discipline->setModelColumn(1);
        ui->comboBox_discipline->setCurrentIndex(-1);
    }
}

void VedomostiKursovye::on_pushButton_find_clicked()
{
    this->refreshData();
}

void VedomostiKursovye::on_pushButton_clear_clicked()
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
    ui->tableView_vedom->setModel(dal_studentsControl->getVedomostiKursovye(0,
                                                                            0,
                                                                            ui->dateEdit_start->date(),
                                                                            ui->dateEdit_end->date()));
    ui->comboBox_gruppa->setModel(dal_studentsControl->getComboGroup(0));
    ui->comboBox_gruppa->setModelColumn(1);
    ui->comboBox_gruppa->setCurrentIndex(-1);
    ui->comboBox_discipline->setModel(dal_studentsControl->getDiscipl());
    ui->comboBox_discipline->setModelColumn(1);
    ui->comboBox_discipline->setCurrentIndex(-1);
    this->refreshData();
}

void VedomostiKursovye::on_pushButton_edit_clicked()
{
    if (! ui->tableView_vedom->model()->index(ui->tableView_vedom->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите ведомость из таблицы"));
        return;
    }
    try
    {
        AddOrEdit_VedomostiKursovye* form = new AddOrEdit_VedomostiKursovye(this, true, ui->tableView_vedom->model()->index(ui->tableView_vedom->currentIndex().row(), 0).data().toInt());
        form->exec();
    }
    catch (...)
    {
        return;
    }

    this->refreshData();
}

void VedomostiKursovye::on_actionEdit_triggered()
{
    on_pushButton_edit_clicked();
}

void VedomostiKursovye::on_actionDelete_triggered()
{
    on_pushButton_del_clicked();
}

void VedomostiKursovye::on_tableView_vedom_clicked(const QModelIndex &index)
{
    ui->tableView_kursovye->setModel(dal_studentsControl->getCourseWorks(0, 0, "", ui->tableView_vedom->model()->index(ui->tableView_vedom->currentIndex().row(), 0).data().toInt()));
    ui->tableView_kursovye->setColumnHidden(0, true);
    ui->tableView_kursovye->setColumnHidden(1, true);
    ui->tableView_kursovye->setColumnHidden(2, true);
    ui->tableView_kursovye->setColumnHidden(3, true);
    ui->tableView_kursovye->setColumnHidden(4, true);
    ui->tableView_kursovye->setColumnHidden(5, true);
    ui->tableView_kursovye->setColumnHidden(6, true);
    ui->tableView_kursovye->setColumnHidden(10, true);
    ui->tableView_kursovye->setColumnHidden(14, true);
}

void VedomostiKursovye::on_pushButto_spisat_clicked()
{
    if (! ui->tableView_vedom->model()->index(ui->tableView_vedom->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите ведомость из таблицы"));
        return;
    }
    if (ui->tableView_vedom->model()->index(ui->tableView_vedom->currentIndex().row(), 10).data().toInt()==1)
    {
        QMessageBox::information(this, tr("Информация"), tr("Эти работы уже списаны"));
        return;
    }
    try
    {
        AddOrEdit_AktyKursovye *form = new AddOrEdit_AktyKursovye(this, false, ui->tableView_vedom->model()->index(ui->tableView_vedom->currentIndex().row(), 0).data().toInt());
        form->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}
