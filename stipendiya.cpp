#include "stipendiya.h"
#include "ui_stipendiya.h"
#include<QMessageBox>
bool Stipendiya::isOpen = false;
Stipendiya::Stipendiya(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Stipendiya)
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

    IsStipendiaModel = new QSqlRelationalTableModel(this);
    IsStipendiaModel->setTable("is_stipendii");
    IsStipendiaModel->setRelation(1, QSqlRelation("is_student", "id_student", "s_fio"));
    IsStipendiaModel->setRelation(2, QSqlRelation("is_stip_type", "id_stp_type", "stp_type"));
    IsStipendiaModel->setHeaderData(1,Qt::Horizontal,tr("Студент"));
    IsStipendiaModel->setHeaderData(2,Qt::Horizontal,tr("Тип стипендии"));
    IsStipendiaModel->setHeaderData(3,Qt::Horizontal,tr("Размер стипендии"));
    IsStipendiaModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    IsStipendiaModel->select();
    IsTypeStipendia = new QSqlTableModel(this);
    IsTypeStipendia->setTable("is_stip_type");
    ui->tableView_stipendia->setModel(IsStipendiaModel);
    ui->tableView_stipendia->setColumnHidden(0,true);
    ui->tableView_stipendia->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_stipendia->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_stipendia->setItemDelegate(new QSqlRelationalDelegate(ui->tableView_stipendia));
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    this->typeSTIP_id=0;
}

Stipendiya::~Stipendiya()
{
    delete ui;
}

void Stipendiya::on_pushButton_add_clicked()
{
    IsStipendiaModel->insertRow(IsStipendiaModel->rowCount());
    ui->tableView_stipendia->edit(IsStipendiaModel->index(IsStipendiaModel->rowCount() - 1, 1));
    ui->tableView_stipendia->setCurrentIndex(IsStipendiaModel->index(IsStipendiaModel->rowCount()-1,1));
}

void Stipendiya::on_pushButton_update_clicked()
{

    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }

    IsStipendiaModel->select();
    IsTypeStipendia->select();

    if (this->typeSTIP_id != 0)
    {
        for (int i = 0; i < ui->comboBox_type_stipen->count(); i++)
        {
            if (IsTypeStipendia->index(i, 0).data().toInt() == this->typeSTIP_id)
            {
                ui->comboBox_type_stipen->setCurrentIndex(i);
                ui->comboBox_type_stipen->view()->setCurrentIndex(IsTypeStipendia->index(i, 0));
                break;
            }
        }
    }
}

void Stipendiya::on_pushButton_del_clicked()
{
    if (!ui->tableView_stipendia->model()->index(ui->tableView_stipendia->currentIndex().row(), 0).data().isValid())
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
        IsStipendiaModel->removeRow(ui->tableView_stipendia->currentIndex().row());
        IsStipendiaModel->submitAll();
        IsStipendiaModel->select();
    }
    else
    {
        return;
    }
}

void Stipendiya::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        this->typeSTIP_id = 0;
        ui->double_razm_stipen->clear();
        IsStipendiaModel->setFilter("");
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
        IsTypeStipendia->select();
        ui->comboBox_type_stipen->setModel(IsTypeStipendia);
        ui->comboBox_type_stipen->setModelColumn(1);
        ui->comboBox_type_stipen->setCurrentIndex(-1);
        this->typeSTIP_id = 0;
    }
}

void Stipendiya::on_pushButton_find_clicked()
{
    this->naideno.clear();
    ui->label_naideno->setVisible(true);
    this->formingSearch();
}

void Stipendiya::formingSearch()
{
    QString condition = "";

    if (ui->double_razm_stipen->value()!=0)
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " stp_size = " + QString::number(ui->double_razm_stipen->value());
    }

    if (this->typeSTIP_id != 0)
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " stp_type_id = " + QString::number(this->typeSTIP_id);
    }

    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    IsStipendiaModel->setFilter(condition);
    if(ui->tableView_stipendia->model()->rowCount()>0)
    {
        this->naideno.append("    Найдено: " + QString::number(ui->tableView_stipendia->model()->rowCount()));
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

void Stipendiya::on_pushButton_save_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    for(int i=0;i<ui->tableView_stipendia->model()->rowCount(); i++)
    {
        if(ui->tableView_stipendia->model()->index(i,1).data().isNull() || ui->tableView_stipendia->model()->index(i,2).data().isNull() || ui->tableView_stipendia->model()->index(i,3).data().isNull())
        {
            QMessageBox::information(this, tr("Информация"),tr("Поля 'Студент', 'Тип стипендии' и 'Размер стипендии' должны быть заполнены"));
            return;
        }
    }
    IsStipendiaModel->submitAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}


void Stipendiya::on_pushButton_cancel_clicked()
{
    IsStipendiaModel->revertAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные не сохранены"));
}

void Stipendiya::on_comboBox_type_stipen_activated(int index)
{
    this->typeSTIP_id=ui->comboBox_type_stipen->model()->index(index,0).data().toInt();
}

void Stipendiya::on_pushButton_edit_clicked()
{
    if (!ui->tableView_stipendia->model()->index(ui->tableView_stipendia->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
        return;
    }
    ui->tableView_stipendia->edit(ui->tableView_stipendia->currentIndex());
}
