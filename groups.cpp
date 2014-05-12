#include "groups.h"
#include "ui_groups.h"
#include <QMessageBox>
#include "delegates/textnndelegate.h"

bool groups::isOpen = false;
groups::groups(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::groups)
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
    groupsModel = new QSqlRelationalTableModel(this);
    groupsModel->setTable("is_group");
    groupsModel->setHeaderData(1,Qt::Horizontal,tr("Специальность"));
    groupsModel->setHeaderData(2,Qt::Horizontal,tr("Название группы"));
    groupsModel->setHeaderData(3,Qt::Horizontal,tr("Доп. инф-я"));
    groupsModel->setHeaderData(4,Qt::Horizontal,tr("Куратор"));
    groupsModel->setHeaderData(5,Qt::Horizontal,tr("Год поступления"));
    groupsModel->setRelation(4, QSqlRelation("is_sotrudniki", "id_sotr", "st_fio"));
    groupsModel->setRelation(1, QSqlRelation("is_specialnosti", "id_specialnosti", "spec_name"));
    groupsModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    groupsModel->select();
    kuratorModel = new QSqlTableModel(this);
    kuratorModel->setTable("is_sotrudniki");
    ui->tableView->setModel(groupsModel);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
   // ui->tableView->setItemDelegateForColumn(1, new textNnDelegate(ui->tableView));
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
}

groups::~groups()
{
    isOpen=false;
    delete ui;
}

void groups::on_pushButton_add_clicked()
{
    groupsModel->insertRow(groupsModel->rowCount());
    ui->tableView->edit(groupsModel->index(groupsModel->rowCount() - 1, 1));
    ui->tableView->setCurrentIndex(groupsModel->index(groupsModel->rowCount()-1,1));
}

void groups::on_pushButton_del_clicked()
{
    if (!ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка удаления"), tr("Ни одной записи не выбрано"));
        return;
    }
    if (this->checkRelation(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().toString()))
    {
        QMessageBox::critical(this, tr("Ошибка удаления"), tr("Невозможно удалить запись, \n так как она используется в других таблицах"));
        return;
    }
    else
    {
        if (QMessageBox::warning(this, tr("Удаление записи"), tr("Вы уверены, что хотите удалить запись? \n Восстановить запись невозможно"),
                                           QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            if (! dal_main->checkConnection())
            {
                QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
                return;
            }
            groupsModel->removeRow(ui->tableView->currentIndex().row());
            groupsModel->submitAll();
            groupsModel->select();
        }
        else
        {
            return;
        }
    }
}

void groups::on_pushButton_update_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }

    groupsModel->select();
    kuratorModel->select();

   if (this->kurator_id != 0)
    {
        for (int i = 0; i < ui->comboBox_kutator->count(); i++)
        {
            if (kuratorModel->index(i, 0).data().toInt() == this->kurator_id)
            {
                ui->comboBox_kutator->setCurrentIndex(i);
                ui->comboBox_kutator->view()->setCurrentIndex(kuratorModel->index(i, 0));
                break;
            }
        }
    }

   QMessageBox::information(this, tr("Информация"), tr("Данные обновлены"));
}

void groups::on_pushButton_find_clicked()
{
    this->naideno.clear();
    ui->label_naideno->setVisible(true);
    this->formingSearch();
}

void groups::on_pushButton_clear_clicked()
{
    this->kurator_id = 0;
    this->naideno.clear();
    ui->label_naideno->clear();
    ui->lineEdit_nazvanie->clear();
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    groupsModel->setFilter("");
    groupsModel->select();
    ui->comboBox_kutator->setModel(kuratorModel);
    ui->comboBox_kutator->setModelColumn(1);
    ui->comboBox_kutator->setCurrentIndex(-1);
}

void groups::on_pushButton_save_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    for(int i=0;i<ui->tableView->model()->rowCount(); i++)
    {
        if(ui->tableView->model()->index(i,1).data().isNull())
        {
            QMessageBox::information(this, tr("Информация"),tr("Поле 'Дисциплина' должно быть заполнено"));
            return;
        }
    }
    groupsModel->submitAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}

bool groups::checkRelation(QString groupe_id)
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return NULL;
    }
    QSqlQuery query;
    query.prepare("select s_group_id from is_student where s_group_id = :groupe_id");
    query.bindValue(":groupe_id", groupe_id);
    query.exec();
    query.next();

    if (query.isValid())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void groups::on_pushButton_cancel_clicked()
{
    groupsModel->revertAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные не сохранены"));
}

void groups::on_comboBox_kutator_activated(int index)
{
    this->kurator_id = ui->comboBox_kutator->model()->index(index,0).data().toInt();
}


void groups::formingSearch()
{
    QString condition = "";

    if (! ui->lineEdit_nazvanie->text().isEmpty())
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " g_name like '%" + ui->lineEdit_nazvanie->text() + "%'";
    }

    if (this->kurator_id != 0)
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " g_kurator = " + QString::number(this->kurator_id);
    }

    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    groupsModel->setFilter(condition);
    if(ui->tableView->model()->rowCount()>0)
    {
    this->naideno.append("    Найдено: " + QString::number(ui->tableView->model()->rowCount()));
    ui->label_result->setVisible(false);
    ui->label_naideno->setText(this->naideno);
    this->naideno.clear();
    }
    else
    {
        this->naideno.append("    Найдено: 0");
        ui->label_result->setVisible(true);
        ui->label_naideno->setText(this->naideno);
        this->naideno.clear();
    }
}


void groups::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        this->kurator_id = 0;
        ui->lineEdit_nazvanie->clear();
        groupsModel->setFilter("");
        ui->label_naideno->setVisible(false);
        on_pushButton_update_clicked();
    }
    else
    {
        if (! dal_main->checkConnection())
        {
            QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
            return;
        }
        ui->groupBox_search->setVisible(true);
        kuratorModel->select();
        ui->comboBox_kutator->setModel(kuratorModel);
        ui->comboBox_kutator->setModelColumn(1);
        ui->comboBox_kutator->setCurrentIndex(-1);
        this->kurator_id = 0;
        connect(ui->lineEdit_nazvanie,SIGNAL(editingFinished()),this,SLOT(formingSearch()));
    }
}

void groups::on_pushButton_edit_clicked()
{
    if (!ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
        return;
    }
    ui->tableView->edit(ui->tableView->currentIndex());
}
