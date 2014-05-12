#include "voenkomat.h"
#include "ui_voenkomat.h"
#include "delegates/textnndelegate.h"
#include <QMessageBox>
bool Voenkomat::isOpen = false;
Voenkomat::Voenkomat(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Voenkomat)
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
    voenkomatModel = new QSqlTableModel(this);
    voenkomatModel->setTable("is_voenkomat");
    voenkomatModel->setHeaderData(1,Qt::Horizontal,tr("Звание"));
    voenkomatModel->select();
    ui->tableView->setModel(voenkomatModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setItemDelegateForColumn(1, new textNnDelegate(ui->tableView));
}

Voenkomat::~Voenkomat()
{
    isOpen = false;
    delete ui;
}

void Voenkomat::on_pushButton_save_clicked()
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
    voenkomatModel->submitAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}

void Voenkomat::on_pushButton_cancel_clicked()
{
    voenkomatModel->revertAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные не сохранены"));
}

void Voenkomat::on_pushButton_add_clicked()
{
    voenkomatModel->insertRow(voenkomatModel->rowCount());
    ui->tableView->edit(voenkomatModel->index(voenkomatModel->rowCount() - 1, 1));
    ui->tableView->setCurrentIndex(voenkomatModel->index(voenkomatModel->rowCount()-1,1));
}

void Voenkomat::on_pushButton_edit_clicked()
{
    if (!ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
        return;
    }
    ui->tableView->edit(ui->tableView->currentIndex());
}

void Voenkomat::on_pushButton_update_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    voenkomatModel->select();
}

void Voenkomat::on_pushButton_del_clicked()
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
            voenkomatModel->removeRow(ui->tableView->currentIndex().row());
            voenkomatModel->submitAll();
            voenkomatModel->select();
        }
        else
        {
            return;
        }
    }
}


bool Voenkomat::checkRelation(QString id_dolj)
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return NULL;
    }
    QSqlQuery query;
    query.prepare("select st_Voenkomat from is_sotrudniki where st_Voenkomat = :id_dolj");
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
