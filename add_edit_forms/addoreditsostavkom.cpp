#include "addoreditsostavkom.h"
#include "ui_addoreditsostavkom.h"
#include<QMessageBox>

AddOrEditSostavKom::AddOrEditSostavKom(QWidget *parent, int select, int id, int viewsv) :
    QDialog(parent),
    ui(new Ui::AddOrEditSostavKom)
{
    ui->setupUi(this);
    ui->lineEdit_nomRaport->setValidator( new QIntValidator(1, 1000000, this));
    ui->lineEdit_nomRaport->setPlaceholderText("Вводятся только цифры");
    this->preds=0;
    this->secretar=0;
    this->chl1=0;
    this->chl2=0;
    this->chl3=0;
    this->chl4=0;
    this->chl5=0;
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    this->isEdit = false;
    ui->dateEdit->setDate(QDate::currentDate());
    ui->label_error->setVisible(false);
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    ui->comboBox_Predsed->setModel(dal_prepodcontrol->getUchastnikiKom(this->preds, this->secretar, this->chl1, this->chl2, this->chl3, this->chl4, this->chl5));
    ui->comboBox_Predsed->setModelColumn(1);
    ui->comboBox_Predsed->setCurrentIndex(-1);

    ui->comboBox_Secr->setModel(dal_prepodcontrol->getUchastnikiKom(this->preds, this->secretar, this->chl1, this->chl2, this->chl3, this->chl4, this->chl5));
    ui->comboBox_Secr->setModelColumn(1);
    ui->comboBox_Secr->setCurrentIndex(-1);

    ui->comboBox_Ch1->setModel(dal_prepodcontrol->getUchastnikiKom(this->preds, this->secretar, this->chl1, this->chl2, this->chl3, this->chl4, this->chl5));
    ui->comboBox_Ch1->setModelColumn(1);
    ui->comboBox_Ch1->setCurrentIndex(-1);

    ui->comboBox_Ch2->setModel(dal_prepodcontrol->getUchastnikiKom(this->preds, this->secretar, this->chl1, this->chl2, this->chl3, this->chl4, this->chl5));
    ui->comboBox_Ch2->setModelColumn(1);
    ui->comboBox_Ch2->setCurrentIndex(-1);

    ui->comboBox_Ch3->setModel(dal_prepodcontrol->getUchastnikiKom(this->preds, this->secretar, this->chl1, this->chl2, this->chl3, this->chl4, this->chl5));
    ui->comboBox_Ch3->setModelColumn(1);
    ui->comboBox_Ch3->setCurrentIndex(-1);

    ui->comboBox_Ch4->setModel(dal_prepodcontrol->getUchastnikiKom(this->preds, this->secretar, this->chl1, this->chl2, this->chl3, this->chl4, this->chl5));
    ui->comboBox_Ch4->setModelColumn(1);
    ui->comboBox_Ch4->setCurrentIndex(-1);

    ui->comboBox_Ch5->setModel(dal_prepodcontrol->getUchastnikiKom(this->preds, this->secretar, this->chl1, this->chl2, this->chl3, this->chl4, this->chl5));
    ui->comboBox_Ch5->setModelColumn(1);
    ui->comboBox_Ch5->setCurrentIndex(-1);

    css = new Styles;

    if (select == 1)
    {
        this->setWindowTitle(tr("Добавить новую запись"));
    }
    else
    {
        this->isEdit = true;
        this->setWindowTitle(tr("Редактирование записи"));

        this->rec_id = id;
        QSqlQuery *query = new QSqlQuery;
        query = dal_prepodcontrol->getCurrentSostavGAK(this->rec_id);
        this->preds = query->value(17).toInt();
        ui->comboBox_Predsed->setCurrentText(query->value(1).toString());
        this->secretar = query->value(18).toInt();
        ui->comboBox_Secr->setCurrentText(query->value(3).toString());
        this->chl1 = query->value(19).toInt();
        ui->comboBox_Ch1->setCurrentText(query->value(5).toString());
        this->chl2 = query->value(20).toInt();
        ui->comboBox_Ch2->setCurrentText(query->value(7).toString());
        this->chl3 = query->value(21).toInt();
        ui->comboBox_Ch3->setCurrentText(query->value(9).toString());
        this->chl4 = query->value(22).toInt();
        ui->comboBox_Ch4->setCurrentText(query->value(11).toString());
        this->chl5 = query->value(23).toInt();
        ui->comboBox_Ch5->setCurrentText(query->value(13).toString());
        ui->dateEdit->setDate(query->value(15).toDate());
        ui->lineEdit_nomRaport->setText(query->value(16).toString());
    }
}

AddOrEditSostavKom::~AddOrEditSostavKom()
{
    delete ui;
}

void AddOrEditSostavKom::on_comboBox_Predsed_activated(int index)
{
    this->preds = ui->comboBox_Predsed->model()->index(ui->comboBox_Predsed->currentIndex(), 0).data().toInt();
    ui->comboBox_Predsed->setStyleSheet(css->pressedStyleSheet);
}

void AddOrEditSostavKom::on_comboBox_Secr_activated(int index)
{
    this->secretar = ui->comboBox_Secr->model()->index(ui->comboBox_Secr->currentIndex(), 0).data().toInt();
    ui->comboBox_Secr->setStyleSheet(css->pressedStyleSheet);
}

void AddOrEditSostavKom::on_comboBox_Ch1_activated(int index)
{
    this->chl1 = ui->comboBox_Ch1->model()->index(ui->comboBox_Ch1->currentIndex(), 0).data().toInt();
    ui->comboBox_Ch1->setStyleSheet(css->pressedStyleSheet);
}

void AddOrEditSostavKom::on_comboBox_Ch2_activated(int index)
{
    this->chl2 = ui->comboBox_Ch2->model()->index(ui->comboBox_Ch2->currentIndex(), 0).data().toInt();
    ui->comboBox_Ch2->setStyleSheet(css->pressedStyleSheet);

}

void AddOrEditSostavKom::on_comboBox_Ch3_activated(int index)
{
    this->chl3 = ui->comboBox_Ch3->model()->index(ui->comboBox_Ch3->currentIndex(), 0).data().toInt();
    ui->comboBox_Ch3->setStyleSheet(css->pressedStyleSheet);
}

void AddOrEditSostavKom::on_comboBox_Ch4_activated(int index)
{
    this->chl4 = ui->comboBox_Ch4->model()->index(ui->comboBox_Ch4->currentIndex(), 0).data().toInt();
    ui->comboBox_Ch4->setStyleSheet(css->pressedStyleSheet);
}

void AddOrEditSostavKom::on_comboBox_Ch5_activated(int index)
{
    this->chl5 = ui->comboBox_Ch5->model()->index(ui->comboBox_Ch5->currentIndex(), 0).data().toInt();
    ui->comboBox_Ch5->setStyleSheet(css->pressedStyleSheet);
}

void AddOrEditSostavKom::on_pushButton_begin_clicked()
{
    this->preds = 0;
    this->secretar = 0;
    this->chl1 = 0;
    this->chl2 = 0;
    this->chl3 = 0;
    this->chl4 = 0;
    this->chl5 = 0;
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    ui->comboBox_Predsed->setModel(dal_prepodcontrol->getUchastnikiKom(this->preds, this->secretar, this->chl1, this->chl2, this->chl3, this->chl4, this->chl5));
    ui->comboBox_Predsed->setModelColumn(1);
    ui->comboBox_Predsed->setCurrentIndex(-1);

    ui->comboBox_Secr->setModel(dal_prepodcontrol->getUchastnikiKom(this->preds, this->secretar, this->chl1, this->chl2, this->chl3, this->chl4, this->chl5));
    ui->comboBox_Secr->setModelColumn(1);
    ui->comboBox_Secr->setCurrentIndex(-1);

    ui->comboBox_Ch1->setModel(dal_prepodcontrol->getUchastnikiKom(this->preds, this->secretar, this->chl1, this->chl2, this->chl3, this->chl4, this->chl5));
    ui->comboBox_Ch1->setModelColumn(1);
    ui->comboBox_Ch1->setCurrentIndex(-1);

    ui->comboBox_Ch2->setModel(dal_prepodcontrol->getUchastnikiKom(this->preds, this->secretar, this->chl1, this->chl2, this->chl3, this->chl4, this->chl5));
    ui->comboBox_Ch2->setModelColumn(1);
    ui->comboBox_Ch2->setCurrentIndex(-1);

    ui->comboBox_Ch3->setModel(dal_prepodcontrol->getUchastnikiKom(this->preds, this->secretar, this->chl1, this->chl2, this->chl3, this->chl4, this->chl5));
    ui->comboBox_Ch3->setModelColumn(1);
    ui->comboBox_Ch3->setCurrentIndex(-1);

    ui->comboBox_Ch4->setModel(dal_prepodcontrol->getUchastnikiKom(this->preds, this->secretar, this->chl1, this->chl2, this->chl3, this->chl4, this->chl5));
    ui->comboBox_Ch4->setModelColumn(1);
    ui->comboBox_Ch4->setCurrentIndex(-1);

    ui->comboBox_Ch5->setModel(dal_prepodcontrol->getUchastnikiKom(this->preds, this->secretar, this->chl1, this->chl2, this->chl3, this->chl4, this->chl5));
    ui->comboBox_Ch5->setModelColumn(1);
    ui->comboBox_Ch5->setCurrentIndex(-1);

    ui->comboBox_Predsed->setEnabled(true);
    ui->comboBox_Secr->setEnabled(false);
    ui->comboBox_Ch1->setEnabled(false);
    ui->comboBox_Ch2->setEnabled(false);
    ui->comboBox_Ch3->setEnabled(false);
    ui->comboBox_Ch4->setEnabled(false);
    ui->comboBox_Ch5->setEnabled(false);

}

void AddOrEditSostavKom::on_pushButton_ok_clicked()
{
    bool error = false;
    bool errorPovtor = false;
    if (ui->comboBox_Predsed->currentIndex()==-1)
    {
        error = true;
    }

    if (ui->comboBox_Secr->currentIndex()==-1)
    {
        error = true;
    }


    if(ui->comboBox_Ch1->currentIndex()==-1)
    {
        error = true;
    }

    if(ui->comboBox_Ch2->currentIndex()==-1)
    {
        error = true;
    }


    if(ui->comboBox_Ch3->currentIndex()==-1)
    {
        error = true;
    }


    if(ui->comboBox_Ch4->currentIndex()==-1)
    {
        error = true;
    }


    if(ui->comboBox_Ch5->currentIndex()==-1)
    {
        error = true;
    }

    if(this->preds == this->secretar)
    {
        errorPovtor = true;
        ui->comboBox_Predsed->setStyleSheet(css->wrongStyleSheet2);
        ui->comboBox_Secr->setStyleSheet(css->wrongStyleSheet2);
    }

    if(this->preds == this->chl1)
    {
        errorPovtor = true;
        ui->comboBox_Predsed->setStyleSheet(css->wrongStyleSheet2);
        ui->comboBox_Ch1->setStyleSheet(css->wrongStyleSheet2);
    }

    if(this->preds == this->chl2)
    {
        errorPovtor = true;
        ui->comboBox_Predsed->setStyleSheet(css->wrongStyleSheet2);
        ui->comboBox_Ch2->setStyleSheet(css->wrongStyleSheet2);
    }

    if(this->preds == this->chl3)
    {
        errorPovtor = true;
        ui->comboBox_Predsed->setStyleSheet(css->wrongStyleSheet2);
        ui->comboBox_Ch3->setStyleSheet(css->wrongStyleSheet2);
    }

    if(this->preds == this->chl4)
    {
        errorPovtor = true;
        ui->comboBox_Predsed->setStyleSheet(css->wrongStyleSheet2);
        ui->comboBox_Ch4->setStyleSheet(css->wrongStyleSheet2);
    }

    if(this->preds == this->chl5)
    {
        errorPovtor = true;
        ui->comboBox_Predsed->setStyleSheet(css->wrongStyleSheet2);
        ui->comboBox_Ch5->setStyleSheet(css->wrongStyleSheet2);
    }

    if(this->secretar == this->chl1)
    {
        errorPovtor = true;
        ui->comboBox_Secr->setStyleSheet(css->wrongStyleSheet2);
        ui->comboBox_Ch1->setStyleSheet(css->wrongStyleSheet2);
    }

    if(this->secretar == this->chl2)
    {
        errorPovtor = true;
        ui->comboBox_Secr->setStyleSheet(css->wrongStyleSheet2);
        ui->comboBox_Ch2->setStyleSheet(css->wrongStyleSheet2);
    }

    if(this->secretar == this->chl3)
    {
        errorPovtor = true;
        ui->comboBox_Secr->setStyleSheet(css->wrongStyleSheet2);
        ui->comboBox_Ch3->setStyleSheet(css->wrongStyleSheet2);
    }

    if(this->secretar == this->chl4)
    {
        errorPovtor = true;
        ui->comboBox_Secr->setStyleSheet(css->wrongStyleSheet2);
        ui->comboBox_Ch4->setStyleSheet(css->wrongStyleSheet2);
    }

    if(this->secretar == this->chl5)
    {
        errorPovtor = true;
        ui->comboBox_Secr->setStyleSheet(css->wrongStyleSheet2);
        ui->comboBox_Ch5->setStyleSheet(css->wrongStyleSheet2);
    }

    if(this->chl1 == this->chl2)
    {
        errorPovtor = true;
        ui->comboBox_Ch1->setStyleSheet(css->wrongStyleSheet2);
        ui->comboBox_Ch2->setStyleSheet(css->wrongStyleSheet2);
    }

    if(this->chl1 == this->chl3)
    {
        errorPovtor = true;
        ui->comboBox_Ch1->setStyleSheet(css->wrongStyleSheet2);
        ui->comboBox_Ch3->setStyleSheet(css->wrongStyleSheet2);
    }

    if(this->chl1 == this->chl4)
    {
        errorPovtor = true;
        ui->comboBox_Ch1->setStyleSheet(css->wrongStyleSheet2);
        ui->comboBox_Ch4->setStyleSheet(css->wrongStyleSheet2);
    }

    if(this->chl1 == this->chl5)
    {
        errorPovtor = true;
        ui->comboBox_Ch1->setStyleSheet(css->wrongStyleSheet2);
        ui->comboBox_Ch5->setStyleSheet(css->wrongStyleSheet2);
    }

    if(this->chl2 == this->chl3)
    {
        errorPovtor = true;
        ui->comboBox_Ch2->setStyleSheet(css->wrongStyleSheet2);
        ui->comboBox_Ch3->setStyleSheet(css->wrongStyleSheet2);
    }

    if(this->chl2 == this->chl4)
    {
        errorPovtor = true;
        ui->comboBox_Ch2->setStyleSheet(css->wrongStyleSheet2);
        ui->comboBox_Ch4->setStyleSheet(css->wrongStyleSheet2);
    }

    if(this->chl2 == this->chl5)
    {
        errorPovtor = true;
        ui->comboBox_Ch2->setStyleSheet(css->wrongStyleSheet2);
        ui->comboBox_Ch5->setStyleSheet(css->wrongStyleSheet2);
    }

    if(this->chl3 == this->chl4)
    {
        errorPovtor = true;
        ui->comboBox_Ch3->setStyleSheet(css->wrongStyleSheet2);
        ui->comboBox_Ch4->setStyleSheet(css->wrongStyleSheet2);
    }

    if(this->chl3 == this->chl5)
    {
        errorPovtor = true;
        ui->comboBox_Ch3->setStyleSheet(css->wrongStyleSheet2);
        ui->comboBox_Ch5->setStyleSheet(css->wrongStyleSheet2);
    }


    if(this->chl4 == this->chl5)
    {
        errorPovtor = true;
        ui->comboBox_Ch4->setStyleSheet(css->wrongStyleSheet2);
        ui->comboBox_Ch5->setStyleSheet(css->wrongStyleSheet2);
    }


    if (errorPovtor==true)
    {
        QMessageBox::critical(this, tr("Ошибка"), tr("Председатель, секретарь и члены комиссии не должны повторяться"));
        return;
    }

    if(ui->lineEdit_nomRaport->text()=="")
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
            if (dal_prepodcontrol->sostavKomAdd(this->preds,\
                                                this->secretar,\
                                                this->chl1,\
                                                this->chl2,\
                                                this->chl3,\
                                                this->chl4,\
                                                this->chl5,\
                                                ui->dateEdit->date(),\
                                                ui->lineEdit_nomRaport->text().toInt()))
            {               
                QMessageBox::information(this, tr("Информация"), tr("Запись успешно добавлена"));
                this->close();
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
            if (dal_prepodcontrol->sostavKomEdit(this->rec_id,\
                                                 this->preds,\
                                                 this->secretar,\
                                                 this->chl1,\
                                                 this->chl2,\
                                                 this->chl3,\
                                                 this->chl4,\
                                                 this->chl5,\
                                                 ui->dateEdit->date(),\
                                                 ui->lineEdit_nomRaport->text().toInt()))
            {
                QMessageBox::information(this, tr("Информация"), tr("Данные успешно отредактированы"));
                this->close();                
            }
            else
            {
                QMessageBox::information(this, tr("Информация"), tr("Данные не были изменены"));
            }
        }
    }
}

void AddOrEditSostavKom::on_pushButtonCancel_clicked()
{
    this->close();
}
