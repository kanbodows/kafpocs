#include "lic_doc_types.h"
#include "ui_lic_doc_types.h"
#include "delegates/textnndelegate.h"
#include <QMessageBox>

bool Lic_doc_types::isOpen = false;
Lic_doc_types::Lic_doc_types(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Lic_doc_types)
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
    lic_doc_typesModel = new QSqlTableModel(this);
    lic_doc_typesModel->setTable("is_lic_doc_types");
    lic_doc_typesModel->setHeaderData(1,Qt::Horizontal,tr("Тип документа"));
    lic_doc_typesModel->select();
    ui->tableView->setModel(lic_doc_typesModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setItemDelegateForColumn(1, new textNnDelegate(ui->tableView));
}

Lic_doc_types::~Lic_doc_types()
{
    isOpen = false;
    delete ui;
}

void Lic_doc_types::on_pushButton_save_clicked()
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
            QMessageBox::information(this, tr("Информация"),tr("Поле 'Должности' должно быть заполнено"));
            return;
        }

    }
    lic_doc_typesModel->submitAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}

void Lic_doc_types::on_pushButton_cancel_clicked()
{
    lic_doc_typesModel->revertAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные не сохранены"));
}

void Lic_doc_types::on_pushButton_add_clicked()
{
    lic_doc_typesModel->insertRow(lic_doc_typesModel->rowCount());
    ui->tableView->edit(lic_doc_typesModel->index(lic_doc_typesModel->rowCount() - 1, 1));
    ui->tableView->setCurrentIndex(lic_doc_typesModel->index(lic_doc_typesModel->rowCount()-1,1));
}

void Lic_doc_types::on_pushButton_edit_clicked()
{
    if (!ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
        return;
    }
    ui->tableView->edit(ui->tableView->currentIndex());
}

void Lic_doc_types::on_pushButton_update_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    lic_doc_typesModel->select();
}

void Lic_doc_types::on_pushButton_del_clicked()
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
            lic_doc_typesModel->removeRow(ui->tableView->currentIndex().row());
            lic_doc_typesModel->submitAll();
            lic_doc_typesModel->select();
        }
        else
        {
            return;
        }
    }
}


bool Lic_doc_types::checkRelation(QString lic_doc_type_id)
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return NULL;
    }
    QSqlQuery query;
    query.prepare("select lic_doc_type from is_lic_docs where lic_doc_type = :lic_doc_type_id");
    query.bindValue(":lic_doc_type_id", lic_doc_type_id);
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
