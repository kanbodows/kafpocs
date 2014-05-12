#include "specialnosti.h"
#include "ui_specialnosti.h"
#include "delegates/textnndelegate.h"
#include <QMessageBox>

bool Specialnosti::isOpen = false;
Specialnosti::Specialnosti(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Specialnosti)
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
    specialnostiiModel = new QSqlTableModel(this);
    specialnostiiModel->setTable("is_specialnosti");
    specialnostiiModel->setHeaderData(1,Qt::Horizontal,tr("Специальность"));
    specialnostiiModel->setHeaderData(2,Qt::Horizontal,tr("Полное название"));
    specialnostiiModel->select();
    ui->tableView_specialnosti->setModel(specialnostiiModel);
    ui->tableView_specialnosti->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_specialnosti->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_specialnosti->setColumnHidden(0,true);
    ui->tableView_specialnosti->setItemDelegateForColumn(1, new textNnDelegate(ui->tableView_specialnosti));
}

Specialnosti::~Specialnosti()
{
    isOpen = false;
    delete ui;
}

void Specialnosti::on_pushButton_save_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    for(int i=0;i<ui->tableView_specialnosti->model()->rowCount(); i++)
    {
        if(ui->tableView_specialnosti->model()->index(i,1).data().isNull())
        {
            QMessageBox::information(this, tr("Информация"),tr("Поле 'Специальность' должно быть заполнено"));
            return;
        }

    }
    specialnostiiModel->submitAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}

void Specialnosti::on_pushButton_cancel_clicked()
{
    specialnostiiModel->revertAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные не сохранены"));
}

void Specialnosti::on_pushButton_add_clicked()
{
    specialnostiiModel->insertRow(specialnostiiModel->rowCount());
    ui->tableView_specialnosti->edit(specialnostiiModel->index(specialnostiiModel->rowCount() - 1, 1));
    ui->tableView_specialnosti->setCurrentIndex(specialnostiiModel->index(specialnostiiModel->rowCount()-1,1));
}

void Specialnosti::on_pushButton_edit_clicked()
{
    if (!ui->tableView_specialnosti->model()->index(ui->tableView_specialnosti->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
        return;
    }
    ui->tableView_specialnosti->edit(ui->tableView_specialnosti->currentIndex());
}

void Specialnosti::on_pushButton_update_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    specialnostiiModel->select();
}

void Specialnosti::on_pushButton_del_clicked()
{
    if (!ui->tableView_specialnosti->model()->index(ui->tableView_specialnosti->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка удаления"), tr("Ни одной записи не выбрано"));
        return;
    }
    if (QMessageBox::warning(this, tr("Удаление записи"), tr("Вы уверены, что хотите удалить запись? \n Восстановить запись невозможно"),
                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        if (! dal_main->checkConnection())
        {
            QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
            return;
        }
        specialnostiiModel->removeRow(ui->tableView_specialnosti->currentIndex().row());
        specialnostiiModel->submitAll();
        specialnostiiModel->select();
    }
    else
    {
        return;
    }
}
