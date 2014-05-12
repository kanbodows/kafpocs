#include "prize_types.h"
#include "ui_prize_types.h"
#include "delegates/textnndelegate.h"
#include <QMessageBox>

bool Prize_types::isOpen = false;
Prize_types::Prize_types(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Prize_types)
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
    prize_typesModel = new QSqlTableModel(this);
    prize_typesModel->setTable("is_prize_types");
    prize_typesModel->setHeaderData(1,Qt::Horizontal,tr("Вид премии"));
    prize_typesModel->select();
    ui->tableView->setModel(prize_typesModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setItemDelegateForColumn(1, new textNnDelegate(ui->tableView));
}

Prize_types::~Prize_types()
{
    isOpen = false;
    delete ui;
}

void Prize_types::on_pushButton_save_clicked()
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
    prize_typesModel->submitAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}

void Prize_types::on_pushButton_cancel_clicked()
{
    prize_typesModel->revertAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные не сохранены"));
}

void Prize_types::on_pushButton_add_clicked()
{
    prize_typesModel->insertRow(prize_typesModel->rowCount());
    ui->tableView->edit(prize_typesModel->index(prize_typesModel->rowCount() - 1, 1));
    ui->tableView->setCurrentIndex(prize_typesModel->index(prize_typesModel->rowCount()-1,1));
}

void Prize_types::on_pushButton_edit_clicked()
{
    if (!ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
        return;
    }
    ui->tableView->edit(ui->tableView->currentIndex());
}

void Prize_types::on_pushButton_update_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    prize_typesModel->select();
}

void Prize_types::on_pushButton_del_clicked()
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
            prize_typesModel->removeRow(ui->tableView->currentIndex().row());
            prize_typesModel->submitAll();
            prize_typesModel->select();
        }
        else
        {
            return;
        }
    }
}


bool Prize_types::checkRelation(QString id_dolj)
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return NULL;
    }
    QSqlQuery query;
    query.prepare("select st_Prize_types from is_sotrudniki where st_Prize_types = :id_dolj");
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
