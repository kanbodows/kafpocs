#include "course.h"
#include "ui_course.h"
#include <QMessageBox>

bool Course::isOpen = false;
Course::Course(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Course)
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
    CourseModel = new QSqlTableModel(this);
    CourseModel->setTable("is_course");
    ui->tableView_Course->setModel(CourseModel);
    CourseModel->setHeaderData(1,Qt::Horizontal,tr("Название курса"));
    CourseModel->setHeaderData(2,Qt::Horizontal,tr("Семестр"));
    ui->tableView_Course->setColumnHidden(0,true);
    ui->tableView_Course->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    CourseModel->select();

}

Course::~Course()
{
    isOpen=false;
    delete ui;
}

void Course::on_pushButton_save_clicked()
{
    CourseModel->submitAll();
    QMessageBox::information(this,tr("Информация"),tr("Данные сохранены"));
}

void Course::on_pushButton_cancel_clicked()
{
    CourseModel->revertAll();
    QMessageBox::information(this,tr("Информация"),tr("Данные не были сохранены"));
}

void Course::on_pushButton_add_clicked()
{
    CourseModel->insertRow(CourseModel->rowCount());
    ui->tableView_Course->edit(CourseModel->index(CourseModel->rowCount() - 1, 1));
}

bool Course::checkRelation(QString id_course)
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return NULL;
    }
    QSqlQuery query;
    query.prepare("select course_id from is_semmestr where course_id = :id_course");
    query.bindValue(":id_course", id_course);
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

void Course::on_pushButton_dell_clicked()
{
    if (!ui->tableView_Course->model()->index(ui->tableView_Course->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка удаления"), tr("Ни одной записи не выбрано"));
        return;
    }
    if (this->checkRelation(ui->tableView_Course->model()->index(ui->tableView_Course->currentIndex().row(), 0).data().toString()))
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
            CourseModel->removeRow(ui->tableView_Course->currentIndex().row());
            CourseModel->submitAll();
            CourseModel->select();
        }
        else
        {
            return;
        }
    }
}

void Course::on_pushButton_update_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    CourseModel->select();

}

void Course::on_pushButton_edit_clicked()
{
    if (!ui->tableView_Course->model()->index(ui->tableView_Course->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
        return;
    }
    ui->tableView_Course->edit(ui->tableView_Course->currentIndex());
}
