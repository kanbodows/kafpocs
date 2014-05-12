#include "addoreditactvnedrenia.h"
#include "ui_addoreditactvnedrenia.h"
#include <QMessageBox>
AddOrEditActVnedrenia::AddOrEditActVnedrenia(QWidget *parent, int select, int id) :
    QDialog(parent),
    ui(new Ui::AddOrEditActVnedrenia)
{
    ui->setupUi(this);
    this->stud_id=0;
    this->group_id=0;
    this->tema_id=0;
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    this->isEdit = false;
    dal_studentsControl = new Dal_studentsControl(this);
    QSqlQueryModel *comboGroup = new QSqlQueryModel(this);
    comboGroup = dal_studentsControl->getComboGroup(0);
    ui->dateEdit->setDate(QDate::currentDate());
    ui->comboBox_grupa->setModel(comboGroup);
    ui->comboBox_grupa->setModelColumn(1);
    ui->comboBox_grupa->setCurrentIndex(-1);
    ui->label_error->setVisible(false);
    QSqlQueryModel *comboStudent = new QSqlQueryModel(this);
    comboStudent = dal_studentsControl->getStudentGroup(this->group_id);
    ui->comboBox_student->setModel(comboStudent);
    ui->comboBox_student->setModelColumn(1);
    ui->comboBox_student->setCurrentIndex(-1);

    QSqlQueryModel *combotema = new QSqlQueryModel(this);
    combotema = dal_studentsControl->getTemadiploma();
    ui->comboBox_tema->setModel(combotema);
    ui->comboBox_tema->setModelColumn(1);
    ui->comboBox_tema->setCurrentIndex(-1);


    if (select == 1)
    {
        this->sel=false;
        this->setWindowTitle(tr("Добавить новую запись"));
    }
    else
    {
        this->isEdit = true;
        this->sel=true;
        this->setWindowTitle(tr("Редактирование записи"));
        this->rec_id = id;
        int ids_groups;
        int ids_student;


        QSqlQuery *query = new QSqlQuery;
        query = dal_studentsControl->getCurrentActVnedr(this->rec_id);
        ids_groups = query->value(2).toInt();
        ids_student = query->value(1).toInt();
        ids_tema = query->value(3).toInt();
        this->ids_tema2 = ids_tema;
        comboStudent = dal_studentsControl->getStudentGroup(ids_groups);
        ui->comboBox_student->setModel(comboStudent);
        ui->comboBox_student->setModelColumn(1);
        ui->comboBox_student->setCurrentIndex(-1);
        ui->lineEdit_nomer_acta->setText(query->value(6).toString());
        ui->lineEdit_predpriyatie->setText(query->value(7).toString());
        ui->dateEdit->setDate(query->value(8).toDate());
        ui->plainTextEdit_dopolnenia->setPlainText(query->value(11).toString());
        for (int j = 0; j < ui->comboBox_grupa->count(); j ++)
        {
            if (comboGroup->index(j, 0).data().toInt() == ids_groups)
            {
                ui->comboBox_grupa->setCurrentIndex(j);
                ui->comboBox_grupa->view()->setCurrentIndex(comboGroup->index(j, 0));
                break;
            }
        }
        for (int i = 0; i < ui->comboBox_student->count(); i ++)
        {
            if (comboStudent->index(i, 0).data().toInt() == ids_student)
            {
                ui->comboBox_student->setCurrentIndex(i);
                ui->comboBox_student->view()->setCurrentIndex(comboStudent->index(i, 0));
                break;
            }
        }

        for (int i = 0; i < ui->comboBox_tema->count(); i ++)
        {
            if (combotema->index(i, 0).data().toInt() == ids_tema)
            {
                ui->comboBox_tema->setCurrentIndex(i);
                ui->comboBox_tema->view()->setCurrentIndex(combotema->index(i, 0));
                break;
            }
        }
    }
}

AddOrEditActVnedrenia::~AddOrEditActVnedrenia()
{
    delete ui;
}

void AddOrEditActVnedrenia::on_comboBox_grupa_activated(int index)
{

    this->group_id = ui->comboBox_grupa->model()->index(ui->comboBox_grupa->currentIndex(),0).data().toInt();
    QSqlQueryModel *comboStudent = new QSqlQueryModel(this);
    comboStudent = dal_studentsControl->getStudentGroup(this->group_id);
    ui->comboBox_student->setModel(comboStudent);
    ui->comboBox_student->setModelColumn(1);
    ui->comboBox_student->setCurrentIndex(-1);

}

void AddOrEditActVnedrenia::on_radioButton_tema_kg_clicked()
{
    if(this->sel==false)
    {
        QSqlQueryModel *combotema = new QSqlQueryModel(this);
        combotema = dal_studentsControl->getTemadiploma();
        ui->comboBox_tema->setModel(combotema);
        ui->comboBox_tema->setModelColumn(2);
        ui->comboBox_tema->setCurrentIndex(-1);
    }
    else
    {
        QSqlQuery *query = new QSqlQuery;
        query = dal_studentsControl->getCurrentActVnedr(this->rec_id);

        ids_tema = query->value(3).toInt();
        if(ids_tema!=this->ids_tema2)
        {
            QSqlQueryModel *combotema = new QSqlQueryModel(this);
            combotema = dal_studentsControl->getTemadiploma();
            ui->comboBox_tema->setModel(combotema);
            ui->comboBox_tema->setModelColumn(2);
            ui->comboBox_tema->setCurrentIndex(-1);

        }
        else{
            QSqlQueryModel *combotema = new QSqlQueryModel(this);
            combotema = dal_studentsControl->getTemadiploma();
            ui->comboBox_tema->setModel(combotema);
            ui->comboBox_tema->setModelColumn(2);
            ui->comboBox_tema->setCurrentIndex(-1);
            for (int i = 0; i < ui->comboBox_tema->count(); i ++)
            {
                if (combotema->index(i, 0).data().toInt() == ids_tema)
                {
                    ui->comboBox_tema->setCurrentIndex(i);
                    ui->comboBox_tema->view()->setCurrentIndex(combotema->index(i, 0));
                    break;
                }
            }
        }
    }
}

void AddOrEditActVnedrenia::on_radioButton_tema_rus_clicked()
{
    if(this->sel==false)
    {
        QSqlQueryModel *combotema = new QSqlQueryModel(this);
        combotema = dal_studentsControl->getTemadiploma();
        ui->comboBox_tema->setModel(combotema);
        ui->comboBox_tema->setModelColumn(1);
        ui->comboBox_tema->setCurrentIndex(-1);
    }

    else
    {
        QSqlQuery *query = new QSqlQuery;
        query = dal_studentsControl->getCurrentActVnedr(this->rec_id);

        ids_tema = query->value(3).toInt();
        qDebug()<<ids_tema<<ids_tema2;
        if(ids_tema!=this->ids_tema2)
        {
            QSqlQueryModel *combotema = new QSqlQueryModel(this);
            combotema = dal_studentsControl->getTemadiploma();
            ui->comboBox_tema->setModel(combotema);
            ui->comboBox_tema->setModelColumn(1);
            ui->comboBox_tema->setCurrentIndex(-1);
            return;
        }
        else
        {
            QSqlQueryModel *combotema = new QSqlQueryModel(this);
            combotema = dal_studentsControl->getTemadiploma();
            ui->comboBox_tema->setModel(combotema);
            ui->comboBox_tema->setModelColumn(1);
            ui->comboBox_tema->setCurrentIndex(-1);
            for (int i = 0; i < ui->comboBox_tema->count(); i ++)
            {
                if (combotema->index(i, 0).data().toInt() == ids_tema)
                {
                    ui->comboBox_tema->setCurrentIndex(i);
                    ui->comboBox_tema->view()->setCurrentIndex(combotema->index(i, 0));
                    break;
                }
            }
        }
    }

}

void AddOrEditActVnedrenia::on_pushButton_ok_clicked()
{
    bool error = false;
    if (ui->comboBox_grupa->currentIndex()==-1)
    {
        error = true;
    }

    if (ui->comboBox_student->currentIndex()==-1)
    {
        error = true;
    }

    if (ui->comboBox_tema->currentIndex()==-1)
    {
        error = true;
    }



    if(ui->lineEdit_nomer_acta->text().isEmpty())
    {
        error = true;
    }

    if(ui->lineEdit_predpriyatie->text().isEmpty())
    {
        error = true;
    }


    if (error==true)
    {
        ui->label_error->setVisible(true);
        return;
    }
    else
    {
        if (! this->isEdit)
        {
            if (! dal_main->checkConnection())
            {
                QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
                return;
            }
            if (dal_studentsControl->getActVnedreniaAdd(ui->comboBox_student->model()->index(ui->comboBox_student->currentIndex(), 0).data().toInt(), ui->lineEdit_nomer_acta->text(),ui->dateEdit->date(),ui->lineEdit_predpriyatie->text(),ui->comboBox_tema->model()->index(ui->comboBox_tema->currentIndex(), 0).data().toInt(),ui->plainTextEdit_dopolnenia->toPlainText()))
            {
                this->close();
                QMessageBox::information(this, tr("Информация"), tr("Запись успешно добавлена"));
            }
            else
            {
                this->close();
                QMessageBox::warning(this, tr("Ошибка"), tr("Данные не были добавлены в базу данных"));
            }
        }
        else
        {
            if (! dal_main->checkConnection())
            {
                QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
                return;
            }
            if (dal_studentsControl->editActVnedr(this->rec_id,\
                                                  ui->comboBox_student->model()->index(ui->comboBox_student->currentIndex(),0).data().toInt(),\
                                                  ui->lineEdit_nomer_acta->text(),\
                                                  ui->dateEdit->date(),\
                                                  ui->lineEdit_predpriyatie->text(),\
                                                  ui->comboBox_tema->model()->index(ui->comboBox_tema->currentIndex(),0).data().toInt(),\
                                                  ui->plainTextEdit_dopolnenia->toPlainText()))
            {
                this->close();
                QMessageBox::information(this, tr("Информация"), tr("Данные успешно отредактированы"));
            }
            else
            {
                this->close();
                QMessageBox::information(this, tr("Информация"), tr("Данные не были изменены"));
            }
        }
    }
}

void AddOrEditActVnedrenia::on_pushButton_cancel_clicked()
{
    this->close();
    QMessageBox::information(this, tr("Информация"), tr("Данные не были изменены"));
}

void AddOrEditActVnedrenia::on_comboBox_tema_activated(int index)
{
    this->ids_tema2 =  ui->comboBox_student->model()->index(ui->comboBox_student->currentIndex(),0).data().toInt();
}
