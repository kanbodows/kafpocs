#include "otpravkasoobsheniasotrudnikam.h"
#include "ui_otpravkasoobsheniasotrudnikam.h"
#include "dals/dal_studentscontrol.h"
#include <QMessageBox>
bool OtpravkaSoobsheniaSotrudnikam::isOpen = false;
OtpravkaSoobsheniaSotrudnikam::OtpravkaSoobsheniaSotrudnikam(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OtpravkaSoobsheniaSotrudnikam)
{
    ui->setupUi(this);
    if (! isOpen)
    {
        isOpen = true;
    }
    else
    {
        QMessageBox::warning(this, tr("Ошибка"), tr("Окно уже открыто"));
        throw std::exception();
    }
    this->mess="";
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    dal_studentsControl = new Dal_studentsControl(this);
    SotrKafModels = dal_studentsControl->SendMessageSotrKaf();

    ui->tableView_SotrKafOtpravkaSoobsh->setModel(SotrKafModels);
    ui->tableView_SotrKafOtpravkaSoobsh->setColumnHidden(0,true);
    ui->tableView_SotrKafOtpravkaSoobsh->setColumnHidden(2,true);
    ui->tableView_SotrKafOtpravkaSoobsh->setColumnHidden(3,true);
    ui->tableView_SotrKafOtpravkaSoobsh->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_SotrKafOtpravkaSoobsh->horizontalHeader()->setStretchLastSection(true);
    ui->label_error->setVisible(false);
}

OtpravkaSoobsheniaSotrudnikam::~OtpravkaSoobsheniaSotrudnikam()
{
    isOpen = false;
    delete ui;
}

void OtpravkaSoobsheniaSotrudnikam::on_pushButton_sendSoobsh_clicked()
{
    bool oshibka = false;

    if(ui->plainTextEdit_soobshenie->toPlainText().isEmpty())
    {
        oshibka =true;
    }

    if(oshibka==true)
    {
        ui->label_error->setVisible(true);
        return;
    }
    ui->label_error->setVisible(false);
    for(int i=0; i<ui->tableView_SotrKafOtpravkaSoobsh->model()->rowCount(); i++)
    {


       if (! dal_main->checkConnection())
        {
            QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
            return ;
        }
        if(ui->tableView_SotrKafOtpravkaSoobsh->model()->index(i,4).data().toString()=="")
            continue;
        else
        {
            smtp *mail = new smtp("smtp.mail.ru", 2525);
            mail->set_Login("kgtupocssystem@mail.ru");
            mail->set_Password_for_login("wer147896325wer");
            mail->set_Sender_mail("kgtupocssystem@mail.ru");
            mail->set_Recipient_mail(ui->tableView_SotrKafOtpravkaSoobsh->model()->index(i,4).data().toString());
            mail->set_Subject(QString::fromLocal8Bit("От кафедры"));
            mail->set_Body_text(ui->plainTextEdit_soobshenie->toPlainText());
            mail->send_Mail();
        }
    }
    QMessageBox::information(this,tr("Информация"),tr("Данные были отправлены"));

    ui->plainTextEdit_soobshenie->clear();

    SotrKafModels = dal_studentsControl->SendMessageSotrKaf();
    ui->tableView_SotrKafOtpravkaSoobsh->setModel(SotrKafModels);
    ui->tableView_SotrKafOtpravkaSoobsh->setColumnHidden(0,true);
    ui->tableView_SotrKafOtpravkaSoobsh->setColumnHidden(2,true);
    ui->tableView_SotrKafOtpravkaSoobsh->setColumnHidden(3,true);
    ui->tableView_SotrKafOtpravkaSoobsh->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_SotrKafOtpravkaSoobsh->horizontalHeader()->setStretchLastSection(true);
    ui->label_error->setVisible(false);
}
