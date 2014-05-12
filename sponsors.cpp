#include "sponsors.h"
#include "ui_sponsors.h"
#include "delegates/datedelegate.h"
#include <QMessageBox>
bool Sponsors::isOpen = false;
Sponsors::Sponsors(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Sponsors)
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
    SponsorModel = new QSqlRelationalTableModel(this);
    SponsorModel->setTable("is_sponsors");
    SponsorModel->setHeaderData(1,Qt::Horizontal,tr("ФИО"));
    SponsorModel->setHeaderData(2,Qt::Horizontal,tr("Дата спонсирования"));
    SponsorModel->setHeaderData(3,Qt::Horizontal,tr("Цель спонсирования"));
    SponsorModel->setHeaderData(4,Qt::Horizontal,tr("Сумма(сом)"));
    SponsorModel->setHeaderData(5,Qt::Horizontal,tr("Мат. помощь"));
    SponsorModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    SponsorModel->select();
    ui->tableView_sponsor->setModel(SponsorModel);
    ui->tableView_sponsor->setColumnHidden(0,true);
    ui->tableView_sponsor->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_sponsor->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_sponsor->setItemDelegate(new QSqlRelationalDelegate(ui->tableView_sponsor));
    ui->tableView_sponsor->setItemDelegateForColumn(2, new dateDelegate(ui->tableView_sponsor));

    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
}

Sponsors::~Sponsors()
{
    isOpen = false;
    delete ui;
}

void Sponsors::formingSearch()
{
    QString condition = "";

    if (! ui->lineEdit_FIO->text().isEmpty())
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " name like '%" + ui->lineEdit_FIO->text() + "%'  ";
    }

    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    SponsorModel->setFilter(condition);
    if(ui->tableView_sponsor->model()->rowCount()>0)
    {
    this->naideno.append("    Найдено: " + QString::number(ui->tableView_sponsor->model()->rowCount()));
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

void Sponsors::on_pushButton_add_clicked()
{
    SponsorModel->insertRow(SponsorModel->rowCount());
    ui->tableView_sponsor->edit(SponsorModel->index(SponsorModel->rowCount() - 1, 1));
    ui->tableView_sponsor->setCurrentIndex(SponsorModel->index(SponsorModel->rowCount()-1,1));
}

void Sponsors::on_pushButton_update_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    SponsorModel->setTable("is_sponsors");
    SponsorModel->setHeaderData(1,Qt::Horizontal,tr("ФИО"));
    SponsorModel->setHeaderData(2,Qt::Horizontal,tr("Дата спонсирования"));
    SponsorModel->setHeaderData(3,Qt::Horizontal,tr("Цель спонсирования"));
    SponsorModel->setHeaderData(4,Qt::Horizontal,tr("Сумма(сом)"));
    SponsorModel->setHeaderData(5,Qt::Horizontal,tr("Мат. помощь"));
    SponsorModel->select();


    ui->tableView_sponsor->setModel(SponsorModel);
    ui->tableView_sponsor->setColumnHidden(0,true);
    ui->tableView_sponsor->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_sponsor->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_sponsor->setItemDelegate(new QSqlRelationalDelegate(ui->tableView_sponsor));
    ui->tableView_sponsor->setItemDelegateForColumn(2, new dateDelegate(ui->tableView_sponsor));

}

void Sponsors::on_pushButton_del_clicked()
{
    if (!ui->tableView_sponsor->model()->index(ui->tableView_sponsor->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка удаления"), tr("Ни одной записи не выбрано"));
        return;
    }

    if (QMessageBox::information(this, tr("Информация"), tr("        Вы уверены, что хотите удалить запись? \n Все данные связанные с записью также будут удалены"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            if (! dal_main->checkConnection())
            {
                QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
                return;
            }
            SponsorModel->removeRow(ui->tableView_sponsor->currentIndex().row());
            SponsorModel->submitAll();
            SponsorModel->select();
        }
        else
        {
            return;
        }

}

void Sponsors::on_pushButton_search_clicked()
{

    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        ui->lineEdit_FIO->clear();
        SponsorModel->setFilter("");
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
        SponsorModel->select();
        connect(ui->lineEdit_FIO,SIGNAL(editingFinished()),this,SLOT(formingSearch()));
    }
}

void Sponsors::on_pushButton_find_clicked()
{
    this->naideno.clear();
    ui->label_naideno->setVisible(true);
    this->formingSearch();
}

void Sponsors::on_pushButton_clear_clicked()
{

    this->naideno.clear();
    ui->label_naideno->clear();
    ui->lineEdit_FIO->clear();
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    SponsorModel->setFilter("");
    SponsorModel->select();
}

void Sponsors::on_pushButton_save_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    for(int i=0;i<ui->tableView_sponsor->model()->rowCount(); i++)
    {
        if(ui->tableView_sponsor->model()->index(i,1).data().isNull()||ui->tableView_sponsor->model()->index(i,2).data().isNull()||ui->tableView_sponsor->model()->index(i,3).data().isNull())
        {
            QMessageBox::information(this, tr("Информация"),tr("Поля 'ФИО', 'Дата спонсирования' и 'Цель спонсирования'                                                                     должно быть заполнены"));
            return;
        }

        if(ui->tableView_sponsor->model()->index(i,4).data().isNull() && ui->tableView_sponsor->model()->index(i,5).data().isNull())
        {
            QMessageBox::information(this, tr("Информация"),tr("Заполните поле 'Сумма(сом)' или поле 'Мат. помощь'"));
            return;
        }
    }
    SponsorModel->submitAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}

void Sponsors::on_pushButton_cancel_clicked()
{
    SponsorModel->revertAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные не сохранены"));
}

void Sponsors::on_pushButton_edit_clicked()
{
    if (!ui->tableView_sponsor->model()->index(ui->tableView_sponsor->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
        return;
    }
    ui->tableView_sponsor->edit(ui->tableView_sponsor->currentIndex());
}
