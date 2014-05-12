#include "edit_poruchstatus.h"
#include "ui_edit_poruchstatus.h"

Edit_PoruchStatus::Edit_PoruchStatus(QWidget *parent, bool isEdit, int id_porucheniya) :
    QDialog(parent),
    id_porucheniya(id_porucheniya),
    isEdit(isEdit),
    ui(new Ui::Edit_PoruchStatus)
{
    ui->setupUi(this);
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    if(this->isEdit)
    {
        this->setWindowTitle("Редактирование записи");
        QSqlQuery *query = new QSqlQuery;
        query = dal_prepodcontrol->getCurrentPoruchenie(this->id_porucheniya);
        ui->label_ispolnitel->setText(query->value(9).toString());
        ui->lineEdit_title->setText(query->value(5).toString());
        ui->plainTextEdit_text->setPlainText(query->value(6).toString());
        ui->lineEdit_file->setText(query->value(7).toString());
        ui->dateEdit->setDate(query->value(11).toDate());
        ui->lineEdit_title->setReadOnly(true);
        ui->plainTextEdit_text->setReadOnly(true);
        ui->lineEdit_file->setReadOnly(true);
        ui->dateEdit->setReadOnly(true);
        if(query->value(4).toInt() == 1)
        {            
            ui->pushButton_openFile->setVisible(true);
            ui->pushButton_wait->setVisible(false);
            this->setWindowTitle("Новое поручение");
        }
        else if(query->value(4).toInt() == 2 || query->value(4).toInt() == 3)
        {
            ui->pushButton_otkl->setVisible(false);
            ui->pushButton_prinyat->setVisible(false);
            this->setWindowTitle("Поручение");
        }
        else if(query->value(4).toInt() == 4)
        {
            ui->pushButton_otkl->setVisible(false);
            ui->pushButton_prinyat->setVisible(false);
            ui->pushButton_wait->setVisible(false);
            this->setWindowTitle("Поручение ожидает проверки");
        }
        else
        {
            ui->pushButton_otkl->setVisible(false);
            ui->pushButton_prinyat->setVisible(false);
            ui->pushButton_wait->setVisible(false);
            this->setWindowTitle("Поручение выболнено");
        }
    }
}

Edit_PoruchStatus::~Edit_PoruchStatus()
{
    delete ui;
}

void Edit_PoruchStatus::on_pushButton_cancel_clicked()
{
    this->close();
}
void Edit_PoruchStatus::on_pushButton_prinyat_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    if (dal_prepodcontrol->setStatusPorucheniya(this->id_porucheniya, 2, false, true))
    {
        QMessageBox::information(this, tr("Информация"), tr("Поручение принято"));
    }
    else
    {
        QMessageBox::warning(this, tr("Ошибка"), tr("Поручение не принято"));
    }
    this->close();
}

void Edit_PoruchStatus::on_pushButton_otkl_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    if (dal_prepodcontrol->setStatusPorucheniya(this->id_porucheniya, 3, false, true))
    {
        QMessageBox::information(this, tr("Информация"), tr("Поручение отменено"));
    }
    else
    {
        QMessageBox::warning(this, tr("Ошибка"), tr("Поручение не отменено"));
    }
    this->close();
}

void Edit_PoruchStatus::on_pushButton_openFile_clicked()
{

}

void Edit_PoruchStatus::on_pushButton_wait_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    if (dal_prepodcontrol->setStatusPorucheniya(this->id_porucheniya, 4, false, true))
    {
        QMessageBox::information(this, tr("Информация"), tr("Поручение ожидает проверки"));
    }
    else
    {
        QMessageBox::warning(this, tr("Ошибка"), tr("Стутус поручения не сменен"));
    }
    this->close();
}
