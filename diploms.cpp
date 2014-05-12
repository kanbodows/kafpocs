#include "diploms.h"
#include "ui_diploms.h"
#include "delegates/word_wrap_delegate.h"
#include "delegates/datedelegate.h"
#include <QMessageBox>
bool Diploms::isOpen = false;
Diploms::Diploms(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Diploms)
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
    dal_studentsControl = new Dal_studentsControl(this);
    this->Student="";
    this->group = 0;

    DiplomsModels = dal_studentsControl->getDiplomsModel(this->Student, this->group);
    ui->tableView_diplom->setModel(DiplomsModels);
    ui->tableView_diplom->setColumnHidden(0,true);
    ui->tableView_diplom->setColumnHidden(1,true);
    ui->tableView_diplom->setColumnHidden(2,true);
    ui->tableView_diplom->setColumnHidden(3,true);
    ui->tableView_diplom->setColumnHidden(4,true);
    ui->tableView_diplom->setColumnHidden(5,true);
    ui->tableView_diplom->setItemDelegateForColumn(7, new TextEditDelegate(ui->tableView_diplom));
    //ui->tableView_diplom->setItemDelegateForColumn(9, new dateDelegate(ui->tableView_diplom));
    ui->tableView_diplom->setItemDelegateForColumn(12, new dateDelegate(ui->tableView_diplom));
    ui->tableView_diplom->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_diplom->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_diplom->verticalHeader()->hide();
    ui->tableView_diplom->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView_diplom->addAction(ui->actionOcenka);
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
}

Diploms::~Diploms()
{
    isOpen = false;
    delete ui;
}

void Diploms::refreshData()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableView_diplom->setModel(dal_studentsControl->getDiplomsModel(this->Student, this->group));

    if(this->vidim==true)
    {
        if(ui->tableView_diplom->model()->rowCount()>0)
        {
            this->naideno.append("                                   Найдено: " + QString::number(ui->tableView_diplom->model()->rowCount()));
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

void Diploms::on_pushButton_search_clicked()
{
    this->vidim = false;
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        this->Student = "";
        ui->lineEdit_name->clear();
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
        ui->comboBox_gruppa->setModel(dal_studentsControl->getComboGroup(0));
        ui->comboBox_gruppa->setModelColumn(1);
        ui->comboBox_gruppa->setCurrentIndex(-1);
        this->group = 0;
    }
}

void Diploms::on_pushButton_clear_clicked()
{
    this->Student = "";
    this->group = 0;
    ui->comboBox_gruppa->setCurrentIndex(-1);
    ui->lineEdit_name->clear();
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
    ui->tableView_diplom->setModel(dal_studentsControl->getDiplomsModel(this->Student, this->group));
}

void Diploms::on_pushButton_find_clicked()
{
    this->vidim=true;
    this->Student = ui->lineEdit_name->text();
    this->refreshData();
}

void Diploms::on_pushButton_del_clicked()
{
    this->vidim = false;
    if (! ui->tableView_diplom->model()->index(ui->tableView_diplom->currentIndex().row(), 0).data().toInt())
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
        if (dal_studentsControl->deleteDiploms(ui->tableView_diplom->model()->index(ui->tableView_diplom->currentIndex().row(), 0).data().toInt()))
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

void Diploms::on_pushButton_update_clicked()
{
    this->vidim=false;
    this->refreshData();
}

void Diploms::on_comboBox_gruppa_activated(int index)
{
    this->group = ui->comboBox_gruppa->model()->index(ui->comboBox_gruppa->currentIndex(), 0).data().toInt();
}

void Diploms::on_pushButton_add_clicked()
{
    try
    {
        AddOrEditDiplomform = new AddOrEditDiplom(this);
        AddOrEditDiplomform->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void Diploms::on_pushButton_postavitOcenki_clicked()
{
    if (! ui->tableView_diplom->model()->index(ui->tableView_diplom->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите запись из таблицы"));
        return;
    }
    int id = ui->tableView_diplom->model()->index(ui->tableView_diplom->currentIndex().row(), 1).data().toInt();
    int rec_id_dipl = ui->tableView_diplom->model()->index(ui->tableView_diplom->currentIndex().row(), 0).data().toInt();
    try
    {
        ProstavitOcenkiForm = new ProstavitOcenki(this, id, rec_id_dipl);
        ProstavitOcenkiForm->exec();
    }
    catch (...)
    {
        return;
    }

    this->refreshData();
}

void Diploms::on_actionOcenka_triggered()
{
  on_pushButton_postavitOcenki_clicked();
}
