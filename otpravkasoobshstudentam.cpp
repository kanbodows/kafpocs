#include "otpravkasoobshstudentam.h"
#include "ui_otpravkasoobshstudentam.h"
#include "dals/dal_studentscontrol.h"
#include <QMessageBox>
bool OtpravkaSoobshStudentam::isOpen = false;
OtpravkaSoobshStudentam::OtpravkaSoobshStudentam(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OtpravkaSoobshStudentam)
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
    this->logc="";
    this->parol="";
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    dal_studentsControl = new Dal_studentsControl(this);
    this->Group=0;

    ui->comboBox_group->setModel(dal_studentsControl->getComboGroup(0));
    ui->comboBox_group->setModelColumn(1);
    ui->comboBox_group->setCurrentIndex(-1);
    StudentModels = dal_studentsControl->SendMessageStud(this->Group);

    ui->tableView_studOtpravkaSoobsh->setModel(StudentModels);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(0,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(2,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(3,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(4,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(5,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(6,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(7,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(9,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(10,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(11,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(12,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(13,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(14,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(15,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(16,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(17,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(18,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(19,true);
    ui->tableView_studOtpravkaSoobsh->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_studOtpravkaSoobsh->horizontalHeader()->setStretchLastSection(true);
    ui->label_error->setVisible(false);
}

OtpravkaSoobshStudentam::~OtpravkaSoobshStudentam()
{
    isOpen = false;
    delete ui;
}

void OtpravkaSoobshStudentam::on_pushButton_sendSoobsh_clicked()
{
    bool oshibka = false;

    if(ui->comboBox_group->currentIndex()==-1)
    {
        oshibka = true;
    }
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
    for(int i=0; i<ui->tableView_studOtpravkaSoobsh->model()->rowCount(); i++)
    {


        if (! dal_main->checkConnection())
        {
            QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
            return ;
        }
        if(ui->tableView_studOtpravkaSoobsh->model()->index(i,8).data().toString()=="")//проверить
            continue;
        else
        {
            smtp *mail = new smtp("smtp.mail.ru", 2525);
            mail->set_Login("kgtupocssystem@mail.ru");
            mail->set_Password_for_login("wer147896325wer");
            mail->set_Sender_mail("kgtupocssystem@mail.ru");
            mail->set_Recipient_mail(ui->tableView_studOtpravkaSoobsh->model()->index(i,8).data().toString());//проверить
            mail->set_Subject(QString::fromLocal8Bit("От кафедры"));
            mail->set_Body_text(ui->plainTextEdit_soobshenie->toPlainText());
            mail->send_Mail();
        }
    }
    QMessageBox::information(this,tr("Информация"),tr("Данные были отправлены"));

    ui->plainTextEdit_soobshenie->clear();

    StudentModels = dal_studentsControl->SendMessageSotrKaf();
    ui->tableView_studOtpravkaSoobsh->setModel(StudentModels);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(0,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(2,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(3,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(4,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(5,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(6,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(7,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(9,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(10,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(11,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(12,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(13,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(14,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(15,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(16,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(17,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(18,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(19,true);
    ui->tableView_studOtpravkaSoobsh->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_studOtpravkaSoobsh->horizontalHeader()->setStretchLastSection(true);
    ui->label_error->setVisible(false);
}

void OtpravkaSoobshStudentam::on_comboBox_group_activated(const QString &arg1)
{

}

void OtpravkaSoobshStudentam::on_comboBox_group_activated(int index)
{
    this->Group = ui->comboBox_group->model()->index(ui->comboBox_group->currentIndex(),0).data().toInt();
    StudentModels = dal_studentsControl->SendMessageStud(this->Group);
    ui->tableView_studOtpravkaSoobsh->setModel(StudentModels);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(0,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(2,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(3,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(4,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(5,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(6,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(7,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(9,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(10,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(11,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(12,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(13,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(14,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(15,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(16,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(17,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(18,true);
    ui->tableView_studOtpravkaSoobsh->setColumnHidden(19,true);

    ui->tableView_studOtpravkaSoobsh->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_studOtpravkaSoobsh->horizontalHeader()->setStretchLastSection(true);
}
