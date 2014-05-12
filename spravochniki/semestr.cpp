#include "semestr.h"
#include "ui_semestr.h"
#include <QMessageBox>
#include "delegates/textnndelegate.h"
bool Semestr::isOpen = false;
Semestr::Semestr(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Semestr)
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
    SemestrModel = new QSqlRelationalTableModel(this);
    SemestrModel->setTable("is_semmestr");
    SemestrModel->setRelation(1, QSqlRelation("is_course", "id_course", "course_name"));
    SemestrModel->setHeaderData(1,Qt::Horizontal,tr("Курса"));
    SemestrModel->setHeaderData(2,Qt::Horizontal,tr("Семестр"));
    SemestrModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    SemestrModel->select();
    CourseWorkModel2 = new QSqlTableModel(this);
    CourseWorkModel2->setTable("is_course");
    ui->tableView_semestr->setModel(SemestrModel);
    ui->tableView_semestr->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_semestr->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_semestr->setColumnHidden(0,true);
    ui->tableView_semestr->setItemDelegate(new QSqlRelationalDelegate(ui->tableView_semestr));
    ui->tableView_semestr->setItemDelegateForColumn(2, new textNnDelegate(ui->tableView_semestr));

    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    this->id_cour=0;
}

Semestr::~Semestr()
{
    isOpen=false;
    delete ui;
}

void Semestr::on_pushButton_add_clicked()
{
    SemestrModel->insertRow(SemestrModel->rowCount());
    ui->tableView_semestr->edit(SemestrModel->index(SemestrModel->rowCount() - 1, 1));
    ui->tableView_semestr->setCurrentIndex(SemestrModel->index(SemestrModel->rowCount()-1,1));
}

void Semestr::on_pushButton_save_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    for(int i=0;i<ui->tableView_semestr->model()->rowCount(); i++)
    {
        if(ui->tableView_semestr->model()->index(i,2).data().isNull())
        {
            QMessageBox::information(this, tr("Информация"),tr("Поле 'Семестр' не должно быть пустым"));
            return;
        }

    }
    SemestrModel->submitAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}

void Semestr::on_pushButton_update_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }


    SemestrModel->select();
    CourseWorkModel2->select();


   if (this->id_cour != 0)
    {
        for (int i = 0; i < ui->comboBox_course->count(); i++)
        {
            if (CourseWorkModel2->index(i, 0).data().toInt() == this->id_cour)
            {
                ui->comboBox_course->setCurrentIndex(i);
                ui->comboBox_course->view()->setCurrentIndex(CourseWorkModel2->index(i, 0));
                break;
            }
        }
    }
}

void Semestr::on_pushBatton_cancel_clicked()
{
    SemestrModel->revertAll();
     QMessageBox::information(this, tr("Информация"), tr("Данные не сохранены"));
}

bool Semestr::checkRelation(QString id_semestr)
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return NULL;
    }
    QSqlQuery query;
    query.prepare("select d_semestr from is_discipline where d_semestr = :id_semestr");
    query.bindValue(":id_semestr", id_semestr);
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

void Semestr::on_pushButton_del_clicked()
{
    if (!ui->tableView_semestr->model()->index(ui->tableView_semestr->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка удаления"), tr("Ни одной записи не выбрано"));
        return;
    }
    if (this->checkRelation(ui->tableView_semestr->model()->index(ui->tableView_semestr->currentIndex().row(), 0).data().toString()))
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
            SemestrModel->removeRow(ui->tableView_semestr->currentIndex().row());
            SemestrModel->submitAll();
            SemestrModel->select();
        }
        else
        {
            return;
        }
    }
}



void Semestr::on_pushButton_find_clicked()
{
    this->naideno.clear();
    ui->label_naideno->setVisible(true);
    this->formingSearch();
}
void Semestr::on_comboBox_course_activated(int index)
{
    this->id_cour = ui->comboBox_course->model()->index(index, 0).data().toInt();
}

void Semestr::formingSearch()
{
    QString condition = "";

    if (! ui->lineEdit_nazvanie->text().isEmpty())
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " semestr_name like '%" + ui->lineEdit_nazvanie->text() + "%'";
    }

    if (this->id_cour != 0)
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " course_id = " + QString::number(this->id_cour);
    }

    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    SemestrModel->setFilter(condition);
    if(ui->tableView_semestr->model()->rowCount()>0)
    {
    this->naideno.append("    Найдено: " + QString::number(ui->tableView_semestr->model()->rowCount()));
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

void Semestr::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {

       // ui->pushButton_search->setStyleSheet(css->currentStyle);
        ui->groupBox_search->setVisible(false);
        this->id_cour = 0;
        ui->lineEdit_nazvanie->clear();
        SemestrModel->setFilter("");
        ui->label_naideno->setVisible(false);
        ui->label_result->setVisible(false);
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
      //  ui->pushButton_search->setStyleSheet(css->currentPressedStyle);
        CourseWorkModel2->select();
        ui->comboBox_course->setModel(CourseWorkModel2);
        ui->comboBox_course->setModelColumn(1);
        ui->comboBox_course->setCurrentIndex(-1);
        this->id_cour = 0;
        connect(ui->lineEdit_nazvanie,SIGNAL(editingFinished()),this,SLOT(formingSearch()));
    }
}



void Semestr::on_pushButton_clear_clicked()
{
    this->id_cour = 0;
    ui->lineEdit_nazvanie->clear();
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    SemestrModel->setFilter("");
    SemestrModel->select();
    ui->comboBox_course->setModel(CourseWorkModel2);
    ui->comboBox_course->setModelColumn(1);
    ui->comboBox_course->setCurrentIndex(-1);
}


void Semestr::on_pushButton_edit_clicked()
{
    if (!ui->tableView_semestr->model()->index(ui->tableView_semestr->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
        return;
    }
    ui->tableView_semestr->edit(ui->tableView_semestr->currentIndex());
}
