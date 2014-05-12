#include "formaobucheniya.h"
#include "ui_formaobucheniya.h"
#include <QMessageBox>
#include "delegates/textnndelegate.h"
bool FormaObucheniya::isOpen = false;
FormaObucheniya::FormaObucheniya(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FormaObucheniya)
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
    formaObucheniyaModel = new QSqlTableModel(this);
    formaObucheniyaModel->setTable("is_form_obuch");
    formaObucheniyaModel->setHeaderData(1,Qt::Horizontal,tr("Форма обучения"));
    formaObucheniyaModel->setHeaderData(2,Qt::Horizontal,tr("Обучение"));
    formaObucheniyaModel->select();
    ui->tableView_formaObuch->setModel(formaObucheniyaModel);
    ui->tableView_formaObuch->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_formaObuch->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_formaObuch->setColumnHidden(0,true);
    ui->tableView_formaObuch->setItemDelegateForColumn(1, new textNnDelegate(ui->tableView_formaObuch));

}

FormaObucheniya::~FormaObucheniya()
{
    isOpen=false;
    delete ui;
}

void FormaObucheniya::on_pushButton_add_clicked()
{
    formaObucheniyaModel->insertRow(formaObucheniyaModel->rowCount());
    ui->tableView_formaObuch->edit(formaObucheniyaModel->index(formaObucheniyaModel->rowCount() - 1, 1));
    ui->tableView_formaObuch->setCurrentIndex(formaObucheniyaModel->index(formaObucheniyaModel->rowCount()-1,1));
}

void FormaObucheniya::on_pushButton_edit_clicked()
{
    if (!ui->tableView_formaObuch->model()->index(ui->tableView_formaObuch->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
        return;
    }
    ui->tableView_formaObuch->edit(ui->tableView_formaObuch->currentIndex());
}

void FormaObucheniya::on_pushButton_save_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    for(int i=0;i<ui->tableView_formaObuch->model()->rowCount(); i++)
    {
        if(ui->tableView_formaObuch->model()->index(i,1).data().isNull() || ui->tableView_formaObuch->model()->index(i,2).data().isNull())
        {
            QMessageBox::information(this, tr("Информация"),tr("Поля 'Форма обучения' и 'Обчение' должны быть заполнены"));
            return;
        }

    }
    formaObucheniyaModel->submitAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}

void FormaObucheniya::on_pushButton_cancel_clicked()
{
    formaObucheniyaModel->revertAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные не сохранены"));
}

void FormaObucheniya::on_pushButton_update_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    formaObucheniyaModel->select();
}

void FormaObucheniya::on_pushButton_dell_clicked()
{
    if (!ui->tableView_formaObuch->model()->index(ui->tableView_formaObuch->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка удаления"), tr("Ни одной записи не выбрано"));
        return;
    }
    if (this->checkRelation(ui->tableView_formaObuch->model()->index(ui->tableView_formaObuch->currentIndex().row(), 0).data().toString()))
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
            formaObucheniyaModel->removeRow(ui->tableView_formaObuch->currentIndex().row());
            formaObucheniyaModel->submitAll();
            formaObucheniyaModel->select();
        }
        else
        {
            return;
        }
    }
}

bool FormaObucheniya::checkRelation(QString id_formObuch)
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return NULL;
    }
    QSqlQuery query;
    query.prepare("select s_form_obuch from is_student where s_form_obuch = :id_formObuch");
    query.bindValue(":id_formObuch", id_formObuch);
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
