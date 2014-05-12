#include "addoredit_poruchenie.h"
#include "ui_addoredit_poruchenie.h"
#include <QMessageBox>
#include <QFileDialog>
#include<qfiledialog.h>
#include <QFile>
#include <QDebug>

Addoredit_Poruchenie::Addoredit_Poruchenie(QWidget *parent, bool isEdit, int id_porucheniya) :
    QDialog(parent),
    id_porucheniya(id_porucheniya),
    isEdit(isEdit),
    ui(new Ui::Addoredit_Poruchenie)
{
    ui->setupUi(this);
    ui->frame->setVisible(false);
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    ui->comboBox_sotr->setModel(dal_prepodcontrol->getcomboPrepod());
    ui->comboBox_sotr->setModelColumn(1);
    ui->comboBox_sotr->setCurrentIndex(-1);
    ui->label_error->setVisible(false);
    ui->dateEdit->setDate(QDate::currentDate());
    ui->pushButton_provereno->setVisible(false);
    if(this->isEdit)
    {
        this->setWindowTitle("Редактирование поручения");
        QSqlQuery *query = new QSqlQuery;
        query = dal_prepodcontrol->getCurrentPoruchenie(this->id_porucheniya);
        for (int j = 0; j < ui->comboBox_sotr->count(); j++)
        {
            if (ui->comboBox_sotr->model()->index(j, 0).data().toInt() == query->value(3).toInt())
            {
                ui->comboBox_sotr->setCurrentIndex(j);
                ui->comboBox_sotr->view()->setCurrentIndex(ui->comboBox_sotr->model()->index(j, 0));
                break;
            }
        }
        ui->lineEdit_title->setText(query->value(5).toString());
        ui->plainTextEdit_text->setPlainText(query->value(6).toString());
        ui->lineEdit_file->setText(query->value(7).toString());
        ui->dateEdit->setDate(query->value(11).toDate());

        if(query->value(4).toInt() == 4)
        {
            ui->pushButton_provereno->setVisible(true);
        }
    }
}

Addoredit_Poruchenie::~Addoredit_Poruchenie()
{
    delete ui;
}

void Addoredit_Poruchenie::on_pushButton_cancel_clicked()
{
    this->close();
}

void Addoredit_Poruchenie::on_pushButton_ok_clicked()
{
    if (ui->comboBox_sotr->currentIndex()==-1 || ui->lineEdit_title->text().isEmpty())
    {
        ui->label_error->setVisible(true);
    }
    else
    {
        if (! this->isEdit)
        {
            QFileInfo fi(this->UrlFail);
            QString names = fi.fileName();
            QFile::copy(this->UrlFail, "C:\\Dropbox\\" + names);
            this->UrlFail = "C:\\Dropbox\\" + names;
            if (! dal_main->checkConnection())
            {
                QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
                return;
            }
            if (dal_prepodcontrol->addPoruchenie(ui->lineEdit_title->text(),
                                                 ui->plainTextEdit_text->toPlainText(),
                                                 this->UrlFail,
                                                 ui->comboBox_sotr->model()->index(ui->comboBox_sotr->currentIndex(),0).data().toInt(),
                                                 ui->dateEdit->date()))
            {

                QMessageBox::information(this, tr("Информация"), tr("Поручение успешно выдано"));
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
            if (dal_prepodcontrol->editPoruchenie(this->id_porucheniya,
                                                  ui->lineEdit_title->text(),
                                                  ui->plainTextEdit_text->toPlainText(),
                                                  this->UrlFail,
                                                  ui->comboBox_sotr->model()->index(ui->comboBox_sotr->currentIndex(),0).data().toInt(),
                                                  ui->dateEdit->date()))
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


void Addoredit_Poruchenie::on_pushButton_provereno_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    if (dal_prepodcontrol->setStatusPorucheniya(this->id_porucheniya, 5, true, false))
    {
        QMessageBox::information(this, tr("Информация"), tr("Поручение проверено"));
    }
    else
    {
        QMessageBox::warning(this, tr("Ошибка"), tr("Стутус поручения не сменен"));
    }
    this->close();
}

void Addoredit_Poruchenie::on_pushButton_choose_clicked()
{
    this->NameFails = "";
    QString filename = QFileDialog::getOpenFileName(this,tr("open file"),"/home","All files(*.*)");
    filename = filename.replace("/" , "\\");

    this->UrlFail = filename;

    QString name =  QFileInfo(filename).baseName();
    ui->lineEdit_file->setText(name);
    this->NameFails = name;
}
