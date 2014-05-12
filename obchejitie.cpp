#include "obchejitie.h"
#include "ui_obchejitie.h"
#include <QMessageBox>
#include "delegates/textnndelegate.h"
#include "delegates/datedelegate.h"
bool Obchejitie::isOpen = false;
Obchejitie::Obchejitie(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Obchejitie)
{
    ui->setupUi(this);
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
    dal_studentsControl = new Dal_studentsControl(this);
    ui->comboBox_gruppa->setModel(dal_studentsControl->getComboGroup(0));
    ui->comboBox_gruppa->setModelColumn(1);
    ui->comboBox_gruppa->setCurrentIndex(-1);
    ui->tableView->setModel(dal_studentsControl->getObchejitie(0,""));
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnHidden(6,true);
    ui->tableView->setColumnHidden(7,true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ui->tableView->addAction(ui->actionEdit);
    ui->tableView->addAction(ui->actionDelete);
}

Obchejitie::~Obchejitie()
{
    isOpen=false;
    delete ui;
}

void Obchejitie::on_pushButton_add_clicked()
{
    try
    {
        addoredit_obchejitie * form = new addoredit_obchejitie(this);
        form->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void Obchejitie::on_pushButton_del_clicked()
{
    if (! ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::information(this, tr("Информация"), tr("Ни одной записи не выбрано"));
        return;
    }
    if (QMessageBox::information(this, tr("Информация"), tr("Вы уверены, что хотите удалить запись? \n Восстановить запись невозможно"),
                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        if (! dal_main->checkConnection())
        {
            QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
            return;
        }
        if (dal_studentsControl->deleteObshejit(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().toInt()))
        {
            this->refreshData();
            QMessageBox::information(this, tr("Удаление"), tr("Данные успешно удалены"));
        }
        else
        {
            QMessageBox::information(this, tr("Информация"), tr("Не удалось удалить данные, попробуйте еще раз"));
            this->refreshData();
        }
    }
    else
    {
        this->refreshData();
        return;
    }
}

void Obchejitie::on_pushButton_update_clicked()
{
   this->refreshData();
}

void Obchejitie::on_pushButton_find_clicked()
{
    this->refreshData();
}

void Obchejitie::on_pushButton_clear_clicked()
{
    ui->label_naideno->clear();
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ui->comboBox_gruppa->setCurrentIndex(-1);
    this->refreshData();
}

void Obchejitie::refreshData()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableView->setModel(dal_studentsControl->getObchejitie(ui->comboBox_gruppa->model()->index(ui->comboBox_gruppa->currentIndex(),0).data().toInt(),
                                                               ui->lineEdit_student->text()));
    if(ui->groupBox_search->isVisible() == true)
    {
        if(ui->tableView->model()->rowCount()>0)
        {
            ui->label_result->setVisible(false);
        }
        else
        {
            ui->label_result->setVisible(true);
        }
        ui->label_naideno->setVisible(true);
        ui->label_naideno->setText("    Найдено: " + QString::number(ui->tableView->model()->rowCount()));
    }
}


void Obchejitie::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        ui->comboBox_gruppa->setCurrentIndex(-1);
        ui->lineEdit_student->clear();
        ui->label_naideno->setVisible(false);
        refreshData();
    }
    else
    {
        ui->groupBox_search->setVisible(true);
        ui->comboBox_gruppa->setModel(dal_studentsControl->getComboGroup(0));
        ui->comboBox_gruppa->setModelColumn(1);
        ui->comboBox_gruppa->setCurrentIndex(-1);
    }
}

void Obchejitie::on_tableView_doubleClicked(const QModelIndex &index)
{
    on_pushButton_edit_clicked();
}

void Obchejitie::on_pushButton_edit_clicked()
{
    if(!ui->tableView->currentIndex().isValid())
    {
        QMessageBox::information(this, tr("Информация"), tr("Не выбрано ни одной записи"));
        return;
    }
    try
    {
        addoredit_obchejitie * form = new addoredit_obchejitie(this, true,  ui->tableView->model()->index(ui->tableView->currentIndex().row(),0).data().toInt());
        form->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void Obchejitie::on_actionEdit_triggered()
{
    on_pushButton_edit_clicked();
}

void Obchejitie::on_actionDelete_triggered()
{
    on_pushButton_del_clicked();
}
