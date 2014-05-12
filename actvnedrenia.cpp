#include "actvnedrenia.h"
#include "ui_actvnedrenia.h"
#include "delegates/datedelegate.h"
#include <QMessageBox>
#include "delegates/word_wrap_delegate.h"
bool ActVnedrenia::isOpen = false;
ActVnedrenia::ActVnedrenia(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ActVnedrenia)
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
    this->Group="";
    this->dateBeginning = QDate();
    this->dateEnd = ui->dateEdit_end->date();
    this->isDate = false;
    ActVnefreniaModels = dal_studentsControl->getActVnedr(this->Student, this->Group, this->dateBeginning, this->dateEnd);

    ui->tableView_actVnedr->setModel(ActVnefreniaModels);
    ui->tableView_actVnedr->setColumnHidden(0,true);
    ui->tableView_actVnedr->setColumnHidden(1,true);
    ui->tableView_actVnedr->setColumnHidden(2,true);
    ui->tableView_actVnedr->setColumnHidden(3,true);
    ui->tableView_actVnedr->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_actVnedr->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_actVnedr->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_actVnedr->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_actVnedr->verticalHeader()->hide();
    ui->tableView_actVnedr->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView_actVnedr->setItemDelegateForColumn(8, new dateDelegate(ui->tableView_actVnedr));
    ui->tableView_actVnedr->setItemDelegateForColumn(9, new TextEditDelegate(ui->tableView_actVnedr));
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ui->tableView_actVnedr->addAction(ui->actionEdit);
    ui->tableView_actVnedr->addAction(ui->actionDelete);
}

ActVnedrenia::~ActVnedrenia()
{
    isOpen = false;
    delete ui;
}

void ActVnedrenia::refreshData()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableView_actVnedr->setModel(dal_studentsControl->getActVnedr(this->Student, this->Group, this->dateBeginning, this->dateEnd));

    if(this->vidim==true)
    {
        if(ui->tableView_actVnedr->model()->rowCount()>0)
        {
            this->naideno.append("                                   Найдено: " + QString::number(ui->tableView_actVnedr->model()->rowCount()));
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

void ActVnedrenia::on_pushButton_search_clicked()
{

    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        this->Student="";
        this->Group="";
        this->dateBeginning = QDate();
        this->isDate = false;
        ui->lineEdit_student->clear();
        ui->lineEdit_group->clear();
        this->refreshData();
    }
    else
    {
        ui->groupBox_search->setVisible(true);
        ui->dateEdit_end->setDate(QDate::currentDate());
    }
    on_pushButton_clear_clicked();
}

void ActVnedrenia::on_pushButton_find_clicked()
{
    this->vidim=true;
    this->Student = ui->lineEdit_student->text();
    this->Group = ui->lineEdit_group->text();
    if (this->isDate==true)
    {
        this->dateBeginning = ui->dateEdit->date();
        this->dateEnd = ui->dateEdit_end->date();
    }
    this->refreshData();
}

void ActVnedrenia::on_pushButton_clear_clicked()
{
    this->dateBeginning = QDate();
    this->isDate = false;
    this->Student="";
    this->Group="";
    ui->lineEdit_student->clear();
    ui->lineEdit_group->clear();
    ui->label_naideno->setVisible(false);
    ui->label_result->setVisible(false);
    this->vidim=false;
    this->refreshData();
}

void ActVnedrenia::on_dateEdit_dateChanged(const QDate &date)
{
    this->isDate = true;
}

void ActVnedrenia::on_dateEdit_end_dateChanged(const QDate &date)
{
    this->isDate = true;
}

void ActVnedrenia::on_pushButton_del_clicked()
{
    if (! ui->tableView_actVnedr->model()->index(ui->tableView_actVnedr->currentIndex().row(), 0).data().toInt())
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
        if (dal_studentsControl->deleteActVned(ui->tableView_actVnedr->model()->index(ui->tableView_actVnedr->currentIndex().row(), 0).data().toInt()))
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

void ActVnedrenia::on_pushButton_update_clicked()
{
    this->vidim=false;
    this->refreshData();
}

void ActVnedrenia::on_pushButton_add_clicked()
{
    try
    {
        addoreditform = new AddOrEditActVnedrenia(this, INSERT);
        addoreditform->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void ActVnedrenia::on_pushButton_edit_clicked()
{
    if (! ui->tableView_actVnedr->model()->index(ui->tableView_actVnedr->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите запись из таблицы"));
        return;
    }
    int id = ui->tableView_actVnedr->model()->index(ui->tableView_actVnedr->currentIndex().row(), 0).data().toInt();
    try
    {
        addoreditform = new AddOrEditActVnedrenia(this, EDIT, id);
        addoreditform->exec();
    }
    catch (...)
    {
        return;
    }

    this->refreshData();
}


void ActVnedrenia::on_tableView_actVnedr_doubleClicked(const QModelIndex &index)
{
      on_pushButton_edit_clicked();
}

void ActVnedrenia::on_actionEdit_triggered()
{
    on_pushButton_edit_clicked();
}

void ActVnedrenia::on_actionDelete_triggered()
{
    on_pushButton_del_clicked();
}
