#include "stip_type.h"
#include "ui_stip_type.h"
#include "delegates/textnndelegate.h"
#include <QMessageBox>

bool Stip_type::isOpen = false;
Stip_type::Stip_type(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Stip_type)
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
    stip_typeModel = new QSqlTableModel(this);
    stip_typeModel->setTable("is_stip_type");
    stip_typeModel->setHeaderData(1,Qt::Horizontal,tr("Вид стипендии"));
    stip_typeModel->select();
    ui->tableView->setModel(stip_typeModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setItemDelegateForColumn(1, new textNnDelegate(ui->tableView));
}

Stip_type::~Stip_type()
{
    isOpen = false;
    delete ui;
}

void Stip_type::on_pushButton_save_clicked()
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
    stip_typeModel->submitAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}

void Stip_type::on_pushButton_cancel_clicked()
{
    stip_typeModel->revertAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные не сохранены"));
}

void Stip_type::on_pushButton_add_clicked()
{
    stip_typeModel->insertRow(stip_typeModel->rowCount());
    ui->tableView->edit(stip_typeModel->index(stip_typeModel->rowCount() - 1, 1));
    ui->tableView->setCurrentIndex(stip_typeModel->index(stip_typeModel->rowCount()-1,1));
}

void Stip_type::on_pushButton_edit_clicked()
{
    if (!ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
        return;
    }
    ui->tableView->edit(ui->tableView->currentIndex());
}

void Stip_type::on_pushButton_update_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    stip_typeModel->select();
}

void Stip_type::on_pushButton_del_clicked()
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
            stip_typeModel->removeRow(ui->tableView->currentIndex().row());
            stip_typeModel->submitAll();
            stip_typeModel->select();
        }
        else
        {
            return;
        }
    }
}


bool Stip_type::checkRelation(QString id_dolj)
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return NULL;
    }
    QSqlQuery query;
    query.prepare("select st_Stip_type from is_sotrudniki where st_Stip_type = :id_dolj");
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
