#include "addoreditplanumr.h"
#include "ui_addoreditplanumr.h"
#include <QMessageBox>
#include"delegates/readonlydelegate.h"
#include <QDebug>
#include"delegates/word_wrap_delegate.h"

AddOrEditPlanUmr::AddOrEditPlanUmr(QWidget *parent, int select, int id) :
    QDialog(parent),
    ui(new Ui::AddOrEditPlanUmr)
{
    ui->setupUi(this);
    ui->lineEditGod->setValidator( new QIntValidator(1, 1000000, this));
    ui->lineEditGod->setPlaceholderText("Вводятся только цифры");
    this->ispolnit="";
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }

    ui->label_error->setVisible(false);
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }

    QSqlQueryModel *ListViewSotr = new QSqlQueryModel(this);
    ListViewSotr = dal_prepodcontrol->getSotrModels("", 0, "");
    ui->listView_fio->setModel(ListViewSotr);
    ui->listView_fio->setModelColumn(1);

    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    if (select == 1)
    {
        this->setWindowTitle(tr("Добавление"));
        ui->checkBox->setVisible(false);
        ui->label->setVisible(true);
    }
    if(id!=-1)
    {
        ui->checkBox->setVisible(true);
        ui->listView_fio->setVisible(false);
        ui->label->setVisible(false);
        this->isEdit = true;
        this->setWindowTitle(tr("Редактирование"));
        this->rec_id = id;
        if (! dal_main->checkConnection())
        {
            QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
            return;
        }
        QSqlQuery *query = new QSqlQuery;
        query = dal_prepodcontrol->getCurrentplanUmr(this->rec_id);
        ui->plainTextEdit_UMR->setPlainText(query->value(2).toString());
        ui->plainTextEdit_anotac->setPlainText(query->value(3).toString());
        ui->doubleSpinBox_obiem->setValue(query->value(4).toDouble());
        ui->spinBox_tiraj->setValue(query->value(5).toInt());
        ui->lineEdit_srokIspoln->setText(query->value(6).toString());
        ui->lineEditGod->setText(query->value(7).toString());
    }
}

AddOrEditPlanUmr::~AddOrEditPlanUmr()
{
    delete ui;
}

void AddOrEditPlanUmr::on_pushButton_add_clicked()
{

    QString list; //Список для имен

    QModelIndexList mlist = ui->listView_fio->selectionModel()->selectedIndexes();
    for(int i = 0;i < mlist.count();i++)
        //Получаем отображаемое имя
      {
            list.append(mlist.at(i).data(Qt::DisplayRole).toString() + ";\n");

      }
     this->ispolnit = list;
    bool error = false;
    qDebug()<<"ssssss"<<ui->checkBox->isChecked();
    if(ui->checkBox->isChecked()==true)
    if(this->ispolnit=="")
    {
        error = true;
    }

    if (ui->plainTextEdit_UMR->toPlainText()=="")
    {
        error = true;
    }

    if (ui->plainTextEdit_anotac->toPlainText()=="")
    {
        error = true;
    }

    if (ui->doubleSpinBox_obiem->value()==0)
    {
        error = true;
    }

    if(ui->spinBox_tiraj->value()==0)
    {
        error = true;
    }


    if(ui->lineEdit_srokIspoln->text().isEmpty())
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
        ui->label_error->setVisible(false);
        dal_prepodcontrol->addPlanUMR(this->ispolnit,
                                      ui->plainTextEdit_UMR->toPlainText(),
                                      ui->plainTextEdit_anotac->toPlainText(),
                                      ui->doubleSpinBox_obiem->value(),
                                      ui->spinBox_tiraj->value(),
                                      ui->lineEdit_srokIspoln->text(),
                                      ui->lineEditGod->text());
        this->close();
        }

        else
        {
            if (! dal_main->checkConnection())
            {
                QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
                return;
            }
            ui->label_error->setVisible(false);
             if (dal_prepodcontrol->EditPlanUMR(this->rec_id,
                                           this->ispolnit,
                                           ui->plainTextEdit_UMR->toPlainText(),
                                           ui->plainTextEdit_anotac->toPlainText(),
                                           ui->doubleSpinBox_obiem->value(),
                                           ui->spinBox_tiraj->value(),
                                           ui->lineEdit_srokIspoln->text(),
                                           ui->lineEditGod->text()))
            {
                QMessageBox::information(this, tr("Информация"), tr("Данные успешно отредактированы"));
            }
            else
            {
                QMessageBox::warning(this, tr("Ошибка"), tr("Данные не были изменены"));
                return;
            }
        }
        this->close();

    }
}

void AddOrEditPlanUmr::on_checkBox_clicked(bool checked)
{
    if(checked==true)
        ui->listView_fio->setVisible(true);
    else
        ui->listView_fio->setVisible(false);
}

void AddOrEditPlanUmr::on_pushButton_cancel_clicked()
{
    this->close();
}
