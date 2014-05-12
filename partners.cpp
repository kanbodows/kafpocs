#include "partners.h"
#include "ui_partners.h"
#include <QMessageBox>
bool Partners::isOpen = false;
Partners::Partners(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Partners)
{
    ui->setupUi(this);
    this->count=0;
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
    PartnerModel = new QSqlRelationalTableModel(this);
    PartnerModel->setTable("is_partners");
    PartnerModel->setHeaderData(1,Qt::Horizontal,tr("ФИО партнера"));
    PartnerModel->setHeaderData(2,Qt::Horizontal,tr("Телефон"));
    PartnerModel->setHeaderData(3,Qt::Horizontal,tr("E-mail"));
    PartnerModel->setHeaderData(4, Qt::Horizontal,tr("Комментарии"));
    PartnerModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    PartnerModel->select();

    ui->tableView_partner->setModel(PartnerModel);
    ui->tableView_partner->setColumnHidden(0,true);
    ui->tableView_partner->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_partner->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_partner->setItemDelegate(new QSqlRelationalDelegate(ui->tableView_partner));
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
}

Partners::~Partners()
{
    isOpen = false;
    delete ui;
}

void Partners::formingSearch()
{
    QString condition = "";

    if (! ui->lineEdit_fio->text().isEmpty())
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " pt_name like '%" + ui->lineEdit_fio->text() + "%'";
    }

    if (! ui->lineEdit_telefon->text().isEmpty())
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " pt_phone like '%" + ui->lineEdit_telefon->text() + "%'";
    }

    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    PartnerModel->setFilter(condition);
    if(ui->tableView_partner->model()->rowCount()>0)
    {
    this->naideno.append("    Найдено: " + QString::number(ui->tableView_partner->model()->rowCount()));
    ui->label_result->setVisible(false);
    ui->label_naideno->setText(this->naideno);
    this->naideno.clear();
    }
    else
    {
        this->naideno.append("    Найдено: 0");
        ui->label_result->setVisible(true);
        ui->label_naideno->setText(this->naideno);
        this->naideno.clear();
    }
}

void Partners::on_pushButton_add_clicked()
{
    PartnerModel->insertRow(PartnerModel->rowCount());
    ui->tableView_partner->edit(PartnerModel->index(PartnerModel->rowCount() - 1, 1));
    ui->tableView_partner->setCurrentIndex(PartnerModel->index(PartnerModel->rowCount()-1,1));
}

void Partners::on_pushButton_update_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }

    PartnerModel->select();
}

void Partners::on_pushButton_del_clicked()
{
    if (!ui->tableView_partner->model()->index(ui->tableView_partner->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка удаления"), tr("Ни одной записи не выбрано"));
        return;
    }
    if (this->checkRelation(ui->tableView_partner->model()->index(ui->tableView_partner->currentIndex().row(), 0).data().toString()))
    {
        QMessageBox::information(this, tr("Информация"), tr("Невозможно удалить запись, \n так как она используется в других таблицах"));
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
            PartnerModel->removeRow(ui->tableView_partner->currentIndex().row());
            PartnerModel->submitAll();
            PartnerModel->select();
        }
        else
        {
            return;
        }

}

void Partners::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);

        ui->lineEdit_fio->clear();
        ui->lineEdit_telefon->clear();
        PartnerModel->setFilter("");
        ui->label_naideno->setVisible(false);
        on_pushButton_update_clicked();
    }
    else
    {
        if (! dal_main->checkConnection())
        {
            QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
            return;
        }
        ui->groupBox_search->setVisible(true);
    }
}

void Partners::on_pushButton_find_clicked()
{

    this->naideno.clear();
    ui->label_naideno->setVisible(true);
    this->formingSearch();
}

void Partners::on_pushButton_clear_clicked()
{

    this->naideno.clear();
    ui->label_naideno->clear();
    ui->lineEdit_fio->clear();
    ui->lineEdit_telefon->clear();
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    PartnerModel->setFilter("");
    PartnerModel->select();
}

void Partners::on_pushButton_save_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    for(int i=0;i<ui->tableView_partner->model()->rowCount(); i++)
    {
        if(ui->tableView_partner->model()->index(i,1).data().isNull())
        {
            QMessageBox::information(this, tr("Информация"),tr("Поле 'ФИО партнера' должно быть заполнено"));
            return;
        }
    }
    PartnerModel->submitAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}

void Partners::on_pushButton_cancel_clicked()
{
    PartnerModel->revertAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные не сохранены"));
}

bool Partners::checkRelation(QString partner_id)
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return NULL;
    }
    QSqlQuery query;
    query.prepare("select id_partners  from is_partners where id_partners = :partner_id");
    query.bindValue(":partner_id", partner_id);
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

void Partners::on_pushButton_edit_clicked()
{
    if (!ui->tableView_partner->model()->index(ui->tableView_partner->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
        return;
    }
    ui->tableView_partner->edit(ui->tableView_partner->currentIndex());
}
