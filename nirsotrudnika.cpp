#include "nirsotrudnika.h"
#include "ui_nirsotrudnika.h"
#include <QMessageBox>

bool NirSotrudnika::isOpen = false;
NirSotrudnika::NirSotrudnika(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::NirSotrudnika)
{
    ui->setupUi(this);
    ui->lineEdit_number_project->setValidator( new QIntValidator(1, 1000000, this));
    ui->lineEdit_number_project->setPlaceholderText("Вводятся только цифры");
    this->count=0;
    ui->lineEdit->setVisible(false);
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
    nir_sotrTable = new QSqlRelationalTableModel(this);
    nir_sotrTable->setTable("is_nir_sotr");
    nir_sotrTable->setHeaderData(1,Qt::Horizontal,tr("Преподаватель"));
    nir_sotrTable->setHeaderData(2,Qt::Horizontal,tr("Номер проекта"));
    nir_sotrTable->setHeaderData(3,Qt::Horizontal,tr("Финансы"));
    nir_sotrTable->setHeaderData(4,Qt::Horizontal,tr("Тип НИР"));
    nir_sotrTable->setRelation(1, QSqlRelation("sotr_kafedri", "id_sotr", "st_fio"));
    nir_sotrTable->setRelation(4, QSqlRelation("is_type_nir", "id_type_nir", "type_nir"));


    nir_sotrTable->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    nir_sotrTable->select();

    ComboSotr = new QSqlTableModel(this);
    ComboSotr->setTable("sotr_kafedri");

    ui->tableView_nirSotr->setModel(nir_sotrTable);
    ui->tableView_nirSotr->setColumnHidden(0,true);
    ui->tableView_nirSotr->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_nirSotr->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_nirSotr->setItemDelegate(new QSqlRelationalDelegate(ui->tableView_nirSotr));

    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
}

NirSotrudnika::~NirSotrudnika()
{
    this->isOpen = false;
    delete ui;
}

void NirSotrudnika::formingSearch()
{
    QString condition = "";

    if (! ui->lineEdit_number_project->text().isEmpty())
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " nir_sotr_nomer_proekta = " + ui->lineEdit_number_project->text();
    }

    if (this->nir_sotr_id != 0)
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " nir_sotr_prepodavatel = " + QString::number(this->nir_sotr_id);
    }

    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    nir_sotrTable->setFilter(condition);
    if(ui->tableView_nirSotr->model()->rowCount()>0)
    {
        this->naideno.append("    Найдено: " + QString::number(ui->tableView_nirSotr->model()->rowCount()));
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

void NirSotrudnika::on_pushButton_add_clicked()
{
    nir_sotrTable->insertRow(nir_sotrTable->rowCount());
    ui->tableView_nirSotr->edit(nir_sotrTable->index(nir_sotrTable->rowCount() - 1, 1));
    ui->tableView_nirSotr->setCurrentIndex(nir_sotrTable->index(nir_sotrTable->rowCount()-1,1));
}

void NirSotrudnika::on_pushButton_update_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    nir_sotrTable = new QSqlRelationalTableModel(this);
    nir_sotrTable->setTable("is_nir_sotr");
    nir_sotrTable->setHeaderData(1,Qt::Horizontal,tr("Преподаватель"));
    nir_sotrTable->setHeaderData(2,Qt::Horizontal,tr("Номер проекта"));
    nir_sotrTable->setHeaderData(3,Qt::Horizontal,tr("Финансы"));
    nir_sotrTable->setRelation(1, QSqlRelation("sotr_kafedri", "id_sotr", "st_fio"));
    nir_sotrTable->setRelation(4, QSqlRelation("is_type_nir", "id_type_nir", "type_nir"));
    nir_sotrTable->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    nir_sotrTable->select();

    ComboSotr = new QSqlTableModel(this);
    ComboSotr->setTable("sotr_kafedri");

    ui->tableView_nirSotr->setModel(nir_sotrTable);
    ui->tableView_nirSotr->setColumnHidden(0,true);
    ui->tableView_nirSotr->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_nirSotr->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_nirSotr->setItemDelegate(new QSqlRelationalDelegate(ui->tableView_nirSotr));

    if (this->nir_sotr_id != 0)
    {
        for (int i = 0; i < ui->comboBox_sotrudnik->count(); i++)
        {
            if (ComboSotr->index(i, 0).data().toInt() == this->nir_sotr_id)
            {
                ui->comboBox_sotrudnik->setCurrentIndex(i);
                ui->comboBox_sotrudnik->view()->setCurrentIndex(ComboSotr->index(i, 0));
                break;
            }
        }
    }
}

void NirSotrudnika::on_pushButton_del_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select * from is_nir_sotr where nir_sotr_id = " + QString::number(ui->tableView_nirSotr->model()->index(ui->tableView_nirSotr->currentIndex().row(),0).data().toInt()));
    query->exec();

    query->first();

    ui->lineEdit->setText(query->value(1).toString());

    if(ui->lineEdit->text().toInt()==dal_main->getCurrentEmployee())
    {
        ui->pushButton_del->setEnabled(true);
        ui->pushButton_edit->setEnabled(true);
        if (!ui->tableView_nirSotr->model()->index(ui->tableView_nirSotr->currentIndex().row(), 0).data().isValid())
        {
            QMessageBox::warning(this, tr("Ошибка удаления"), tr("Ни одной записи не выбрано"));
            return;
        }

        if (QMessageBox::warning(this, tr("Удаление записи"), tr("Вы уверены, что хотите удалить запись? \n Все данные связанные с записью также будут удалены"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            if (! dal_main->checkConnection())
            {
                QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
                return;
            }
            nir_sotrTable->removeRow(ui->tableView_nirSotr->currentIndex().row());
            nir_sotrTable->submitAll();
            nir_sotrTable->select();
        }
        else
        {
            return;
        }
    }
    else
    {
        ui->pushButton_del->setEnabled(false);
        ui->pushButton_edit->setEnabled(false);
        QMessageBox::information(this,tr("Внимание"),tr("Вы не можете удалить не свою запись"));


    }

}

void NirSotrudnika::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        this->nir_sotr_id = 0;
        ui->lineEdit_number_project->clear();
        nir_sotrTable->setFilter("");
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
        ComboSotr->select();
        ui->comboBox_sotrudnik->setModel(ComboSotr);
        ui->comboBox_sotrudnik->setModelColumn(1);
        ui->comboBox_sotrudnik->setCurrentIndex(-1);
        this->nir_sotr_id = 0;
        connect(ui->lineEdit_number_project,SIGNAL(editingFinished()),this,SLOT(formingSearch()));
    }
}

void NirSotrudnika::on_comboBox_sotrudnik_activated(int index)
{
    this->nir_sotr_id = ui->comboBox_sotrudnik->model()->index(index,0).data().toInt();
}

void NirSotrudnika::on_pushButton_find_clicked()
{
    this->naideno.clear();
    ui->label_naideno->setVisible(true);
    this->formingSearch();
}

void NirSotrudnika::on_pushButton_clear_clicked()
{
    this->nir_sotr_id = 0;
    this->naideno.clear();
    ui->label_naideno->clear();
    ui->lineEdit_number_project->clear();
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    nir_sotrTable->setFilter("");
    nir_sotrTable->select();
    ui->comboBox_sotrudnik->setModel(ComboSotr);
    ui->comboBox_sotrudnik->setModelColumn(1);
    ui->comboBox_sotrudnik->setCurrentIndex(-1);
}

void NirSotrudnika::on_pushButton_save_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    for(int i=0;i<ui->tableView_nirSotr->model()->rowCount(); i++)
    {
        if(ui->tableView_nirSotr->model()->index(i,1).data().isNull()||ui->tableView_nirSotr->model()->index(i,2).data().isNull()||ui->tableView_nirSotr->model()->index(i,3).data().isNull())
        {
            QMessageBox::information(this, tr("Информация"),tr("Поля 'Преподаватель', 'Номер проекта' и 'Финансы' должно быть заполнено"));
            return;
        }
    }

    for(int i=0;i<ui->tableView_nirSotr->model()->rowCount(); i++)
    {
        if(i==ui->tableView_nirSotr->currentIndex().row())
            continue;
        if(ui->tableView_nirSotr->model()->index(ui->tableView_nirSotr->currentIndex().row(),2).data().toInt()==ui->tableView_nirSotr->model()->index(i,2).data().toInt())
        {
            QMessageBox::information(this, tr("Информация"),tr("Поле 'Номер проекта' не должно повторяться"));
            return;
        }
    }
    nir_sotrTable->submitAll();

    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}

void NirSotrudnika::on_pushButton_cancel_clicked()
{
    nir_sotrTable->revertAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные не сохранены"));
}

void NirSotrudnika::on_pushButton_edit_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select * from is_nir_sotr where nir_sotr_id = " + QString::number(ui->tableView_nirSotr->model()->index(ui->tableView_nirSotr->currentIndex().row(),0).data().toInt()));
    query->exec();

    query->first();

    ui->lineEdit->setText(query->value(1).toString());

    if(ui->lineEdit->text().toInt()==dal_main->getCurrentEmployee())
    {
        ui->pushButton_del->setEnabled(true);
        ui->pushButton_edit->setEnabled(true);
        if (!ui->tableView_nirSotr->model()->index(ui->tableView_nirSotr->currentIndex().row(), 0).data().isValid())
        {
            QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
            return;
        }
        ui->tableView_nirSotr->edit(ui->tableView_nirSotr->currentIndex());
    }
    else
    {
        ui->pushButton_del->setEnabled(false);
        ui->pushButton_edit->setEnabled(false);
        QMessageBox::information(this,tr("Внимание"),tr("Вы не можете редактировать не свою запись"));


    }
}

void NirSotrudnika::on_tableView_nirSotr_clicked(const QModelIndex &index)
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select * from is_nir_sotr where nir_sotr_id = " + QString::number(ui->tableView_nirSotr->model()->index(ui->tableView_nirSotr->currentIndex().row(),0).data().toInt()));
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
