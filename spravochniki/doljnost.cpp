#include "doljnost.h"
#include "ui_doljnost.h"
#include "delegates/textnndelegate.h"
#include <QMessageBox>

bool Doljnost::isOpen = false;
Doljnost::Doljnost(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Doljnost)
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
    doljnostiModel = new QSqlTableModel(this);
    doljnostiModel->setTable("is_doljnost");
    doljnostiModel->setHeaderData(1,Qt::Horizontal,tr("Должности"));
    doljnostiModel->select();
    ui->tableView_doljnost->setModel(doljnostiModel);
    ui->tableView_doljnost->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_doljnost->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_doljnost->setColumnHidden(0,true);
    ui->tableView_doljnost->setItemDelegateForColumn(1, new textNnDelegate(ui->tableView_doljnost));
}

Doljnost::~Doljnost()
{
    isOpen = false;
    delete ui;
}

void Doljnost::on_pushButton_save_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    for(int i=0;i<ui->tableView_doljnost->model()->rowCount(); i++)
    {
        if(ui->tableView_doljnost->model()->index(i,1).data().isNull())
        {
            QMessageBox::information(this, tr("Информация"),tr("Поле 'Должности' должно быть заполнено"));
            return;
        }

    }
    doljnostiModel->submitAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}

void Doljnost::on_pushButton_cancel_clicked()
{
    doljnostiModel->revertAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные не сохранены"));
}

void Doljnost::on_pushButton_add_clicked()
{
    doljnostiModel->insertRow(doljnostiModel->rowCount());
    ui->tableView_doljnost->edit(doljnostiModel->index(doljnostiModel->rowCount() - 1, 1));
    ui->tableView_doljnost->setCurrentIndex(doljnostiModel->index(doljnostiModel->rowCount()-1,1));
}

void Doljnost::on_pushButton_edit_clicked()
{
    if (!ui->tableView_doljnost->model()->index(ui->tableView_doljnost->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
        return;
    }
    ui->tableView_doljnost->edit(ui->tableView_doljnost->currentIndex());
}

void Doljnost::on_pushButton_update_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    doljnostiModel->select();
}

void Doljnost::on_pushButton_del_clicked()
{
    if (!ui->tableView_doljnost->model()->index(ui->tableView_doljnost->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка удаления"), tr("Ни одной записи не выбрано"));
        return;
    }
    if (this->checkRelation(ui->tableView_doljnost->model()->index(ui->tableView_doljnost->currentIndex().row(), 0).data().toString()))
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
            doljnostiModel->removeRow(ui->tableView_doljnost->currentIndex().row());
            doljnostiModel->submitAll();
            doljnostiModel->select();
        }
        else
        {
            return;
        }
    }
}


bool Doljnost::checkRelation(QString id_dolj)
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return NULL;
    }
    QSqlQuery query;
    query.prepare("select st_doljnost from is_sotrudniki where st_doljnost = :id_dolj");
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
