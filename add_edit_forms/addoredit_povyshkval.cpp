#include "addoredit_povyshkval.h"
#include "ui_addoredit_povyshkval.h"
#include <QMessageBox>
#include <QFileDialog>
#include<qfiledialog.h>
#include <QFile>
#include <QDebug>
Addoredit_PovyshKval::Addoredit_PovyshKval(QWidget *parent, bool isEdit, int id_povyshKval) :
    QDialog(parent),
    id_povyshKval(id_povyshKval),
    isEdit(isEdit),
    ui(new Ui::Addoredit_PovyshKval)
{
    ui->setupUi(this);
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    ui->comboBox_sotr->setModel(dal_prepodcontrol->getcomboPrepod());
    ui->comboBox_sotr->setModelColumn(1);
    ui->comboBox_sotr->setCurrentIndex(-1);
    ui->label->setVisible(false);
    ui->lineEdit_ocenka->setValidator( new QDoubleValidator(0, 10000, 1, this));
    ui->lineEdit_ocenka->setPlaceholderText("Вводятся только цифры");
    ui->label_error->setVisible(false);
    ui->dateEdit->setDate(QDate::currentDate());
    if(this->isEdit)
    {
        this->setWindowTitle("Редактирование записи");
        QSqlQuery *query = new QSqlQuery;
        query = dal_prepodcontrol->getCurrentPovyskKval(this->id_povyshKval);
        for (int j = 0; j < ui->comboBox_sotr->count(); j++)
        {
            if (ui->comboBox_sotr->model()->index(j, 0).data().toInt() == query->value(1).toInt())
            {
                ui->comboBox_sotr->setCurrentIndex(j);
                ui->comboBox_sotr->view()->setCurrentIndex(ui->comboBox_sotr->model()->index(j, 0));
                break;
            }
        }

        ui->dateEdit->setDate(query->value(2).toDate());
        ui->lineEdit_tema->setText(query->value(3).toString());
        ui->lineEdit_kurs->setText(query->value(4).toString());
        ui->lineEdit_mesto->setText(query->value(5).toString());
        ui->lineEdit_sert->setText(query->value(6).toString());
        ui->lineEdit_file->setText(query->value(7).toString());
        ui->lineEdit_ocenka->setText(query->value(8).toString());
        this->UrlFail = query->value(9).toString();
        this->NameFails = query->value(7).toString();
    }
}

Addoredit_PovyshKval::~Addoredit_PovyshKval()
{
    delete ui;
}

void Addoredit_PovyshKval::on_pushButton_cancel_clicked()
{
    this->close();
}

void Addoredit_PovyshKval::on_pushButton_ok_clicked()
{
    bool error=false;
    if (ui->comboBox_sotr->currentIndex()==-1)
    {
        error = true;
    }
    else
    {
        error = false;
    }
    if (error == true)
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
            QFileInfo fi(this->UrlFail);
            QString names = fi.fileName();
            QFile::copy(this->UrlFail, "C:\\Dropbox\\" + names);
            this->UrlFail = "C:\\Dropbox\\" + names;
            if (dal_prepodcontrol->addPovyskKval(ui->comboBox_sotr->model()->index(ui->comboBox_sotr->currentIndex(),0).data().toInt(),
                                                 ui->dateEdit->date(),
                                                 ui->lineEdit_tema->text(),
                                                 ui->lineEdit_kurs->text(),
                                                 ui->lineEdit_mesto->text(),
                                                 ui->lineEdit_sert->text(),
                                                 this->NameFails,
                                                 ui->lineEdit_ocenka->text().toDouble(),
                                                 this->UrlFail))
            {

                QMessageBox::information(this, tr("Информация"), tr("Запись успешно добавлена"));
            }
            else
            {
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
            QFileInfo fi(this->UrlFail);
            QString names = fi.fileName();
            QFile::copy(this->UrlFail, "C:\\Dropbox\\" + names);
            this->UrlFail = "C:\\Dropbox\\" + names;
            if (dal_prepodcontrol->editPovyskKval(this->id_povyshKval,
                                                  ui->comboBox_sotr->model()->index(ui->comboBox_sotr->currentIndex(),0).data().toInt(),
                                                  ui->dateEdit->date(),
                                                  ui->lineEdit_tema->text(),
                                                  ui->lineEdit_kurs->text(),
                                                  ui->lineEdit_mesto->text(),
                                                  ui->lineEdit_sert->text(),
                                                  this->NameFails,
                                                  ui->lineEdit_ocenka->text().toDouble(),
                                                  this->UrlFail))
            {
                QMessageBox::information(this, tr("Информация"), tr("Данные успешно отредактированы"));
            }
            else
            {
                QMessageBox::warning(this, tr("Ошибка"), tr("Данные не были изменены"));
            }
        }
        this->close();
    }
}



void Addoredit_PovyshKval::on_pushButton_choose_clicked()
{
    this->NameFails = "";
    QString filename = QFileDialog::getOpenFileName(this,tr("open file"),"/home","All files(*.*)");
    filename = filename.replace("/" , "\\");

    this->UrlFail = filename;

    QString name =  QFileInfo(filename).baseName();
    ui->lineEdit_file->setText(name);
    this->NameFails = name;
}
