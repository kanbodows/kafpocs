#include "proekty.h"
#include "ui_proekty.h"
#include <QMessageBox>
#include "delegates/textnndelegate.h"
#include "delegates/datedelegate.h"
bool Proekty::isOpen = false;
Proekty::Proekty(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Proekty)
{
    ui->setupUi(this);
    if (! this->isOpen)
    {
        this->isOpen = true;
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
    proektyModel = new QSqlRelationalTableModel(this);
    proektyModel->setTable("is_proekty");
    proektyModel->setHeaderData(1,Qt::Horizontal,tr("Преподаватель"));
    proektyModel->setHeaderData(2,Qt::Horizontal,tr("Дата"));
    proektyModel->setHeaderData(3,Qt::Horizontal,tr("Проект"));
    proektyModel->setHeaderData(4,Qt::Horizontal,tr("Описание проекта"));
    proektyModel->setRelation(1, QSqlRelation("sotr_kafedri", "id_sotr", "st_fio"));
    proektyModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    proektyModel->select();
    sotrudnikiModel = new QSqlTableModel(this);
    sotrudnikiModel->setTable("sotr_kafedri");
    ui->tableView->setModel(proektyModel);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setItemDelegate(new QSqlRelationalDelegate(ui->tableView));
    ui->tableView->setItemDelegateForColumn(2, new dateDelegate(ui->tableView));
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ui->lineEdit->setVisible(false);
}

Proekty::~Proekty()
{
    this->isOpen=false;
    delete ui;
}

void Proekty::on_pushButton_add_clicked()
{
    proektyModel->insertRow(proektyModel->rowCount());
    ui->tableView->edit(proektyModel->index(proektyModel->rowCount() - 1, 1));
    ui->tableView->setCurrentIndex(proektyModel->index(proektyModel->rowCount()-1,1));
}

void Proekty::on_pushButton_del_clicked()
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select * from is_proekty where id_proekta = " + QString::number(ui->tableView->model()->index(ui->tableView->currentIndex().row(),0).data().toInt()));
    query->exec();

    query->first();

    ui->lineEdit->setText(query->value(1).toString());
    if(ui->lineEdit->text().toInt()==dal_main->getCurrentEmployee())
    {
        ui->pushButton_del->setEnabled(true);
        ui->pushButton_edit->setEnabled(true);
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    if (!ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().isValid())
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
        proektyModel->removeRow(ui->tableView->currentIndex().row());
        proektyModel->submitAll();
        proektyModel->select();
    }
    else
    {
        return;
    }
    }
    else
    {
        ui->pushButton_del->setEnabled(false);
        ui->pushButton_edit->setEnabled(false);
        QMessageBox::information(this,tr("Внимание"),tr("Вы не можете удалить не свою запись"));
    }
}

void Proekty::on_pushButton_update_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    proektyModel->select();
    sotrudnikiModel->select();
    if (this->sotrudnik_id != 0)
    {
        for (int i = 0; i < ui->comboBox_sotr->count(); i++)
        {
            if (sotrudnikiModel->index(i, 0).data().toInt() == this->sotrudnik_id)
            {
                ui->comboBox_sotr->setCurrentIndex(i);
                ui->comboBox_sotr->view()->setCurrentIndex(sotrudnikiModel->index(i, 0));
                break;
            }
        }
    }
}

void Proekty::on_pushButton_find_clicked()
{
    ui->label_naideno->setVisible(true);
    this->formingSearch();
}

void Proekty::on_pushButton_clear_clicked()
{
    this->sotrudnik_id = 0;
    ui->label_naideno->clear();
    ui->lineEdit_nazvanie->clear();
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    proektyModel->setFilter("");
    proektyModel->select();
    ui->comboBox_sotr->setModel(sotrudnikiModel);
    ui->comboBox_sotr->setModelColumn(1);
    ui->comboBox_sotr->setCurrentIndex(-1);
}

void Proekty::on_pushButton_save_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    for(int i=0;i<ui->tableView->model()->rowCount(); i++)
    {
        if(ui->tableView->model()->index(i,1).data().isNull()||ui->tableView->model()->index(i,2).data().isNull()||ui->tableView->model()->index(i,3).data().isNull())
        {
            QMessageBox::information(this, tr("Информация"),tr("Поля 'Преподаватель', 'Дата' и 'Проект' должно быть заполнено"));
            return;
        }
    }
    proektyModel->submitAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}

void Proekty::on_pushButton_cancel_clicked()
{
    proektyModel->revertAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные не сохранены"));
}

void Proekty::on_comboBox_sotr_activated(int index)
{
    this->sotrudnik_id = ui->comboBox_sotr->model()->index(index,0).data().toInt();
}


void Proekty::formingSearch()
{
    QString condition = "";

    if (! ui->lineEdit_nazvanie->text().isEmpty())
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " proekt_name like '%" + ui->lineEdit_nazvanie->text() + "%'";
    }

    if (this->sotrudnik_id != 0)
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " sotrudnik_id = " + QString::number(this->sotrudnik_id);
    }

    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    proektyModel->setFilter(condition);
    if(ui->tableView->model()->rowCount()>0)
    {
        ui->label_result->setVisible(false);
        ui->label_naideno->setText("    Найдено: " + QString::number(ui->tableView->model()->rowCount()));
    }
    else
    {
        ui->label_result->setVisible(true);
        ui->label_naideno->setText("    Найдено: 0");
    }
}


void Proekty::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        this->sotrudnik_id = 0;
        ui->lineEdit_nazvanie->clear();
        proektyModel->setFilter("");
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
        sotrudnikiModel->select();
        ui->comboBox_sotr->setModel(sotrudnikiModel);
        ui->comboBox_sotr->setModelColumn(1);
        ui->comboBox_sotr->setCurrentIndex(-1);
        this->sotrudnik_id = 0;
        connect(ui->lineEdit_nazvanie,SIGNAL(editingFinished()),this,SLOT(formingSearch()));
    }
}

void Proekty::on_pushButton_edit_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select * from is_proekty where id_proekta = " + QString::number(ui->tableView->model()->index(ui->tableView->currentIndex().row(),0).data().toInt()));
    query->exec();

    query->first();

    ui->lineEdit->setText(query->value(1).toString());
    if(ui->lineEdit->text().toInt()==dal_main->getCurrentEmployee())
    {
        ui->pushButton_del->setEnabled(true);
        ui->pushButton_edit->setEnabled(true);
    if (!ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
        return;
    }
    ui->tableView->edit(ui->tableView->currentIndex());
    }
    else
    {
        ui->pushButton_del->setEnabled(false);
        ui->pushButton_edit->setEnabled(false);
        QMessageBox::information(this,tr("Внимание"),tr("Вы не можете редактировать не свою запись"));
    }
}

void Proekty::on_tableView_clicked(const QModelIndex &index)
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select * from is_proekty where id_proekta = " + QString::number(ui->tableView->model()->index(ui->tableView->currentIndex().row(),0).data().toInt()));
    query->exec();

    query->first();

    ui->lineEdit->setText(query->value(1).toString());
    if(ui->lineEdit->text().toInt()==dal_main->getCurrentEmployee())
    {
        ui->pushButton_del->setEnabled(true);
        ui->pushButton_edit->setEnabled(true);
    }
    else
    {
        ui->pushButton_del->setEnabled(false);
        ui->pushButton_edit->setEnabled(false);
    }
}
