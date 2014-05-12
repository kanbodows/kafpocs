#include "otpravkasoobshvipusk.h"
#include "ui_otpravkasoobshvipusk.h"
#include "dals/dal_studentscontrol.h"
#include <QMessageBox>
#include "delegates/datedelegate.h"
#include "delegates/checkboxdelegate.h"
bool OtpravkaSoobshVipusk::isOpen = false;
OtpravkaSoobshVipusk::OtpravkaSoobshVipusk(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OtpravkaSoobshVipusk)
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
    this->Student="";
    this->Group="";
    this->dateBeginning = QDate();
    this->dateEnd = QDate();

    ui->comboBox_group->setModel(dal_studentsControl->getComboGroup(0));
    ui->comboBox_group->setModelColumn(1);
    ui->comboBox_group->setCurrentIndex(-1);
    vipusknikModels = dal_studentsControl->SendMessageVip(this->Group);

    ui->tableView_vipOtpravkaSoobsh->setModel(vipusknikModels);
    ui->tableView_vipOtpravkaSoobsh->setColumnHidden(0,true);
    ui->tableView_vipOtpravkaSoobsh->setColumnHidden(1,true);
    ui->tableView_vipOtpravkaSoobsh->setColumnHidden(2,true);
    ui->tableView_vipOtpravkaSoobsh->setColumnHidden(3,true);
    ui->tableView_vipOtpravkaSoobsh->setColumnHidden(4,true);
    ui->tableView_vipOtpravkaSoobsh->setColumnHidden(6,true);
    ui->tableView_vipOtpravkaSoobsh->setColumnHidden(8,true);
    ui->tableView_vipOtpravkaSoobsh->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_vipOtpravkaSoobsh->horizontalHeader()->setStretchLastSection(true);
    ui->label_error->setVisible(false);
}

OtpravkaSoobshVipusk::~OtpravkaSoobshVipusk()
{
    isOpen = false;
    delete ui;
}

void OtpravkaSoobshVipusk::on_pushButton_sendSoobsh_clicked()
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
    for(int i=0; i<ui->tableView_vipOtpravkaSoobsh->model()->rowCount(); i++)
    {


       if (! dal_main->checkConnection())
        {
            QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
            return ;
        }
        if(ui->tableView_vipOtpravkaSoobsh->model()->index(i,7).data().toString()=="")
            continue;
        else
        {
            smtp *mail = new smtp("smtp.mail.ru", 2525);
            mail->set_Login("kgtupocssystem@mail.ru");
            mail->set_Password_for_login("wer147896325wer");
            mail->set_Sender_mail("kgtupocssystem@mail.ru");
            mail->set_Recipient_mail(ui->tableView_vipOtpravkaSoobsh->model()->index(i,7).data().toString());
            mail->set_Subject(QString::fromLocal8Bit("От кафедры"));
            mail->set_Body_text(ui->plainTextEdit_soobshenie->toPlainText());
            mail->send_Mail();
        }
    }
    QMessageBox::information(this,tr("Информация"),tr("Данные были отправлены"));
    ui->comboBox_group->setCurrentIndex(-1);
    ui->plainTextEdit_soobshenie->clear();
    this->Group = "";
    vipusknikModels = dal_studentsControl->SendMessageVip(this->Group);
    ui->tableView_vipOtpravkaSoobsh->setModel(vipusknikModels);
    ui->tableView_vipOtpravkaSoobsh->setColumnHidden(0,true);
    ui->tableView_vipOtpravkaSoobsh->setColumnHidden(1,true);
    ui->tableView_vipOtpravkaSoobsh->setColumnHidden(2,true);
    ui->tableView_vipOtpravkaSoobsh->setColumnHidden(3,true);
    ui->tableView_vipOtpravkaSoobsh->setColumnHidden(4,true);
    ui->tableView_vipOtpravkaSoobsh->setColumnHidden(6,true);
    ui->tableView_vipOtpravkaSoobsh->setColumnHidden(8,true);
    ui->tableView_vipOtpravkaSoobsh->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_vipOtpravkaSoobsh->horizontalHeader()->setStretchLastSection(true);
}


void OtpravkaSoobshVipusk::on_comboBox_group_activated(const QString &arg1)
{

    this->Group = arg1;
    vipusknikModels = dal_studentsControl->SendMessageVip(this->Group);
    ui->tableView_vipOtpravkaSoobsh->setModel(vipusknikModels);
    ui->tableView_vipOtpravkaSoobsh->setColumnHidden(0,true);
    ui->tableView_vipOtpravkaSoobsh->setColumnHidden(1,true);
    ui->tableView_vipOtpravkaSoobsh->setColumnHidden(2,true);
    ui->tableView_vipOtpravkaSoobsh->setColumnHidden(3,true);
    ui->tableView_vipOtpravkaSoobsh->setColumnHidden(4,true);
    ui->tableView_vipOtpravkaSoobsh->setColumnHidden(6,true);
    ui->tableView_vipOtpravkaSoobsh->setColumnHidden(8,true);

    ui->tableView_vipOtpravkaSoobsh->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_vipOtpravkaSoobsh->horizontalHeader()->setStretchLastSection(true);
}
