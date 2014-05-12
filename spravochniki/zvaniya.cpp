#include "zvaniya.h"
#include "ui_zvaniya.h"
#include "delegates/textnndelegate.h"
#include <QMessageBox>
bool Zvaniya::isOpen = false;
Zvaniya::Zvaniya(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Zvaniya)
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
    zvaniyaModel = new QSqlTableModel(this);
    zvaniyaModel->setTable("is_zvaniya");
    zvaniyaModel->setHeaderData(1,Qt::Horizontal,tr("Звание"));
    zvaniyaModel->select();
    ui->tableView->setModel(zvaniyaModel);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setItemDelegateForColumn(1, new textNnDelegate(ui->tableView));
}

Zvaniya::~Zvaniya()
{
    isOpen = false;
    delete ui;
}

void Zvaniya::on_pushButton_save_clicked()
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
    zvaniyaModel->submitAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}

void Zvaniya::on_pushButton_cancel_clicked()
{
    zvaniyaModel->revertAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные не сохранены"));
}

void Zvaniya::on_pushButton_add_clicked()
{
    zvaniyaModel->insertRow(zvaniyaModel->rowCount());
    ui->tableView->edit(zvaniyaModel->index(zvaniyaModel->rowCount() - 1, 1));
    ui->tableView->setCurrentIndex(zvaniyaModel->index(zvaniyaModel->rowCount()-1,1));
}

void Zvaniya::on_pushButton_edit_clicked()
{
    if (!ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
        return;
    }
    ui->tableView->edit(ui->tableView->currentIndex());
}

void Zvaniya::on_pushButton_update_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    zvaniyaModel->select();
}

void Zvaniya::on_pushButton_del_clicked()
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
            zvaniyaModel->removeRow(ui->tableView->currentIndex().row());
            zvaniyaModel->submitAll();
            zvaniyaModel->select();
        }
        else
        {
            return;
        }
    }
}


bool Zvaniya::checkRelation(QString id_zvan)
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return NULL;
    }
    QSqlQuery query;
    query.prepare("(select zvanie from is_stud_voen where zvanie = :id_zvan) union (select zvanie from is_sotr_voen where zvanie = :id_zvan)");
    query.bindValue(":id_zvan", id_zvan);
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
