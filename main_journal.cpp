#include "main_journal.h"
#include "ui_main_journal.h"
#include <QMessageBox>
#include "delegates/textnndelegate.h"
#include "delegates/datedelegate.h"
bool Main_journal::isOpen = false;
Main_journal::Main_journal(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Main_journal)
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
    main_journalModel = new QSqlRelationalTableModel(this);
    main_journalModel->setTable("is_main_journal");
    main_journalModel->setHeaderData(1,Qt::Horizontal,tr("Студент"));
    main_journalModel->setHeaderData(2,Qt::Horizontal,tr("Предмет"));
    main_journalModel->setHeaderData(3,Qt::Horizontal,tr("Модуль 1"));
    main_journalModel->setHeaderData(4,Qt::Horizontal,tr("Модуль 2"));
    main_journalModel->setHeaderData(5,Qt::Horizontal,tr("Итог"));
    main_journalModel->setRelation(1, QSqlRelation("is_student", "id_student", "s_fio"));
    main_journalModel->setRelation(2, QSqlRelation("is_discipline", "id_discipline", "d_name"));
    main_journalModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    main_journalModel->select();
    studentModel = new QSqlTableModel(this);
    studentModel->setTable("is_student");
    disciplineModel = new QSqlTableModel(this);
    disciplineModel->setTable("is_discipline");
    ui->tableView->setModel(main_journalModel);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
//    ui->tableView->setItemDelegateForColumn(1, new dateDelegate(ui->tableView));
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    this->count=0;
    this->discipline=0;
    this->student=0;
}

Main_journal::~Main_journal()
{
    isOpen=false;
    delete ui;
}

void Main_journal::on_pushButton_add_clicked()
{
    main_journalModel->insertRow(main_journalModel->rowCount());
    ui->tableView->edit(main_journalModel->index(main_journalModel->rowCount() - 1, 1));
    ui->tableView->setCurrentIndex(main_journalModel->index(main_journalModel->rowCount()-1,1));
}

void Main_journal::on_pushButton_del_clicked()
{
    if (!ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка удаления"), tr("Ни одной записи не выбрано"));
        return;
    }
   /* if (this->checkRelation(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().toString()))
    {
        QMessageBox::critical(this, tr("Ошибка удаления"), tr("Невозможно удалить запись, \n так как она используется в других таблицах"));
        return;
    }*/
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
            main_journalModel->removeRow(ui->tableView->currentIndex().row());
            main_journalModel->submitAll();
            main_journalModel->select();
        }
        else
        {
            return;
        }
    }
}

void Main_journal::on_pushButton_update_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }

    main_journalModel->select();
    studentModel->select();
    disciplineModel->select();

   if (this->discipline != 0)
    {
        for (int i = 0; i < ui->comboBox_discipline->count(); i++)
        {
            if (disciplineModel->index(i, 0).data().toInt() == this->discipline)
            {
                ui->comboBox_discipline->setCurrentIndex(i);
                ui->comboBox_discipline->view()->setCurrentIndex(disciplineModel->index(i, 0));
                break;
            }
        }
    }

   if (this->student != 0)
    {
        for (int i = 0; i < ui->comboBox_student->count(); i++)
        {
            if (studentModel->index(i, 0).data().toInt() == this->student)
            {
                ui->comboBox_student->setCurrentIndex(i);
                ui->comboBox_student->view()->setCurrentIndex(studentModel->index(i, 0));
                break;
            }
        }
    }
}

void Main_journal::on_pushButton_find_clicked()
{
    this->naideno.clear();
    ui->label_naideno->setVisible(true);
    this->formingSearch();
}

void Main_journal::on_pushButton_clear_clicked()
{
    this->discipline = 0;
    this->student = 0;
    this->naideno.clear();
    ui->label_naideno->clear();
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    main_journalModel->setFilter("");
    main_journalModel->select();
    ui->comboBox_discipline->setModel(disciplineModel);
    ui->comboBox_discipline->setModelColumn(1);
    ui->comboBox_discipline->setCurrentIndex(-1);

    ui->comboBox_student->setModel(studentModel);
    ui->comboBox_student->setModelColumn(1);
    ui->comboBox_student->setCurrentIndex(-1);
}

void Main_journal::on_pushButton_save_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    for(int i=0;i<ui->tableView->model()->rowCount(); i++)
    {
        if(ui->tableView->model()->index(i,2).data().isNull() || ui->tableView->model()->index(i,3).data().isNull())
        {
            QMessageBox::information(this, tr("Информация"),tr("Поля 'Тип лицензии' и 'Тип документа' должны быть заполнены"));
            return;
        }
    }
    main_journalModel->submitAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}
/*
bool Main_journal::checkRelation(QString groupe_id)
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
*/
void Main_journal::on_pushButton_cancel_clicked()
{
    main_journalModel->revertAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные не сохранены"));
}
/*
void Main_journal::on_comboBox_kutator_activated(int index)
{
    this->sotrudnik_id = ui->comboBox_kutator->model()->index(index,0).data().toInt();
}
*/

void Main_journal::formingSearch()
{
    QString condition = "";

    if (this->discipline!=0 )
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
         condition = condition + " is_main_journal.`predmet` = " + QString::number(this->discipline);
    }

    if (this->student != 0)
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " is_main_journal.`student`= " + QString::number(this->student);
    }

    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    main_journalModel->setFilter(condition);
    qDebug()<<condition;
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


void Main_journal::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        this->discipline = 0;
        this->student = 0;
        main_journalModel->setFilter("");
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
        studentModel->select();
        disciplineModel->select();
        ui->comboBox_student->setModel(studentModel);
        ui->comboBox_student->setModelColumn(1);
        ui->comboBox_student->setCurrentIndex(-1);

        ui->comboBox_discipline->setModel(disciplineModel);
        ui->comboBox_discipline->setModelColumn(1);
        ui->comboBox_discipline->setCurrentIndex(-1);
        this->discipline = 0;
        this->student = 0;

    }
}

void Main_journal::on_comboBox_student_activated(int index)
{
     this->student = ui->comboBox_student->model()->index(index,0).data().toInt();
}

void Main_journal::on_comboBox_discipline_activated(int index)
{
    this->discipline = ui->comboBox_discipline->model()->index(index,0).data().toInt();
}
