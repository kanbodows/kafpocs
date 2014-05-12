#include "kafedry.h"
#include "ui_kafedry.h"
#include "delegates/textnndelegate.h"
#include <QMessageBox>

bool Kafedry::isOpen = false;
Kafedry::Kafedry(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Kafedry)
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
    kafedryiModel = new QSqlTableModel(this);
    kafedryiModel->setTable("is_kafedry");
    kafedryiModel->setHeaderData(1,Qt::Horizontal,tr("Кафедра"));
    kafedryiModel->setHeaderData(2,Qt::Horizontal,tr("Полное название"));
    kafedryiModel->select();
    ui->tableView_kafedry->setModel(kafedryiModel);
    ui->tableView_kafedry->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_kafedry->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_kafedry->setColumnHidden(0,true);
    ui->tableView_kafedry->setItemDelegateForColumn(1, new textNnDelegate(ui->tableView_kafedry));
}

Kafedry::~Kafedry()
{
    isOpen = false;
    delete ui;
}

void Kafedry::on_pushButton_save_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    for(int i=0;i<ui->tableView_kafedry->model()->rowCount(); i++)
    {
        if(ui->tableView_kafedry->model()->index(i,1).data().isNull())
        {
            QMessageBox::information(this, tr("Информация"),tr("Поле 'Кафедра' должно быть заполнено"));
            return;
        }

    }
    kafedryiModel->submitAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}

void Kafedry::on_pushButton_cancel_clicked()
{
    kafedryiModel->revertAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные не сохранены"));
}

void Kafedry::on_pushButton_add_clicked()
{
    kafedryiModel->insertRow(kafedryiModel->rowCount());
    ui->tableView_kafedry->edit(kafedryiModel->index(kafedryiModel->rowCount() - 1, 1));
    ui->tableView_kafedry->setCurrentIndex(kafedryiModel->index(kafedryiModel->rowCount()-1,1));
}

void Kafedry::on_pushButton_edit_clicked()
{
    if (!ui->tableView_kafedry->model()->index(ui->tableView_kafedry->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
        return;
    }
    ui->tableView_kafedry->edit(ui->tableView_kafedry->currentIndex());
}

void Kafedry::on_pushButton_update_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    kafedryiModel->select();
}

void Kafedry::on_pushButton_del_clicked()
{
    if (!ui->tableView_kafedry->model()->index(ui->tableView_kafedry->currentIndex().row(), 0).data().isValid())
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
        kafedryiModel->removeRow(ui->tableView_kafedry->currentIndex().row());
        kafedryiModel->submitAll();
        kafedryiModel->select();
    }
    else
    {
        return;
    }
}
