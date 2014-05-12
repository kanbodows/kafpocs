#include "poruch_status.h"
#include "ui_poruch_status.h"
#include "delegates/textnndelegate.h"
#include "delegates/checkboxdelegate.h"
#include <QMessageBox>
bool Poruch_status::isOpen = false;
Poruch_status::Poruch_status(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Poruch_status)
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
    poruch_statusModel = new QSqlTableModel(this);
    poruch_statusModel->setTable("is_poruch_status");
    poruch_statusModel->setHeaderData(1,Qt::Horizontal,tr("Статус поручения"));
    poruch_statusModel->setHeaderData(2,Qt::Horizontal,tr("Назначающий поручение"));
    poruch_statusModel->setHeaderData(3,Qt::Horizontal,tr("Исполнитель поручения"));
    poruch_statusModel->setEditStrategy(QSqlTableModel::OnManualSubmit);
    poruch_statusModel->select();
    ui->tableView->setModel(poruch_statusModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setItemDelegateForColumn(1, new textNnDelegate(ui->tableView));
    ui->tableView->setItemDelegateForColumn(2, new checkboxdelegate(ui->tableView));
    ui->tableView->setItemDelegateForColumn(3, new checkboxdelegate(ui->tableView));
}

Poruch_status::~Poruch_status()
{
    isOpen = false;
    delete ui;
}

void Poruch_status::on_pushButton_save_clicked()
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
    poruch_statusModel->submitAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}

void Poruch_status::on_pushButton_cancel_clicked()
{
    poruch_statusModel->revertAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные не сохранены"));
}

void Poruch_status::on_pushButton_add_clicked()
{
    poruch_statusModel->insertRow(poruch_statusModel->rowCount());
    ui->tableView->edit(poruch_statusModel->index(poruch_statusModel->rowCount() - 1, 1));
    ui->tableView->setCurrentIndex(poruch_statusModel->index(poruch_statusModel->rowCount()-1,1));
}

void Poruch_status::on_pushButton_edit_clicked()
{
    if (!ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
        return;
    }
    ui->tableView->edit(ui->tableView->currentIndex());
}

void Poruch_status::on_pushButton_update_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    poruch_statusModel->select();
}

void Poruch_status::on_pushButton_del_clicked()
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
            poruch_statusModel->removeRow(ui->tableView->currentIndex().row());
            poruch_statusModel->submitAll();
            poruch_statusModel->select();
        }
        else
        {
            return;
        }
    }
}


bool Poruch_status::checkRelation(QString id_dolj)
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return NULL;
    }
    QSqlQuery query;
    query.prepare("select st_Poruch_status from is_sotrudniki where st_Poruch_status = :id_dolj");
    query.bindValue(":id_dolj", id_dolj);
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
