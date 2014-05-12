#include "tip_voprosa.h"
#include "ui_tip_voprosa.h"
#include "delegates/textnndelegate.h"
#include <QMessageBox>

bool Tip_voprosa::isOpen = false;
Tip_voprosa::Tip_voprosa(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Tip_voprosa)
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
    tip_voprosaModel = new QSqlTableModel(this);
    tip_voprosaModel->setTable("is_tip_voprosa");
    tip_voprosaModel->setHeaderData(1,Qt::Horizontal,tr("Тип вопроса"));
    tip_voprosaModel->select();
    ui->tableView->setModel(tip_voprosaModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setItemDelegateForColumn(1, new textNnDelegate(ui->tableView));
}

Tip_voprosa::~Tip_voprosa()
{
    isOpen = false;
    delete ui;
}

void Tip_voprosa::on_pushButton_save_clicked()
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
    tip_voprosaModel->submitAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}

void Tip_voprosa::on_pushButton_cancel_clicked()
{
    tip_voprosaModel->revertAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные не сохранены"));
}

void Tip_voprosa::on_pushButton_add_clicked()
{
    tip_voprosaModel->insertRow(tip_voprosaModel->rowCount());
    ui->tableView->edit(tip_voprosaModel->index(tip_voprosaModel->rowCount() - 1, 1));
    ui->tableView->setCurrentIndex(tip_voprosaModel->index(tip_voprosaModel->rowCount()-1,1));
}

void Tip_voprosa::on_pushButton_edit_clicked()
{
    if (!ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
        return;
    }
    ui->tableView->edit(ui->tableView->currentIndex());
}

void Tip_voprosa::on_pushButton_update_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    tip_voprosaModel->select();
}

void Tip_voprosa::on_pushButton_del_clicked()
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
            tip_voprosaModel->removeRow(ui->tableView->currentIndex().row());
            tip_voprosaModel->submitAll();
            tip_voprosaModel->select();
        }
        else
        {
            return;
        }
    }
}


bool Tip_voprosa::checkRelation(QString id_dolj)
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return NULL;
    }
    QSqlQuery query;
    query.prepare("select st_Tip_voprosa from is_sotrudniki where st_Tip_voprosa = :id_dolj");
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
