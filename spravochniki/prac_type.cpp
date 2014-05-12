#include "prac_type.h"
#include "ui_prac_type.h"
#include "delegates/textnndelegate.h"
#include <QMessageBox>
bool Prac_type::isOpen = false;
Prac_type::Prac_type(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Prac_type)
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
    prac_typeModel = new QSqlTableModel(this);
    prac_typeModel->setTable("is_prac_type");
    prac_typeModel->setHeaderData(1,Qt::Horizontal,tr("Виды практик"));
    prac_typeModel->select();
    ui->tableView->setModel(prac_typeModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setItemDelegateForColumn(1, new textNnDelegate(ui->tableView));
}

Prac_type::~Prac_type()
{
    isOpen = false;
    delete ui;
}

void Prac_type::on_pushButton_save_clicked()
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
    prac_typeModel->submitAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}

void Prac_type::on_pushButton_cancel_clicked()
{
    prac_typeModel->revertAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные не сохранены"));
}

void Prac_type::on_pushButton_add_clicked()
{
    prac_typeModel->insertRow(prac_typeModel->rowCount());
    ui->tableView->edit(prac_typeModel->index(prac_typeModel->rowCount() - 1, 1));
    ui->tableView->setCurrentIndex(prac_typeModel->index(prac_typeModel->rowCount()-1,1));
}

void Prac_type::on_pushButton_edit_clicked()
{
    if (!ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
        return;
    }
    ui->tableView->edit(ui->tableView->currentIndex());
}

void Prac_type::on_pushButton_update_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    prac_typeModel->select();
}

void Prac_type::on_pushButton_del_clicked()
{
    if (!ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка удаления"), tr("Ни одной записи не выбрано"));
        return;
    }
    /*if (this->checkRelation(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().toString()))
    {
        QMessageBox::critical(this, tr("Ошибка удаления"), tr("Невозможно удалить запись, \n так как она используется в других таблицах"));
        return;
    }
    else*/
    {
        if (QMessageBox::warning(this, tr("Удаление записи"), tr("Вы уверены, что хотите удалить запись? \n Восстановить запись невозможно"),
                                           QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            if (! dal_main->checkConnection())
            {
                QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
                return;
            }
            prac_typeModel->removeRow(ui->tableView->currentIndex().row());
            prac_typeModel->submitAll();
            prac_typeModel->select();
        }
        else
        {
            return;
        }
    }
}


/*bool Prac_type::checkRelation(QString id_dolj)
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return NULL;
    }
    QSqlQuery query;
    query.prepare("select st_Prac_type from is_sotrudniki where st_Prac_type = :id_dolj");
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
}*/
