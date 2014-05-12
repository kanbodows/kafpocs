#include "addoredittrudovip.h"
#include "ui_addoredittrudovip.h"
#include <QMessageBox>
#include <QFileDialog>
#include<qfiledialog.h>
#include <QFile>
AddOrEditTrudoVip::AddOrEditTrudoVip(QWidget *parent, int select, int id) :
    QDialog(parent),
    ui(new Ui::AddOrEditTrudoVip)
{
    ui->setupUi(this);
    this->vip_id=0;
    this->group_id=0;
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    this->isEdit = false;
    dal_studentsControl = new Dal_studentsControl(this);
    QSqlQueryModel *comboGroup = new QSqlQueryModel(this);
    comboGroup = dal_studentsControl->getComboGroup(0);
    ui->comboBoxGroup->setModel(comboGroup);
    ui->comboBoxGroup->setModelColumn(1);
    ui->comboBoxGroup->setCurrentIndex(-1);
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
        int ids_groups;
        int ids_vip;

        QSqlQuery *query = new QSqlQuery;
        query = dal_studentsControl->getCurrentTrudoVipusknik(this->rec_id);
        ids_groups = query->value(2).toInt();
        ids_vip = query->value(1).toInt();
        qDebug()<<ids_vip;

        this->group_id = ids_groups;
        this->vip_id = ids_vip;
        comboGroup = dal_studentsControl->getComboGroup(0);
        ui->comboBoxGroup->setModel(comboGroup);
        ui->comboBoxGroup->setModelColumn(1);
        ui->comboBoxGroup->setCurrentIndex(-1);

        QSqlQueryModel *comboBox_Vipusks = new QSqlQueryModel(this);
        comboBox_Vipusks = dal_studentsControl->getVipGroup(this->group_id);
        ui->comboBox_Vipusk->setModel(comboBox_Vipusks);
        ui->comboBox_Vipusk->setModelColumn(5);
        ui->comboBox_Vipusk->setCurrentIndex(-1);

        ui->lineEdit_AdresRaboti->setText(query->value(5).toString());
        ui->lineEdit_ZanimaemayaDoljnost->setText(query->value(7).toString());
        ui->checkBox->setChecked(query->value(6).toInt());

        for (int j = 0; j < ui->comboBoxGroup->count(); j ++)
        {
            if (comboGroup->index(j, 0).data().toInt() == ids_groups)
            {
                ui->comboBoxGroup->setCurrentIndex(j);
                ui->comboBoxGroup->view()->setCurrentIndex(comboGroup->index(j, 0));
                break;
            }
        }
        for (int i = 0; i < ui->comboBox_Vipusk->count(); i ++)
        {
            if (comboBox_Vipusks->index(i, 1).data().toInt() == ids_vip)
            {
                ui->comboBox_Vipusk->setCurrentIndex(i);
                ui->comboBox_Vipusk->view()->setCurrentIndex(comboBox_Vipusks->index(i, 1));
                break;
            }
        }
    }

}

AddOrEditTrudoVip::~AddOrEditTrudoVip()
{
    delete ui;
}

void AddOrEditTrudoVip::on_comboBoxGroup_activated(int index)
{
    this->group_id = ui->comboBoxGroup->model()->index(ui->comboBoxGroup->currentIndex(),0).data().toInt();
    QSqlQueryModel *comboBox_Vipusk = new QSqlQueryModel(this);
    comboBox_Vipusk = dal_studentsControl->getVipGroup(this->group_id);
    ui->comboBox_Vipusk->setModel(comboBox_Vipusk);
    ui->comboBox_Vipusk->setModelColumn(5);
    ui->comboBox_Vipusk->setCurrentIndex(-1);
    if(ui->comboBox_Vipusk->count()==0)
        QMessageBox::information(this,tr("Информация"),tr("В данной группе нет выпускников"));
}

void AddOrEditTrudoVip::on_pushButton_clicked()
{
    bool error = false;
    if (ui->comboBoxGroup->currentIndex()==-1)
    {
        error = true;
    }

    if (ui->comboBox_Vipusk->currentIndex()==-1)
    {
        error = true;
    }


    if(ui->lineEdit_AdresRaboti->text().isEmpty())
    {
        error = true;
    }

    if(ui->lineEdit_ZanimaemayaDoljnost->text().isEmpty())
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
            if (dal_studentsControl->getTrudoVipAdd(this->vip_id,\
                                                    ui->lineEdit_AdresRaboti->text(),\
                                                    ui->lineEdit_ZanimaemayaDoljnost->text(),\
                                                    ui->checkBox->isChecked()))
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
            if (dal_studentsControl->editTrudoVipusknik(this->rec_id,\
                                                   this->vip_id,\
                                                   ui->lineEdit_AdresRaboti->text(),\
                                                   ui->lineEdit_ZanimaemayaDoljnost->text(),\
                                                   ui->checkBox->isChecked()))
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



void AddOrEditTrudoVip::on_comboBox_Vipusk_activated(int index)
{
    this->vip_id = ui->comboBox_Vipusk->model()->index(ui->comboBox_Vipusk->currentIndex(),1).data().toInt();
}

void AddOrEditTrudoVip::on_pushButton_2_clicked()
{
    this->close();
}
