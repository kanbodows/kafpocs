#include "accessdb.h"
#include "ui_accessdb.h"
#include "QNetworkProxy"
//#include <QSettings>
accessdb::accessdb(QWidget *parent)
   : QDialog(parent),
    ui(new Ui::accessdb)
{
    ui->setupUi(this);
//    QNetworkProxy proxy;
//         proxy.setType(QNetworkProxy::HttpProxy);
//         proxy.setHostName("192.168.9.254");
//         proxy.setPort(4236);
//         QNetworkProxy::setApplicationProxy(proxy);

    this->marker = 0;
    css = new Styles();
}

accessdb::~accessdb()
{
    delete ui;
}



void accessdb::on_pushButton_ok_clicked()
{
    if(ui->lineEdit_dbName->text()=="" || ui->lineEdit_port->text()=="" || ui->lineEdit_host->text()==""\
            || /*ui->lineEdit_password->text()=="" ||*/ ui->lineEdit_userName->text()=="")
    {
        on_lineEdit_dbName_cursorPositionChanged();
        on_lineEdit_password_cursorPositionChanged();
        on_lineEdit_host_cursorPositionChanged();
        on_lineEdit_port_cursorPositionChanged();
        on_lineEdit_userName_cursorPositionChanged();
         QMessageBox::warning(this, tr("Ошибка"), tr("Заполните все поля"));
    }
    else
    {
    QString dbname = ui->lineEdit_dbName->text();
    int port = ui->lineEdit_port->text().toInt();
    QString host = ui->lineEdit_host->text();
    QString username = ui->lineEdit_userName->text();
    QString password = ui->lineEdit_password->text();
    if (ui->checkBox_saveSettings->isChecked())
    {
        QSettings settings("team2", "pocskaf2");
        settings.setValue("databaseName", dbname);
        settings.setValue("port", port);
        settings.setValue("host", host);
        settings.setValue("username", username);
        settings.setValue("password", password);

    }

    if (dal_main->setConnection(dbname, port, host, username, password))
    {
        QMessageBox::information(this, tr("Подключение успешно"), tr("Соединение успешно установлено"));
        this->marker =0;
        this->close();
    }
    else
    {
        QMessageBox::information(this, tr("Ошибка подключения"), tr("Соединение не может быть установлено"));
        return;
    }
    }
}

void accessdb::on_pushButton_cancel_clicked()
{
    this->marker = 1;
    this->close();
}

void accessdb::on_lineEdit_dbName_cursorPositionChanged()
{
    if(ui->lineEdit_dbName->text()=="")
    {
        ui->lineEdit_dbName->setStyleSheet(css->wrongStyleSheet);
    }
    else
    {
        ui->lineEdit_dbName->setStyleSheet(css->pressedStyleSheet);
    }
}

void accessdb::on_lineEdit_port_cursorPositionChanged()
{
    if(ui->lineEdit_port->text()=="")
    {
        ui->lineEdit_port->setStyleSheet(css->wrongStyleSheet);
    }
    else
    {
        ui->lineEdit_port->setStyleSheet(css->pressedStyleSheet);
    }
}

void accessdb::on_lineEdit_host_cursorPositionChanged()
{
    if(ui->lineEdit_host->text()=="")
    {
        ui->lineEdit_host->setStyleSheet(css->wrongStyleSheet);
    }
    else
    {
        ui->lineEdit_host->setStyleSheet(css->pressedStyleSheet);
    }
}

void accessdb::on_lineEdit_userName_cursorPositionChanged()
{
    if(ui->lineEdit_userName->text()=="")
    {
        ui->lineEdit_userName->setStyleSheet(css->wrongStyleSheet);
    }
    else
    {
        ui->lineEdit_userName->setStyleSheet(css->pressedStyleSheet);
    }
}

void accessdb::on_lineEdit_password_cursorPositionChanged()
{
    if(ui->lineEdit_password->text()=="")
    {
        ui->lineEdit_password->setStyleSheet(css->wrongStyleSheet);
    }
    else
    {
        ui->lineEdit_password->setStyleSheet(css->pressedStyleSheet);
    }
}

void accessdb::closeEvent(QCloseEvent *e)
{
    if (this->marker == 1)
        exit(1);
}
