#include "smenaparrolya.h"
#include "ui_smenaparrolya.h"
#include<QDebug>
#include<QMessageBox>

SmenaParrolya::SmenaParrolya(QWidget *parent, int rec) :
    QDialog(parent),
    ui(new Ui::SmenaParrolya)
{
    ui->setupUi(this);
    css = new Styles();
    ui->label_error->setVisible(false);
    this->record_id = rec;
    ui->labelStpa->setVisible(false);
    ui->lineEdit_StarProvParol->setVisible(false);
    QSqlQuery *query = new QSqlQuery;
    query = dal_prepodcontrol->getCurrentSotrudnik(rec);
    this->setWindowTitle("Изменение пароля");
    ui->label_fioSotr->setText("ФИО сотрудника: " + query->value(1).toString());
    ui->lineEdit_StarProvParol->setText(query->value(13).toString());
}

SmenaParrolya::~SmenaParrolya()
{
    delete ui;
}

void SmenaParrolya::on_pushButton_ok_clicked()
{
    bool error = false;
    ui->label_error->setVisible(false);
    if (ui->lineEdit_StarParol->text()=="")
    {
        error = true;
        ui->lineEdit_StarParol->setStyleSheet(css->wrongStyleSheet2);
    }

    if (ui->lineEdit_VvodNovogoParol->text()=="")
    {
        error = true;
        ui->lineEdit_VvodNovogoParol->setStyleSheet(css->wrongStyleSheet2);
    }

    if (ui->lineEdit_PodtverParpl->text()=="")
    {
        error = true;
        ui->lineEdit_PodtverParpl->setStyleSheet(css->wrongStyleSheet2);
    }

    if (error==true)
    {
        ui->label_error->setVisible(true);
        return;
    }
    else
    {
        if(ui->lineEdit_StarParol->text() == ui->lineEdit_StarProvParol->text())
        {
            ui->lineEdit_StarParol->setStyleSheet(css->pressedStyleSheet);
            if(ui->lineEdit_VvodNovogoParol->text() == ui->lineEdit_PodtverParpl->text())
            {
                ui->lineEdit_PodtverParpl->setStyleSheet(css->pressedStyleSheet);
                ui->lineEdit_VvodNovogoParol->setStyleSheet(css->pressedStyleSheet);
                if (dal_prepodcontrol->editParolSotr(this->record_id, ui->lineEdit_VvodNovogoParol->text()))
                {
                    this->close();
                    QMessageBox::information(this, tr("Информация"), tr("Пароль успешно изменен"));
                }
                else
                {
                    this->close();
                    QMessageBox::warning(this, tr("Информация"), tr("Пароль не изменен"));
                }
            }
            else
            {
                QMessageBox::information(this, tr("Информация"), tr("Пароли не совпадают"));
                ui->lineEdit_PodtverParpl->setStyleSheet(css->wrongStyleSheet2);
                ui->lineEdit_VvodNovogoParol->setStyleSheet(css->wrongStyleSheet2);
            }

        }
        else
        {
            QMessageBox::information(this, tr("Информация"), tr("Старый пароль введен не верно"));
            ui->lineEdit_StarParol->setStyleSheet(css->wrongStyleSheet2);
        }
    }

}

void SmenaParrolya::on_lineEdit_StarParol_editingFinished()
{
    if (ui->lineEdit_StarParol->text()=="")
    {
        ui->lineEdit_StarParol->setStyleSheet(css->wrongStyleSheet2);
    }
    else
        ui->lineEdit_StarParol->setStyleSheet(css->pressedStyleSheet);
}

void SmenaParrolya::on_lineEdit_VvodNovogoParol_editingFinished()
{

    if (ui->lineEdit_VvodNovogoParol->text()=="")
    {
        ui->lineEdit_VvodNovogoParol->setStyleSheet(css->wrongStyleSheet2);
    }

    else
        ui->lineEdit_VvodNovogoParol->setStyleSheet(css->pressedStyleSheet);
}

void SmenaParrolya::on_lineEdit_PodtverParpl_editingFinished()
{
    if (ui->lineEdit_PodtverParpl->text()=="")
    {
        ui->lineEdit_PodtverParpl->setStyleSheet(css->wrongStyleSheet2);
    }
    else
        ui->lineEdit_PodtverParpl->setStyleSheet(css->pressedStyleSheet);
}

void SmenaParrolya::on_pushButton_cancel_clicked()
{
   this->close();
}
