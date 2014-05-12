#include "lic_docs.h"
#include "ui_lic_docs.h"
#include <QMessageBox>
#include "delegates/textnndelegate.h"
#include "delegates/datedelegate.h"
bool Lic_docs::isOpen = false;
Lic_docs::Lic_docs(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Lic_docs)
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
    lic_docsModel = new QSqlRelationalTableModel(this);
    lic_docsModel->setTable("is_lic_docs");
    lic_docsModel->setHeaderData(1,Qt::Horizontal,tr("Дата"));
    lic_docsModel->setHeaderData(2,Qt::Horizontal,tr("Тип лицензии"));
    lic_docsModel->setHeaderData(3,Qt::Horizontal,tr("Тип документа"));
    lic_docsModel->setHeaderData(4,Qt::Horizontal,tr("Название Файла"));
    lic_docsModel->setRelation(2, QSqlRelation("is_lic_types", "id_lic", "lic_type"));
    lic_docsModel->setRelation(3, QSqlRelation("is_lic_doc_types", "id_lic_doc", "lic_doc_type"));
    lic_docsModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    lic_docsModel->select();
    type_licModel = new QSqlTableModel(this);
    type_licModel->setTable("is_lic_types");
    type_docModel = new QSqlTableModel(this);
    type_docModel->setTable("is_lic_doc_types");
    ui->tableView->setModel(lic_docsModel);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));   
//    ui->tableView->setItemDelegateForColumn(1, new textNnDelegate(ui->tableView));
    ui->tableView->setItemDelegateForColumn(1, new dateDelegate(ui->tableView));
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    this->count=0;
    this->type_doc=0;
    this->type_lic=0;
    this->dateBeginning = QDate();
    this->dateEnd = ui->dateEdit_end->date();
    this->isDate = false;
}

Lic_docs::~Lic_docs()
{
    isOpen=false;
    delete ui;
}

void Lic_docs::on_pushButton_add_clicked()
{
    lic_docsModel->insertRow(lic_docsModel->rowCount());
    ui->tableView->edit(lic_docsModel->index(lic_docsModel->rowCount() - 1, 1));
    ui->tableView->setCurrentIndex(lic_docsModel->index(lic_docsModel->rowCount()-1,1));
}

void Lic_docs::on_pushButton_del_clicked()
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
            lic_docsModel->removeRow(ui->tableView->currentIndex().row());
            lic_docsModel->submitAll();
            lic_docsModel->select();
        }
        else
        {
            return;
        }
    }
}

void Lic_docs::on_pushButton_update_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }

    lic_docsModel->select();
    type_licModel->select();
    type_docModel->select();

   if (this->type_doc != 0)
    {
        for (int i = 0; i < ui->comboBox_type_doc->count(); i++)
        {
            if (type_docModel->index(i, 0).data().toInt() == this->type_doc)
            {
                ui->comboBox_type_doc->setCurrentIndex(i);
                ui->comboBox_type_doc->view()->setCurrentIndex(type_docModel->index(i, 0));
                break;
            }
        }
    }

   if (this->type_lic != 0)
    {
        for (int i = 0; i < ui->comboBox_type_lic->count(); i++)
        {
            if (type_licModel->index(i, 0).data().toInt() == this->type_lic)
            {
                ui->comboBox_type_lic->setCurrentIndex(i);
                ui->comboBox_type_lic->view()->setCurrentIndex(type_licModel->index(i, 0));
                break;
            }
        }
    }

   QMessageBox::information(this, tr("Информация"), tr("Данные обновлены"));
}

void Lic_docs::on_pushButton_find_clicked()
{
    this->naideno.clear();
    ui->label_naideno->setVisible(true);
    if (this->isDate)
    {
        this->dateBeginning = ui->dateEdit_start->date();
        this->dateEnd = ui->dateEdit_end->date();
    }
    this->formingSearch();
}

void Lic_docs::on_pushButton_clear_clicked()
{
    this->type_doc = 0;
    this->type_lic = 0;
    this->naideno.clear();
    ui->label_naideno->clear();
    this->dateBeginning = QDate();
    this->isDate = false;
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    lic_docsModel->setFilter("");
    lic_docsModel->select();
    ui->comboBox_type_doc->setModel(type_docModel);
    ui->comboBox_type_doc->setModelColumn(1);
    ui->comboBox_type_doc->setCurrentIndex(-1);

    ui->comboBox_type_lic->setModel(type_licModel);
    ui->comboBox_type_lic->setModelColumn(1);
    ui->comboBox_type_lic->setCurrentIndex(-1);
}

void Lic_docs::on_pushButton_save_clicked()
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
    lic_docsModel->submitAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}
/*
bool Lic_docs::checkRelation(QString groupe_id)
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
void Lic_docs::on_pushButton_cancel_clicked()
{
    lic_docsModel->revertAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные не сохранены"));
}
/*
void Lic_docs::on_comboBox_kutator_activated(int index)
{
    this->sotrudnik_id = ui->comboBox_kutator->model()->index(index,0).data().toInt();
}
*/

void Lic_docs::formingSearch()
{
    qDebug()<<this->type_doc<<" "<<this->type_lic;
    QString condition = "";

    if (this->type_doc!=0 )
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
         condition = condition + " is_lic_docs.`lic_doc_type_id` = " + QString::number(this->type_doc);
    }

    if (this->type_lic != 0)
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " is_lic_docs.`lic_type_id`= " + QString::number(this->type_lic);
    }

    if(!this->dateBeginning.isNull())
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " is_lic_docs.`year` between '" + dateBeginning.toString("yyyy-MM-dd") + "' and '" + dateEnd.toString("yyyy-MM-dd 23:59:59") + "'";
    }

    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    lic_docsModel->setFilter(condition);
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


void Lic_docs::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        this->type_doc = 0;
        this->type_lic = 0;
        this->dateBeginning = QDate();
        this->isDate = false;
        lic_docsModel->setFilter("");
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
        type_licModel->select();
        type_docModel->select();
        ui->comboBox_type_lic->setModel(type_licModel);
        ui->comboBox_type_lic->setModelColumn(1);
        ui->comboBox_type_lic->setCurrentIndex(-1);

        ui->comboBox_type_doc->setModel(type_docModel);
        ui->comboBox_type_doc->setModelColumn(1);
        ui->comboBox_type_doc->setCurrentIndex(-1);
        this->type_doc = 0;
        this->type_lic = 0;
         ui->dateEdit_end->setDate(QDate::currentDate());

    }
}

void Lic_docs::on_comboBox_type_lic_activated(int index)
{
     this->type_lic = ui->comboBox_type_lic->model()->index(index,0).data().toInt();
}

void Lic_docs::on_comboBox_type_doc_activated(int index)
{
    this->type_doc = ui->comboBox_type_doc->model()->index(index,0).data().toInt();
}

void Lic_docs::on_dateEdit_start_dateChanged(const QDate &date)
{
    this->isDate=true;
}

void Lic_docs::on_dateEdit_end_dateChanged(const QDate &date)
{
    this->isDate=true;
}

void Lic_docs::on_pushButton_edit_clicked()
{
    if (!ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
        return;
    }
    ui->tableView->edit(ui->tableView->currentIndex());
}
