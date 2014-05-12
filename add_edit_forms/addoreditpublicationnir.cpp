#include "addoreditpublicationnir.h"
#include "ui_addoreditpublicationnir.h"
#include <QMessageBox>
#include <QFileDialog>
#include<qfiledialog.h>
#include <QFile>
#include <QDebug>

AddOrEditPublicationNir::AddOrEditPublicationNir(QWidget *parent, int select, int id) :
    QDialog(parent),
    ui(new Ui::AddOrEditPublicationNir)
{
    ui->setupUi(this);
    ui->lineEdit_GodVipuska->setDate(QDate::currentDate());
    this->pub_nirs=0;
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    this->isEdit = false;
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    QSqlQueryModel *comboPub_nir = new QSqlQueryModel(this);
    comboPub_nir = dal_prepodcontrol->getComboPubNir();

    ui->comboBox_pubNir->setModel(comboPub_nir);
    ui->comboBox_pubNir->setModelColumn(2);
    ui->comboBox_pubNir->setCurrentIndex(-1);

    ui->label_error->setVisible(false);

    if (select == 1)
    {
        this->setWindowTitle(tr("Добавить новую запись"));
    }

    else
    {
        this->isEdit = true;
        this->setWindowTitle(tr("Редактирование записи"));
        this->rec_id = id;
        int ids_pub_id;
        QSqlQuery *query = new QSqlQuery;
        query = dal_prepodcontrol->getCurrentPublication(this->rec_id);
        ids_pub_id = query->value(1).toInt();

        comboPub_nir = dal_prepodcontrol->getComboPubNir();
        ui->comboBox_pubNir->setModel(comboPub_nir);
        ui->comboBox_pubNir->setModelColumn(2);

        ui->lineEdit_NameFile->setText(query->value(2).toString());

        this->UrlFail = query->value(3).toString();
        this->NameFails = query->value(2).toString();

        ui->lineEdit_mejduNarodStandartNomer->setText(query->value(4).toString());
        ui->lineEdit_NazvaniePublikacii->setText(query->value(5).toString());
        ui->lineEdit_NazvanieJurnala->setText(query->value(6).toString());
        ui->lineEdit_NomerVipuskaJurnaka->setText(query->value(7).toString());
        ui->lineEdit_GodVipuska->setDate(query->value(8).toDate());




        for (int j = 0; j < ui->comboBox_pubNir->count(); j ++)
        {
            if (comboPub_nir->index(j, 0).data().toInt() == ids_pub_id)
            {
                ui->comboBox_pubNir->setCurrentIndex(j);
                ui->comboBox_pubNir->view()->setCurrentIndex(comboPub_nir->index(j, 0));
                break;
            }
        }
    }
}

AddOrEditPublicationNir::~AddOrEditPublicationNir()
{
    delete ui;
}

void AddOrEditPublicationNir::on_pushButton_clicked()
{
    bool error = false;
    if (ui->comboBox_pubNir->currentIndex()==-1)
    {
        error = true;
    }

    if (ui->lineEdit_NameFile->text()=="")
    {
        error = true;
    }

    if (ui->lineEdit_mejduNarodStandartNomer->text()=="")
    {
        error = true;
    }

    if (ui->lineEdit_GodVipuska->text()=="")
    {
        error = true;
    }

    if (ui->lineEdit_NazvanieJurnala->text()=="")
    {
        error = true;
    }

    if (ui->lineEdit_NazvaniePublikacii->text()=="")
    {
        error = true;
    }

    if (ui->lineEdit_NomerVipuskaJurnaka->text()=="")
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
            QFileInfo fi(this->UrlFail);
            QString names = fi.fileName();
            QFile::copy(this->UrlFail, "C:\\Dropbox\\" + names);
            this->UrlFail = "C:\\Dropbox\\" + names;

            if (! dal_main->checkConnection())
            {
                QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
                return;
            }
            if (dal_prepodcontrol->PublicationNirAdd(ui->comboBox_pubNir->model()->index(ui->comboBox_pubNir->currentIndex(), 0).data().toInt(),\
                                                     this->NameFails,\
                                                     this->UrlFail,\
                                                     ui->lineEdit_mejduNarodStandartNomer->text(),\
                                                     ui->lineEdit_NazvaniePublikacii->text(),\
                                                     ui->lineEdit_NazvanieJurnala->text(),\
                                                     ui->lineEdit_NomerVipuskaJurnaka->text(),\
                                                     ui->lineEdit_GodVipuska->date()
                                                     ))
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
            QFileInfo fi(this->UrlFail);
            QString names = fi.fileName();
            QFile::copy(this->UrlFail, "C:\\Dropbox\\" + names);
            this->UrlFail = "C:\\Dropbox\\" + names;

            if (dal_prepodcontrol->editPublNir(this->rec_id,\
                                               ui->comboBox_pubNir->model()->index(ui->comboBox_pubNir->currentIndex(), 0).data().toInt(),\
                                               this->NameFails,\
                                               this->UrlFail,\
                                               ui->lineEdit_mejduNarodStandartNomer->text(),\
                                               ui->lineEdit_NazvaniePublikacii->text(),\
                                               ui->lineEdit_NazvanieJurnala->text(),\
                                               ui->lineEdit_NomerVipuskaJurnaka->text(),\
                                               ui->lineEdit_GodVipuska->date()
                                               ))
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

void AddOrEditPublicationNir::on_toolButton_clicked()
{
    this->NameFails = "";
    QString filename = QFileDialog::getOpenFileName(this,tr("open file"),"/home","All files(*.*)");
    filename = filename.replace("/" , "\\");

    this->UrlFail = filename;

    QString name =  QFileInfo(filename).baseName();
    ui->lineEdit_NameFile->setText(name);
    this->NameFails = name;

}

void AddOrEditPublicationNir::on_pushButton_2_clicked()
{
    this->close();
}
