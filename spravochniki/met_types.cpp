#include "met_types.h"
#include "ui_met_types.h"
#include "delegates/textnndelegate.h"
#include <QMessageBox>
bool Met_types::isOpen = false;
Met_types::Met_types(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Met_types)
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
    met_typesModel = new QSqlTableModel(this);
    met_typesModel->setTable("is_met_types");
    met_typesModel->setHeaderData(1,Qt::Horizontal,tr("Тип методички"));
    met_typesModel->select();
    ui->tableView->setModel(met_typesModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setItemDelegateForColumn(1, new textNnDelegate(ui->tableView));
}

Met_types::~Met_types()
{
    isOpen = false;
    delete ui;
}

void Met_types::on_pushButton_save_clicked()
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
            QMessageBox::information(this, tr("Информация"),tr("Поле 'Тип методички' должно быть заполнено"));
            return;
        }

    }
    met_typesModel->submitAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}

void Met_types::on_pushButton_cancel_clicked()
{
    met_typesModel->revertAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные не сохранены"));
}

void Met_types::on_pushButton_add_clicked()
{
    met_typesModel->insertRow(met_typesModel->rowCount());
    ui->tableView->edit(met_typesModel->index(met_typesModel->rowCount() - 1, 1));
    ui->tableView->setCurrentIndex(met_typesModel->index(met_typesModel->rowCount()-1,1));
}

void Met_types::on_pushButton_edit_clicked()
{
    if (!ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
        return;
    }
    ui->tableView->edit(ui->tableView->currentIndex());
}

void Met_types::on_pushButton_update_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    met_typesModel->select();
}

void Met_types::on_pushButton_del_clicked()
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
            met_typesModel->removeRow(ui->tableView->currentIndex().row());
            met_typesModel->submitAll();
            met_typesModel->select();
        }
        else
        {
            return;
        }
    }
}


bool Met_types::checkRelation(QString id_met_typ)
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return NULL;
    }
    QSqlQuery query;
    query.prepare("select met_type from is_metodichki where met_type = :id_met_typ");
    query.bindValue(":id_met_typ", id_met_typ);
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
