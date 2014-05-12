#include "addoreditvipusknik.h"
#include "ui_addoreditvipusknik.h"
#include <QMessageBox>
#include <QFileDialog>
#include<qfiledialog.h>
#include <QFile>
AddOrEditVipusknik::AddOrEditVipusknik(QWidget *parent, int select, int id, int viewsv) :
    QDialog(parent),
    ui(new Ui::AddOrEditVipusknik)
{
    ui->setupUi(this);
    ui->lineEdit_mail->setPlaceholderText("Пример: serega_90.18@mail.ru");
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
        if(viewsv==3)
        {
            this->setWindowTitle(tr("Подробная информация об выпускнике"));
            ui->label_grup->setText("Группа");
            ui->comboBox_grupa->setEnabled(false);
            ui->label_stud->setText("Студент");
            ui->comboBox_student->setEnabled(false);
            ui->label_adresEmail->setText("Адрес эл. почты");
            ui->lineEdit_mail->setReadOnly(true);
            ui->label_datas->setText("Дата");
            ui->dateEdit->setReadOnly(true);
            ui->label_dolj->setText("Должность");
            ui->lineEdit_doljn->setReadOnly(true);
            ui->label_mestoRaboti->setText("Место работы");
            ui->lineEdit_workspace->setReadOnly(true);
            ui->label_nomer_dipl->setText("Номер диплома");
            ui->lineEdit_nomer_diploma->setReadOnly(true);
            ui->label_telef->setText("Номер телефона");
            ui->lineEdit_tel->setReadOnly(true);
            ui->pushButton_addFoto->setVisible(false);
        }

        else
            this->setWindowTitle(tr("Редактирование записи"));
        this->rec_id = id;
        int ids_groups;
        int ids_student;

        QSqlQuery *query = new QSqlQuery;
        query = dal_studentsControl->getCurrentVipusknik(this->rec_id);
        ids_groups = query->value(2).toInt();
        ids_student = query->value(1).toInt();
        comboStudent = dal_studentsControl->getStudentGroup(ids_groups);
        ui->comboBox_student->setModel(comboStudent);
        ui->comboBox_student->setModelColumn(1);
        ui->comboBox_student->setCurrentIndex(-1);
        ui->lineEdit_nomer_diploma->setText(query->value(3).toString());
        ui->label_foto_diploma->setPixmap(query->value(4).toString());
        ui->label_foto_diploma->setScaledContents(true);
        this->foto = query->value(4).toString();
        ui->dateEdit->setDate(query->value(5).toDate());
        ui->lineEdit_mail->setText(query->value(6).toString());
        ui->lineEdit_tel->setText(query->value(7).toString());
        ui->lineEdit_workspace->setText(query->value(8).toString());
        ui->lineEdit_doljn->setText(query->value(9).toString());

        for (int j = 0; j < ui->comboBox_grupa->count(); j ++)
        {
            if (comboGroup->index(j, 0).data().toInt() == ids_groups)
            {
                ui->comboBox_grupa->setCurrentIndex(j);
                ui->comboBox_grupa->view()->setCurrentIndex(comboGroup->index(j, 0));
                break;
            }
        }
        //       comboStudent->setQuery("select * from is_student where id_student = " + QString::number(ids_student));
        //       ui->comboBox_student->setCurrentIndex(0);
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

AddOrEditVipusknik::~AddOrEditVipusknik()
{

    delete ui;
}

void AddOrEditVipusknik::on_comboBox_grupa_activated(int index)
{
    this->group_id = ui->comboBox_grupa->model()->index(ui->comboBox_grupa->currentIndex(),0).data().toInt();
    QSqlQueryModel *comboStudent = new QSqlQueryModel(this);
    comboStudent = dal_studentsControl->getStudentGroup(this->group_id);
    ui->comboBox_student->setModel(comboStudent);
    ui->comboBox_student->setModelColumn(1);
    ui->comboBox_student->setCurrentIndex(-1);
}

void AddOrEditVipusknik::on_pushButton_addFoto_clicked()
{
    this->foto = "";
    QString filename = QFileDialog::getOpenFileName(this,tr("open foto"),"/home","Image files(*.png *.jpg *.bmp)");
    filename = filename.replace("/" , "\\");
    ui->label_foto_diploma->setPixmap(filename);
    ui->label_foto_diploma->setScaledContents(true);
    this->foto = filename;
    if(this->foto!="")
        ui->label_foto_diploma->setText("");

}

void AddOrEditVipusknik::on_pushButton_ok_clicked()
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
    
    
    if(ui->lineEdit_nomer_diploma->text().isEmpty())
    {
        error = true;
    }
    
    if(ui->label_foto_diploma->text() == "Фото диплома")
    {
        error = true;
    }
    int prov=0;
    prov = ui->lineEdit_mail->text().count("@");
    if(prov==0)
    {
        QMessageBox::information(this,tr("Информация"), tr("Поле ' адрес почты' введено не верно"));
        return;
    }
    else
    {
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
                QFileInfo fi(this->foto);
                QString name = fi.fileName();
                QFile::copy(this->foto, "C:\\Dropbox\\" + name);
                this->foto = "C:\\Dropbox\\" + name;

                if (dal_studentsControl->getVipusknikAdd(ui->comboBox_student->model()->index(ui->comboBox_student->currentIndex(), 0).data().toInt(), ui->lineEdit_nomer_diploma->text(),this->foto,ui->dateEdit->date(),ui->lineEdit_mail->text(),ui->lineEdit_tel->text(),ui->lineEdit_workspace->text(),ui->lineEdit_doljn->text()))
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
                QFileInfo fi(this->foto);
                QString name = fi.fileName();
                QFile::copy(this->foto, "C:\\Dropbox\\" + name);
                this->foto = "C:\\Dropbox\\" + name;
                if (dal_studentsControl->editVipusknik(this->rec_id,\
                                                       ui->comboBox_student->model()->index(ui->comboBox_student->currentIndex(),0).data().toInt(),\
                                                       ui->lineEdit_nomer_diploma->text(),\
                                                       this->foto,\
                                                       ui->dateEdit->date(),\
                                                       ui->lineEdit_mail->text(),\
                                                       ui->lineEdit_tel->text(),\
                                                       ui->lineEdit_workspace->text(),\
                                                       ui->lineEdit_doljn->text()))
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
}

void AddOrEditVipusknik::on_pushButton_cancel_clicked()
{
    this->close();
}

void AddOrEditVipusknik::on_comboBox_student_activated(int index)
{

}
