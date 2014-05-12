#include "addoredit_metodichki.h"
#include "ui_addoredit_metodichki.h"
#include <QMessageBox>
#include <QFileDialog>
#include<qfiledialog.h>
#include <QFile>
#include <QDebug>
addoredit_metodichki::addoredit_metodichki(QWidget *parent, bool isEdit, int id_met, int discipline_id) :
    QDialog(parent),
    id_met(id_met),
    isEdit(isEdit),
    discipline_id(discipline_id),
    ui(new Ui::addoredit_metodichki)
{
    ui->setupUi(this);
    ui->lineEdit_god->setDate(QDate::currentDate());
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    if(this->discipline_id == -1)// если добавляем через форму metodichki
    {
        ui->comboBox_discipline->setModel(dal_prepodcontrol->getDisciplines());
        ui->comboBox_discipline->setModelColumn(1);
        ui->comboBox_discipline->setCurrentIndex(-1);
    }
    else // иначе идет добавление на дисциплин где этот комбобокс не нужен, так как дисциплина была уже выбрана (this->discipline_id)
    {
        ui->comboBox_discipline->setVisible(false);
        ui->label->setVisible(false);
    }
    ui->comboBox_sotr->setModel(dal_prepodcontrol->getcomboPrepod());
    ui->comboBox_sotr->setModelColumn(1);
    ui->comboBox_sotr->setCurrentIndex(-1);
    ui->comboBox_type->setModel(dal_prepodcontrol->getMetTypes());
    ui->comboBox_type->setModelColumn(1);
    ui->comboBox_type->setCurrentIndex(-1);

    ui->label_error->setVisible(false);
    if(this->isEdit)
    {
        this->setWindowTitle("Редактирование записи");
        QSqlQuery *query = new QSqlQuery;
        query = dal_prepodcontrol->getCurrentMetodichka(this->id_met);
        if(this->discipline_id == -1)// если добавляем через форму metodichki
        {
            for (int j = 0; j < ui->comboBox_discipline->count(); j++)
            {
                if (ui->comboBox_discipline->model()->index(j, 0).data().toInt() == query->value(1).toInt())
                {
                    ui->comboBox_discipline->setCurrentIndex(j);
                    ui->comboBox_discipline->view()->setCurrentIndex(ui->comboBox_discipline->model()->index(j, 0));
                    break;
                }
            }
           this->discipline_id = query->value(1).toInt();
            qDebug()<< "asdasda" <<this->discipline_id ;
        }

        for (int j = 0; j < ui->comboBox_sotr->count(); j++)
        {
            if (ui->comboBox_sotr->model()->index(j, 0).data().toInt() == query->value(7).toInt())
            {
                ui->comboBox_sotr->setCurrentIndex(j);
                ui->comboBox_sotr->view()->setCurrentIndex(ui->comboBox_sotr->model()->index(j, 0));
                break;
            }
        }
        for (int i = 0; i < ui->comboBox_type->count(); i++)
        {
            if (ui->comboBox_type->model()->index(i, 0).data().toInt() == query->value(6).toInt())
            {
                ui->comboBox_type->setCurrentIndex(i);
                ui->comboBox_type->view()->setCurrentIndex(ui->comboBox_type->model()->index(i, 0));
                break;
            }
        }
        ui->lineEdit_nazv->setText(query->value(2).toString());
        ui->lineEdit_avtor->setText(query->value(3).toString());
        ui->lineEdit_file->setText(query->value(4).toString());
        ui->lineEdit_god->setDate(query->value(5).toDate());
        this->NameFails = query->value(4).toString();
        this->UrlFail = query->value(8).toString();
        this->discipline_id = query->value(1).toInt();
    }
}

addoredit_metodichki::~addoredit_metodichki()
{
    delete ui;
}

void addoredit_metodichki::on_pushButton_cancel_clicked()
{
    this->close();
}

void addoredit_metodichki::on_pushButton_ok_clicked()
{
    if (ui->comboBox_type->currentIndex()==-1 || ui->lineEdit_nazv->text().isEmpty() || (this->discipline_id == -1 && ui->comboBox_discipline->currentIndex()==-1))
    {
        ui->label_error->setVisible(true);
        return;
    }
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    if (! this->isEdit)
    {
        QFileInfo fi(this->UrlFail);
        QString names = fi.fileName();
        QFile::copy(this->UrlFail, "C:\\Dropbox\\" + names);
        this->UrlFail = "C:\\Dropbox\\" + names;
        if (dal_prepodcontrol->addMetodichka(this->discipline_id,
                                             ui->lineEdit_nazv->text(),
                                             ui->lineEdit_avtor->text(),
                                             this->NameFails,
                                             ui->lineEdit_god->date(),
                                             ui->comboBox_type->model()->index(ui->comboBox_type->currentIndex(),0).data().toInt(),
                                             ui->comboBox_sotr->model()->index(ui->comboBox_sotr->currentIndex(),0).data().toInt(),
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
        QFileInfo fi(this->UrlFail);
        QString names = fi.fileName();
        QFile::copy(this->UrlFail, "C:\\Dropbox\\" + names);
        this->UrlFail = "C:\\Dropbox\\" + names;
        if (dal_prepodcontrol->editMetodichka(this->id_met,
                                              this->discipline_id,
                                              ui->lineEdit_nazv->text(),
                                              ui->lineEdit_avtor->text(),
                                              ui->lineEdit_file->text(),
                                              ui->lineEdit_god->date(),
                                              ui->comboBox_type->model()->index(ui->comboBox_type->currentIndex(),0).data().toInt(),
                                              ui->comboBox_sotr->model()->index(ui->comboBox_sotr->currentIndex(),0).data().toInt(),
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



void addoredit_metodichki::on_pushButton_choose_clicked()
{
    this->NameFails = "";
    QString filename = QFileDialog::getOpenFileName(this,tr("open file"),"/home","All files(*.*)");
    filename = filename.replace("/" , "\\");

    this->UrlFail = filename;

    QString name =  QFileInfo(filename).baseName();
    ui->lineEdit_file->setText(name);
    this->NameFails = name;
}

void addoredit_metodichki::on_comboBox_discipline_activated(int index)
{
    this->discipline_id = ui->comboBox_discipline->model()->index(ui->comboBox_discipline->currentIndex(),0).data().toInt();
}
