#include "addoreditolimp.h"
#include "ui_addoreditolimp.h"
#include <QMessageBox>

AddOrEditOlimp::AddOrEditOlimp(QWidget *parent, int select, int id) :
    QDialog(parent),
    ui(new Ui::AddOrEditOlimp)
{
    ui->setupUi(this);
    ui->lineEdit_zanMesto->setValidator( new QIntValidator(1, 1000000, this));
    ui->lineEdit_zanMesto->setPlaceholderText("Вводятся только цифры");
    this->stud_id=0;
    this->group_id=0;
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



    if (select == 1)
    {
        this->setWindowTitle(tr("Добавить новую запись"));
    }

    else
    {
        this->isEdit = true;
        this->setWindowTitle(tr("Редактирование записи"));
        this->rec_id = id;
        int ids_groups;
        int ids_student;

        QSqlQuery *query = new QSqlQuery;
        query = dal_studentsControl->getCurrentOlipm(this->rec_id);
        ids_groups = query->value(1).toInt();
        ids_student = query->value(2).toInt();
        comboStudent = dal_studentsControl->getStudentGroup(ids_groups);
        ui->comboBox_student->setModel(comboStudent);
        ui->comboBox_student->setModelColumn(1);
        ui->comboBox_student->setCurrentIndex(-1);
        ui->lineEdit_predmet->setText(query->value(3).toString());
        ui->dateEdit->setDate(query->value(4).toDate());
        ui->plainTextEdit_commen->setPlainText(query->value(5).toString());
        ui->lineEdit_zanMesto->setText(QString::number(query->value(6).toInt()));
        if(ui->lineEdit_zanMesto->text().toInt()==0)
            ui->lineEdit_zanMesto->setText("");
        ui->lineEdit_MestoProved->setText(query->value(7).toString());
        ui->lineEdit_proveryaushii->setText(query->value(8).toString());
        for (int j = 0; j < ui->comboBox_grupa->count(); j ++)
        {
            if (comboGroup->index(j, 0).data().toInt() == ids_groups)
            {
                ui->comboBox_grupa->setCurrentIndex(j);
                ui->comboBox_grupa->view()->setCurrentIndex(comboGroup->index(j, 0));
                break;
            }
        }
         qDebug()<<"ghghghg"<<ui->comboBox_student->count();
        for (int i = 0; i < ui->comboBox_student->count(); i ++)
        {
            if (comboStudent->index(i, 0).data().toInt() == ids_student)
            {
                ui->comboBox_student->setCurrentIndex(i);
                ui->comboBox_student->view()->setCurrentIndex(comboStudent->index(i, 0));
                break;
            }
        }
    }

}

AddOrEditOlimp::~AddOrEditOlimp()
{
    delete ui;
}

void AddOrEditOlimp::on_pushButton_ok_clicked()
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


    if(ui->lineEdit_predmet->text().isEmpty())
    {
        error = true;
    }


    if(ui->lineEdit_MestoProved->text().isEmpty())
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
            if (dal_studentsControl->getOlimpAdd(ui->comboBox_student->model()->index(ui->comboBox_student->currentIndex(), 0).data().toInt(), ui->lineEdit_predmet->text(),ui->dateEdit->date(),ui->plainTextEdit_commen->toPlainText(),ui->lineEdit_zanMesto->text().toInt(),ui->lineEdit_MestoProved->text(),ui->lineEdit_proveryaushii->text()))
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
            if (dal_studentsControl->editOlimp(this->rec_id,\
                                               ui->comboBox_student->model()->index(ui->comboBox_student->currentIndex(),0).data().toInt(),\
                                               ui->lineEdit_predmet->text(),\
                                               ui->dateEdit->date(),\
                                               ui->plainTextEdit_commen->toPlainText(),\
                                               ui->lineEdit_zanMesto->text().toInt(),\
                                               ui->lineEdit_MestoProved->text(),\
                                               ui->lineEdit_proveryaushii->text()))
            {
                this->close();
                QMessageBox::information(this, tr("Информация"), tr("Данные успешно отредактированы"));
            }
            else
            {
                this->close();
                QMessageBox::warning(this, tr("Ошибка"), tr("Данные не были изменены"));
            }
        }
    }
}

void AddOrEditOlimp::on_comboBox_grupa_activated(int index)
{
    this->group_id = ui->comboBox_grupa->model()->index(ui->comboBox_grupa->currentIndex(),0).data().toInt();
    QSqlQueryModel *comboStudent = new QSqlQueryModel(this);
    comboStudent = dal_studentsControl->getStudentGroup(this->group_id);
    ui->comboBox_student->setModel(comboStudent);
    ui->comboBox_student->setModelColumn(1);
    ui->comboBox_student->setCurrentIndex(-1);
}

void AddOrEditOlimp::on_comboBox_student_activated(int index)
{

}
