#include "prostavitocenki.h"
#include "ui_prostavitocenki.h"
#include <QMessageBox>
#include<QDebug>
ProstavitOcenki::ProstavitOcenki(QWidget *parent, int id, int rec_idDipl) :
    QDialog(parent),
    ui(new Ui::ProstavitOcenki)
{
    ui->setupUi(this);
    ui->lineEdit_ocenka->setValidator( new QIntValidator(1, 1000000, this));
    ui->lineEdit_ocenka->setPlaceholderText("Вводятся только цифры");

    ui->label_error->setVisible(false);
    ui->dateEdit->setDate(QDate::currentDate());
    this->stud_id = id;
    this->rec = rec_idDipl;
    qDebug()<<this->rec;

    dal_studentsControl = new Dal_studentsControl(this);
    QSqlQuery *query = new QSqlQuery;
    query = dal_studentsControl->getStudentNeotch(this->stud_id);
    ui->label_nameStud->setText(query->value(1).toString());
}

ProstavitOcenki::~ProstavitOcenki()
{
    delete ui;
}

void ProstavitOcenki::on_pushButton_save_clicked()
{
    bool error = false;
    if(ui->lineEdit_ocenka->text().isEmpty())
    {
        error = true;
    }

    if(error == true)
    {
        ui->label_error->setVisible(true);
        return;
    }
    else
    {
        if (! dal_main->checkConnection())
        {
            QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
            return;
        }
        if (dal_studentsControl->DiplomOcenkaUpdate(this->rec,
                                                    ui->lineEdit_ocenka->text(),
                                                    ui->dateEdit->date()))
        {
            this->close();
            QMessageBox::information(this, tr("Информация"), tr("Оценка успешно поставлена"));
        }
        else
        {
            this->close();
            QMessageBox::warning(this, tr("Ошибка"), tr("Оценка не была поставлена"));
        }
    }
}



void ProstavitOcenki::on_pushButton_cancel_clicked()
{
    this->close();
}
