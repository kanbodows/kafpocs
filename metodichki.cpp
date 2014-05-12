#include "metodichki.h"
#include "ui_metodichki.h"
#include <QMessageBox>
#include "delegates/textnndelegate.h"
#include "delegates/datedelegate.h"
bool Metodichki::isOpen = false;
Metodichki::Metodichki(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Metodichki)
{
    ui->setupUi(this);

    if (! this->isOpen)
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

    met_typesModel = new QSqlTableModel(this);
    met_typesModel->setTable("is_met_types");
    disciplineModel = new QSqlTableModel(this);
    disciplineModel->setTable("is_discipline");
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    this->discipline=0;
    this->met_type = 0;
    ui->tableView->setModel(dal_prepodcontrol->getMetodichka(this->discipline, this->met_type));
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnHidden(1,true);
    ui->tableView->setColumnHidden(2,true);
    ui->tableView->setColumnHidden(3,true);
    ui->tableView->setColumnHidden(10,true);
    ui->tableView->setColumnHidden(11,true);
    ui->tableView->setItemDelegateForColumn(8, new dateDelegate(ui->tableView));
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ui->tableView->addAction(ui->actionOpenFile);
    ui->tableView->addAction(ui->actionEdit);
    ui->tableView->addAction(ui->actionDelete);
}

Metodichki::~Metodichki()
{
    this->isOpen=false;
    delete ui;
}

void Metodichki::on_pushButton_add_clicked()
{
    try
    {
        addoredit_metodichki * form = new addoredit_metodichki(this,false,-1,-1);
        form->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void Metodichki::on_pushButton_del_clicked()
{
    if (! ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().toInt())
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
        if (dal_prepodcontrol->deleteMetodichka(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().toInt()))
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

void Metodichki::on_pushButton_update_clicked()
{
    this->refreshData();
}

void Metodichki::on_pushButton_find_clicked()
{
    this->refreshData();
}

void Metodichki::on_pushButton_clear_clicked()
{
    this->discipline = 0;
    this->met_type = 0;
    ui->label_naideno->clear();
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ui->tableView->setModel(dal_prepodcontrol->getMetodichka(this->discipline, this->met_type));
    ui->comboBox_discipline->setModel(disciplineModel);
    ui->comboBox_discipline->setModelColumn(1);
    ui->comboBox_discipline->setCurrentIndex(-1);
    ui->comboBox_met_type->setModel(met_typesModel);
    ui->comboBox_met_type->setModelColumn(1);
    ui->comboBox_met_type->setCurrentIndex(-1);
}

void Metodichki::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        this->discipline = 0;
        this->met_type = 0;
        ui->label_naideno->setVisible(false);
        refreshData();
    }
    else
    {
        if (! dal_main->checkConnection())
        {
            QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
            return;
        }
        ui->groupBox_search->setVisible(true);
        met_typesModel->select();
        disciplineModel->select();
        ui->comboBox_met_type->setModel(met_typesModel);
        ui->comboBox_met_type->setModelColumn(1);
        ui->comboBox_met_type->setCurrentIndex(-1);
        ui->comboBox_discipline->setModel(disciplineModel);
        ui->comboBox_discipline->setModelColumn(1);
        ui->comboBox_discipline->setCurrentIndex(-1);
    }
}

void Metodichki::on_comboBox_met_type_activated(int index)
{
    this->met_type = ui->comboBox_met_type->model()->index(index,0).data().toInt();
}

void Metodichki::on_comboBox_discipline_activated(int index)
{
    this->discipline = ui->comboBox_discipline->model()->index(index,0).data().toInt();
}

void Metodichki::on_pushButton_edit_clicked()
{
    if (!ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка удаления"), tr("Ни одной записи не выбрано"));
        return;
    }
    try
    {
        addoredit_metodichki * form = new addoredit_metodichki(this,true,ui->tableView->model()->index(ui->tableView->currentIndex().row(),0).data().toInt());
        form->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void Metodichki::on_tableView_doubleClicked(const QModelIndex &index)
{
    on_pushButton_edit_clicked();
}

void Metodichki::refreshData()
{
    if (!dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableView->setModel(dal_prepodcontrol->getMetodichka(this->discipline, this->met_type));
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

void Metodichki::on_actionEdit_triggered()
{
    on_pushButton_edit_clicked();
}

void Metodichki::on_actionDelete_triggered()
{
    on_pushButton_del_clicked();
}

void Metodichki::on_actionOpenFile_triggered()
{
    QProcess* proc=new QProcess(this);
    proc->start("explorer " + ui->tableView->model()->index(ui->tableView->currentIndex().row(),11).data().toString());
}
