#include "addoreditgak.h"
#include "ui_addoreditgak.h"
#include<QMessageBox>

AddOrEditGAK::AddOrEditGAK(QWidget *parent, int select, int id) :
    QDialog(parent),
    ui(new Ui::AddOrEditGAK)
{
    ui->setupUi(this);
    ui->lineEditNomPrikaza->setValidator( new QIntValidator(1, 1000000, this));
    ui->lineEditNomPrikaza->setPlaceholderText("Вводятся только цифры");

    this->discp1=0;
    this->discp2=0;
    this->discp3=0;
    this->discp4=0;
    this->nomRaporta = 0;
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    this->isEdit = false;
    ui->dateEdit->setDate(QDate::currentDate());
    ui->label_error->setVisible(false);
    dal_studentsControl = new Dal_studentsControl(this);

    QSqlQueryModel *comboDiscpl1 = new QSqlQueryModel(this);
    comboDiscpl1 = dal_studentsControl->getDiscipl();
    ui->comboBoxd1->setModel(comboDiscpl1);
    ui->comboBoxd1->setModelColumn(1);
    ui->comboBoxd1->setCurrentIndex(-1);

    QSqlQueryModel *comboDiscpl2 = new QSqlQueryModel(this);
    comboDiscpl2 = dal_studentsControl->getDiscipl();
    ui->comboBoxd2->setModel(comboDiscpl2);
    ui->comboBoxd2->setModelColumn(1);
    ui->comboBoxd2->setCurrentIndex(-1);

    QSqlQueryModel *comboDiscpl3 = new QSqlQueryModel(this);
    comboDiscpl3 = dal_studentsControl->getDiscipl();
    ui->comboBoxd3->setModel(comboDiscpl3);
    ui->comboBoxd3->setModelColumn(1);
    ui->comboBoxd3->setCurrentIndex(-1);

    QSqlQueryModel *comboDiscpl4 = new QSqlQueryModel(this);
    comboDiscpl4 = dal_studentsControl->getDiscipl();
    ui->comboBoxd4->setModel(comboDiscpl4);
    ui->comboBoxd4->setModelColumn(1);
    ui->comboBoxd4->setCurrentIndex(-1);

    QSqlQueryModel *comboFormKomis = new QSqlQueryModel(this);
    comboFormKomis = dal_studentsControl->getNomRapFromSostavGAK();
    ui->lineEdit_GodFormir->setModel(comboFormKomis);
    ui->lineEdit_GodFormir->setModelColumn(1);
    ui->lineEdit_GodFormir->setCurrentIndex(-1);

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
        int ids_disc1;
        int ids_disc2;
        int ids_disc3;
        int ids_disc4;
        int ids_FormKomis;

        QSqlQuery *query = new QSqlQuery;
        query = dal_studentsControl->getCurrentGAK(this->rec_id);

        ids_disc1 = query->value(2).toInt();
        for (int j = 0; j < ui->comboBoxd1->count(); j++)
        {
            if (comboDiscpl1->index(j, 0).data().toInt() == ids_disc1)
            {
                ui->comboBoxd1->setCurrentIndex(j);
                ui->comboBoxd1->view()->setCurrentIndex(comboDiscpl1->index(j, 0));
                break;
            }
        }

        ids_disc2 = query->value(3).toInt();
        for (int j = 0; j < ui->comboBoxd2->count(); j++)
        {
            if (comboDiscpl2->index(j, 0).data().toInt() == ids_disc2)
            {
                ui->comboBoxd2->setCurrentIndex(j);
                ui->comboBoxd2->view()->setCurrentIndex(comboDiscpl2->index(j, 0));
                break;
            }
        }

        ids_disc3 = query->value(4).toInt();
        for (int j = 0; j < ui->comboBoxd3->count(); j++)
        {
            if (comboDiscpl3->index(j, 0).data().toInt() == ids_disc3)
            {
                ui->comboBoxd3->setCurrentIndex(j);
                ui->comboBoxd3->view()->setCurrentIndex(comboDiscpl3->index(j, 0));
                break;
            }
        }

        ids_disc4 = query->value(5).toInt();
        for (int j = 0; j < ui->comboBoxd4->count(); j++)
        {
            if (comboDiscpl4->index(j, 0).data().toInt() == ids_disc4)
            {
                ui->comboBoxd4->setCurrentIndex(j);
                ui->comboBoxd4->view()->setCurrentIndex(comboDiscpl4->index(j, 0));
                break;
            }
        }


        ids_FormKomis = query->value(1).toInt();
        for (int j = 0; j < ui->lineEdit_GodFormir->count(); j++)
        {
            if (comboFormKomis->index(j, 0).data().toInt() == ids_FormKomis)
            {
                ui->lineEdit_GodFormir->setCurrentIndex(j);
                ui->lineEdit_GodFormir->view()->setCurrentIndex(comboFormKomis->index(j, 0));
                break;
            }
        }




        this->discp1 = ids_disc1;
        this->discp2 = ids_disc2;
        this->discp3 = ids_disc3;
        this->discp4 = ids_disc4;
        this->nomRaporta = ids_FormKomis;


        ui->dateEdit->setDate(query->value(10).toDate());
        ui->lineEditKabinet->setText(query->value(11).toString());
        ui->lineEditNomPrikaza->setText(query->value(12).toString());
    }
}

AddOrEditGAK::~AddOrEditGAK()
{
    delete ui;
}

void AddOrEditGAK::on_comboBoxd1_activated(int index)
{
    this->discp1 = ui->comboBoxd1->model()->index(ui->comboBoxd1->currentIndex(), 0).data().toInt();
    ui->comboBoxd1->setStyleSheet(css->pressedStyleSheet);
}

void AddOrEditGAK::on_comboBoxd2_activated(int index)
{
    this->discp2 = ui->comboBoxd2->model()->index(ui->comboBoxd2->currentIndex(), 0).data().toInt();
    ui->comboBoxd2->setStyleSheet(css->pressedStyleSheet);
}

void AddOrEditGAK::on_comboBoxd3_activated(int index)
{
    this->discp3 = ui->comboBoxd3->model()->index(ui->comboBoxd3->currentIndex(), 0).data().toInt();
    ui->comboBoxd3->setStyleSheet(css->pressedStyleSheet);
}

void AddOrEditGAK::on_comboBoxd4_activated(int index)
{
    this->discp4 = ui->comboBoxd4->model()->index(ui->comboBoxd4->currentIndex(), 0).data().toInt();
    ui->comboBoxd4->setStyleSheet(css->pressedStyleSheet);
}

void AddOrEditGAK::on_pushButton_clicked()
{
    bool error = false;
    bool errorPovtor = false;
    if (ui->comboBoxd1->currentIndex()==-1)
    {
        error = true;
    }

    if (ui->comboBoxd2->currentIndex()==-1)
    {
        error = true;
    }


    if(ui->comboBoxd3->currentIndex()==-1)
    {
        error = true;
    }

    if(ui->comboBoxd4->currentIndex()==-1)
    {
        error = true;
    }

    if(ui->lineEdit_GodFormir->currentIndex()==-1)
    {
        error = true;
    }

    if(this->discp1 == this->discp2)
    {
        errorPovtor = true;
        ui->comboBoxd1->setStyleSheet(css->wrongStyleSheet2);
        ui->comboBoxd2->setStyleSheet(css->wrongStyleSheet2);
    }

    if(this->discp1 == this->discp3)
    {
        errorPovtor = true;
        ui->comboBoxd1->setStyleSheet(css->wrongStyleSheet2);
        ui->comboBoxd3->setStyleSheet(css->wrongStyleSheet2);
    }

    if(this->discp1 == this->discp4)
    {
        errorPovtor = true;
        ui->comboBoxd1->setStyleSheet(css->wrongStyleSheet2);
        ui->comboBoxd4->setStyleSheet(css->wrongStyleSheet2);
    }


    if(this->discp2 == this->discp3)
    {
        errorPovtor = true;
        ui->comboBoxd2->setStyleSheet(css->wrongStyleSheet2);
        ui->comboBoxd3->setStyleSheet(css->wrongStyleSheet2);
    }

    if(this->discp2 == this->discp4)
    {
        errorPovtor = true;
        ui->comboBoxd2->setStyleSheet(css->wrongStyleSheet2);
        ui->comboBoxd4->setStyleSheet(css->wrongStyleSheet2);
    }


    if(this->discp3 == this->discp4)
    {
        errorPovtor = true;
        ui->comboBoxd3->setStyleSheet(css->wrongStyleSheet2);
        ui->comboBoxd4->setStyleSheet(css->wrongStyleSheet2);
    }

    if(ui->lineEditKabinet->text()=="")
    {
        error = true;
    }
    if(ui->lineEditNomPrikaza->text()=="")
    {
        error = true;
    }

    if (errorPovtor==true)
    {
        QMessageBox::critical(this, tr("Ошибка"), tr("Дисциплины не должны повторяться"));
        return;
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
            if (dal_studentsControl->GAKAdd(this->discp1,\
                                            this->discp2,\
                                            this->discp3,\
                                            this->discp4,\
                                            ui->dateEdit->date(),\
                                            ui->lineEditKabinet->text(),\
                                            ui->lineEditNomPrikaza->text().toInt(),\
                                            this->nomRaporta))
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
            if (dal_studentsControl->GAKsEdit(this->rec_id,\
                                              this->discp1,\
                                              this->discp2,\
                                              this->discp3,\
                                              this->discp4,\
                                              ui->dateEdit->date(),\
                                              ui->lineEditKabinet->text(),\
                                              ui->lineEditNomPrikaza->text().toInt(),\
                                              this->nomRaporta))
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

void AddOrEditGAK::on_pushButton_2_clicked()
{
    this->close();
}

void AddOrEditGAK::on_lineEdit_GodFormir_activated(int index)
{
    this->nomRaporta = ui->lineEdit_GodFormir->model()->index(ui->lineEdit_GodFormir->currentIndex(), 0).data().toInt();
    ui->lineEdit_GodFormir->setStyleSheet(css->pressedStyleSheet);
}
