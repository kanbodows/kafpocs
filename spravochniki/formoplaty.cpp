#include "formoplaty.h"
#include "ui_formoplaty.h"
#include "delegates/textnndelegate.h"
#include <QMessageBox>

bool FormOplaty::isOpen = false;
FormOplaty::FormOplaty(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormOplaty)
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
    formaOplatiModel = new QSqlRelationalTableModel(this);
    formaOplatiModel->setTable("is_form_opl");
    formaOplatiModel->setHeaderData(1,Qt::Horizontal,tr("Форма оплаты"));
    formaOplatiModel->select();
    ui->tableView_formaOplati->setModel(formaOplatiModel);
    ui->tableView_formaOplati->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_formaOplati->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_formaOplati->setColumnHidden(0,true);
    ui->tableView_formaOplati->setItemDelegateForColumn(1, new textNnDelegate(ui->tableView_formaOplati));
}

FormOplaty::~FormOplaty()
{
    isOpen = false;
    delete ui;
}

void FormOplaty::on_pushButton_add_clicked()
{
    formaOplatiModel->insertRow(formaOplatiModel->rowCount());
    ui->tableView_formaOplati->edit(formaOplatiModel->index(formaOplatiModel->rowCount() - 1, 1));
    ui->tableView_formaOplati->setCurrentIndex(formaOplatiModel->index(formaOplatiModel->rowCount()-1,1));
}

void FormOplaty::on_pushButton_edit_clicked()
{
    if (!ui->tableView_formaOplati->model()->index(ui->tableView_formaOplati->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
        return;
    }
    ui->tableView_formaOplati->edit(ui->tableView_formaOplati->currentIndex());
}

void FormOplaty::on_pushButton_update_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    formaOplatiModel->select();
}

void FormOplaty::on_pushButton_del_clicked()
{

    if (!ui->tableView_formaOplati->model()->index(ui->tableView_formaOplati->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка удаления"), tr("Ни одной записи не выбрано"));
        return;
    }
    if (this->checkRelation(ui->tableView_formaOplati->model()->index(ui->tableView_formaOplati->currentIndex().row(), 0).data().toString()))
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
            formaOplatiModel->removeRow(ui->tableView_formaOplati->currentIndex().row());
            formaOplatiModel->submitAll();
            formaOplatiModel->select();
        }
        else
        {
            return;
        }
    }
}

bool FormOplaty::checkRelation(QString id_formOpl)
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return NULL;
    }
    QSqlQuery query;
    query.prepare("select s_form_oplat from is_student where s_form_oplat = :id_formOpl");
    query.bindValue(":id_formOpl", id_formOpl);
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

void FormOplaty::on_pushButton_cancel_clicked()
{
    formaOplatiModel->revertAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные не сохранены"));

}

void FormOplaty::on_pushButton_save_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    for(int i=0;i<ui->tableView_formaOplati->model()->rowCount(); i++)
    {
        if(ui->tableView_formaOplati->model()->index(i,1).data().isNull())
        {
            QMessageBox::information(this, tr("Информация"),tr("Поле 'Форма оплаты' должно быть заполнено"));
            return;
        }

    }
    formaOplatiModel->submitAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}
