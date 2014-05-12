#include "issport.h"
#include "ui_issport.h"
#include <QMessageBox>
bool IsSport::isOpen = false;

IsSport::IsSport(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::IsSport)
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
    IsSportModel = new QSqlRelationalTableModel(this);
    IsSportModel->setTable("is_sport");
    IsSportModel->setHeaderData(1,Qt::Horizontal,tr("Студент"));
    IsSportModel->setHeaderData(2,Qt::Horizontal,tr("Вид спорта"));
    IsSportModel->setHeaderData(3,Qt::Horizontal,tr("Место проведения"));
    IsSportModel->setHeaderData(4,Qt::Horizontal,tr("Место"));
    IsSportModel->setHeaderData(5,Qt::Horizontal,tr("Награды"));
    IsSportModel->setRelation(1, QSqlRelation("is_student", "id_student", "s_fio"));
    IsSportModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    IsSportModel->select();
    studentModel = new QSqlTableModel(this);
    studentModel->setTable("is_student");
    ui->tableView_sport->setModel(IsSportModel);
    ui->tableView_sport->setColumnHidden(0,true);
    ui->tableView_sport->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_sport->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_sport->setColumnHidden(0,true);
    ui->tableView_sport->setItemDelegate(new QSqlRelationalDelegate(ui->tableView_sport));
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
}

IsSport::~IsSport()
{
    isOpen=false;
    delete ui;
}

void IsSport::on_pushButton_add_clicked()
{
    qDebug()<<QString::number(dal_main->getCurrentRole());
    IsSportModel->insertRow(IsSportModel->rowCount());
    ui->tableView_sport->edit(IsSportModel->index(IsSportModel->rowCount() - 1, 1));
    ui->tableView_sport->setCurrentIndex(IsSportModel->index(IsSportModel->rowCount()-1,1));
}

void IsSport::on_pushButton_update_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }

    IsSportModel->select();
    studentModel->select();

    if (this->student_id != 0)
    {
        for (int i = 0; i < ui->comboBox_student->count(); i++)
        {
            if (studentModel->index(i, 0).data().toInt() == this->student_id)
            {
                ui->comboBox_student->setCurrentIndex(i);
                ui->comboBox_student->view()->setCurrentIndex(studentModel->index(i, 0));
                break;
            }
        }
    }

}

void IsSport::on_pushButton_del_clicked()
{
    if (!ui->tableView_sport->model()->index(ui->tableView_sport->currentIndex().row(), 0).data().isValid())
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
        IsSportModel->removeRow(ui->tableView_sport->currentIndex().row());
        IsSportModel->submitAll();
        IsSportModel->select();
    }
    else
    {
        return;
    }
}

void IsSport::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        this->student_id = 0;
        ui->lineEdit_nazvanie->clear();
        ui->lineEdit_nagradi->clear();
        IsSportModel->setFilter("");
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
        studentModel->select();
        ui->comboBox_student->setModel(studentModel);
        ui->comboBox_student->setModelColumn(1);
        ui->comboBox_student->setCurrentIndex(-1);
        this->student_id = 0;
    }
}

void IsSport::on_pushButton_find_clicked()
{
    this->naideno.clear();
    ui->label_naideno->setVisible(true);
    this->formingSearch();
}

void IsSport::on_pushButton_clear_clicked()
{
    this->student_id = 0;
    this->naideno.clear();
    ui->label_naideno->clear();
    ui->lineEdit_nazvanie->clear();
    ui->lineEdit_nagradi->clear();
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    IsSportModel->setFilter("");
    IsSportModel->select();
    ui->comboBox_student->setModel(studentModel);
    ui->comboBox_student->setModelColumn(1);
    ui->comboBox_student->setCurrentIndex(-1);
}

void IsSport::formingSearch()
{
    QString condition = "";

    if (! ui->lineEdit_nazvanie->text().isEmpty())
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " vid_sporta like '%" + ui->lineEdit_nazvanie->text() + "%'";
    }


    if (! ui->lineEdit_nagradi->text().isEmpty())
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " nagradi like '%" + ui->lineEdit_nagradi->text() + "%'";
    }

    if (this->student_id != 0)
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " student = " + QString::number(this->student_id);
    }

    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    IsSportModel->setFilter(condition);
    if(ui->tableView_sport->model()->rowCount()>0)
    {
        this->naideno.append("    Найдено: " + QString::number(ui->tableView_sport->model()->rowCount()));
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

void IsSport::on_comboBox_student_activated(int index)
{
    this->student_id = ui->comboBox_student->model()->index(index,0).data().toInt();
}

void IsSport::on_pushButton_save_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    for(int i=0;i<ui->tableView_sport->model()->rowCount(); i++)
    {
        if(ui->tableView_sport->model()->index(i,1).data().isNull() || ui->tableView_sport->model()->index(i,2).data().isNull() || ui->tableView_sport->model()->index(i,3).data().isNull())

        {
            QMessageBox::information(this, tr("Информация"),tr("Поля 'Студент', 'Вид спорта' и 'Место проведения' должны быть заполнены"));
            return;
        }
    }
    IsSportModel->submitAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}


void IsSport::on_pushButton_cancel_clicked()
{
    IsSportModel->revertAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные не сохранены"));
}

void IsSport::on_pushButton_edit_clicked()
{
    if (!ui->tableView_sport->model()->index(ui->tableView_sport->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
        return;
    }
    ui->tableView_sport->edit(ui->tableView_sport->currentIndex());
}
