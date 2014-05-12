#include "kur_rab.h"
#include "ui_kur_rab.h"
#include <QMessageBox>
#include "delegates/textnndelegate.h"
#include "delegates/datedelegate.h"
bool Kur_rab::isOpen = false;
Kur_rab::Kur_rab(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Kur_rab)
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
    kur_rabModel = new QSqlRelationalTableModel(this);
    kur_rabModel->setTable("is_kur_rab");
    kur_rabModel->setHeaderData(1,Qt::Horizontal,tr("Преподаватель"));
    kur_rabModel->setHeaderData(2,Qt::Horizontal,tr("Дата"));
    kur_rabModel->setHeaderData(3,Qt::Horizontal,tr("Описание работы"));
    kur_rabModel->setHeaderData(4,Qt::Horizontal,tr("Оценка"));
    kur_rabModel->setRelation(1, QSqlRelation("sotr_kafedri", "id_sotr", "st_fio"));
    kur_rabModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    kur_rabModel->select();
    sotrudnikiModel = new QSqlTableModel(this);
    sotrudnikiModel->setTable("sotr_kafedri");

    ui->tableView->setModel(kur_rabModel);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
    ui->tableView->setItemDelegateForColumn(2, new dateDelegate(ui->tableView));
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ui->lineEdit->setVisible(false);
}

Kur_rab::~Kur_rab()
{
    isOpen=false;
    delete ui;
}

void Kur_rab::on_pushButton_add_clicked()
{
    kur_rabModel->insertRow(kur_rabModel->rowCount());
    ui->tableView->edit(kur_rabModel->index(kur_rabModel->rowCount() - 1, 1));
    ui->tableView->setCurrentIndex(kur_rabModel->index(kur_rabModel->rowCount()-1,1));
}

void Kur_rab::on_pushButton_del_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    QSqlQuery *query = new QSqlQuery;
        query->prepare("select * from is_kur_rab where id_kur_rab = " + QString::number(ui->tableView->model()->index(ui->tableView->currentIndex().row(),0).data().toInt()));
        query->exec();

        query->first();

        ui->lineEdit->setText(query->value(1).toString());

        if(ui->lineEdit->text().toInt()==dal_main->getCurrentEmployee())
        {
            ui->pushButton_del->setEnabled(true);
            ui->pushButton_edit->setEnabled(true);
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
            kur_rabModel->removeRow(ui->tableView->currentIndex().row());
            kur_rabModel->submitAll();
            kur_rabModel->select();
        }
        else
        {
            return;
        }
    }
        }
        else
        {
            ui->pushButton_del->setEnabled(false);
            ui->pushButton_edit->setEnabled(false);
            QMessageBox::information(this,tr("Внимание"),tr("Вы не можете удалить не свою запись"));


        }
}

void Kur_rab::on_pushButton_update_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    kur_rabModel = new QSqlRelationalTableModel(this);
    kur_rabModel->setTable("is_kur_rab");
    kur_rabModel->setHeaderData(1,Qt::Horizontal,tr("Преподаватель"));
    kur_rabModel->setHeaderData(2,Qt::Horizontal,tr("Дата"));
    kur_rabModel->setHeaderData(3,Qt::Horizontal,tr("Описание работы"));
    kur_rabModel->setHeaderData(4,Qt::Horizontal,tr("Оценка"));
    kur_rabModel->setRelation(1, QSqlRelation("sotr_kafedri", "id_sotr", "st_fio"));
    kur_rabModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    kur_rabModel->select();
    sotrudnikiModel = new QSqlTableModel(this);
    sotrudnikiModel->setTable("is_sotrudniki");

    ui->tableView->setModel(kur_rabModel);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
    ui->tableView->setItemDelegateForColumn(2, new dateDelegate(ui->tableView));

   if (this->sotrudnik_id != 0)
    {
        for (int i = 0; i < ui->comboBox_kutator->count(); i++)
        {
            if (sotrudnikiModel->index(i, 0).data().toInt() == this->sotrudnik_id)
            {
                ui->comboBox_kutator->setCurrentIndex(i);
                ui->comboBox_kutator->view()->setCurrentIndex(sotrudnikiModel->index(i, 0));
                break;
            }
        }
    }
}

void Kur_rab::on_pushButton_find_clicked()
{
    this->naideno.clear();
    ui->label_naideno->setVisible(true);
    this->formingSearch();
}

void Kur_rab::on_pushButton_clear_clicked()
{
    this->sotrudnik_id = 0;
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
    kur_rabModel->setFilter("");
    kur_rabModel->select();
    ui->comboBox_kutator->setModel(sotrudnikiModel);
    ui->comboBox_kutator->setModelColumn(1);
    ui->comboBox_kutator->setCurrentIndex(-1);
}

void Kur_rab::on_pushButton_save_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    for(int i=0;i<ui->tableView->model()->rowCount(); i++)
    {
        if(ui->tableView->model()->index(i,1).data().isNull()||ui->tableView->model()->index(i,2).data().isNull()||ui->tableView->model()->index(i,3).data().isNull())
        {
            QMessageBox::information(this, tr("Информация"),tr("Поля 'Преподаватель', 'Дата' и 'Описание работы' должно быть заполнено"));
            return;
        }
    }
    kur_rabModel->submitAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}

bool Kur_rab::checkRelation(QString groupe_id)
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

void Kur_rab::on_pushButton_cancel_clicked()
{
    kur_rabModel->revertAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные не сохранены"));
}

void Kur_rab::on_comboBox_kutator_activated(int index)
{
    this->sotrudnik_id = ui->comboBox_kutator->model()->index(index,0).data().toInt();
}


void Kur_rab::formingSearch()
{
    QString condition = "";

    if (! ui->lineEdit_nazvanie->text().isEmpty())
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " work like '%" + ui->lineEdit_nazvanie->text() + "%'";
    }

    if (this->sotrudnik_id != 0)
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " sotrudnik = " + QString::number(this->sotrudnik_id);
    }

    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    kur_rabModel->setFilter(condition);
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


void Kur_rab::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        this->sotrudnik_id = 0;
        ui->lineEdit_nazvanie->clear();
        kur_rabModel->setFilter("");
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
        sotrudnikiModel->select();
        ui->comboBox_kutator->setModel(sotrudnikiModel);
        ui->comboBox_kutator->setModelColumn(1);
        ui->comboBox_kutator->setCurrentIndex(-1);
        this->sotrudnik_id = 0;
        connect(ui->lineEdit_nazvanie,SIGNAL(editingFinished()),this,SLOT(formingSearch()));
    }
}

void Kur_rab::on_pushButton_edit_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    QSqlQuery *query = new QSqlQuery;
     query->prepare("select * from is_kur_rab where id_kur_rab = " + QString::number(ui->tableView->model()->index(ui->tableView->currentIndex().row(),0).data().toInt()));
     query->exec();

     query->first();

     ui->lineEdit->setText(query->value(1).toString());

     if(ui->lineEdit->text().toInt()==dal_main->getCurrentEmployee())
     {
         ui->pushButton_del->setEnabled(true);
         ui->pushButton_edit->setEnabled(true);
         if (!ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().isValid())
         {
             QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
             return;
         }
         ui->tableView->edit(ui->tableView->currentIndex());
     }
     else
     {
         ui->pushButton_del->setEnabled(false);
         ui->pushButton_edit->setEnabled(false);
         QMessageBox::information(this,tr("Внимание"),tr("Вы не можете редактировать не свою запись"));


     }
}

void Kur_rab::on_tableView_clicked(const QModelIndex &index)
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select * from is_kur_rab where id_kur_rab = " + QString::number(ui->tableView->model()->index(ui->tableView->currentIndex().row(),0).data().toInt()));
    query->exec();

    query->first();

    ui->lineEdit->setText(query->value(1).toString());

    if(ui->lineEdit->text().toInt()==dal_main->getCurrentEmployee())
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
