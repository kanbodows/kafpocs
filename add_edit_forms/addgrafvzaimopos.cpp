#include "addgrafvzaimopos.h"
#include "ui_addgrafvzaimopos.h"
#include <QMessageBox>
#include <QDebug>
#include "delegates/transactiondatedelegate.h"
#include "delegates/transactiondatedelegatewithousec.h"

AddGrafVzaimopos::AddGrafVzaimopos(QWidget *parent, int select, int id, QString GodS1, QString GodPo1) :
    QDialog(parent),
    ui(new Ui::AddGrafVzaimopos)
{
    ui->setupUi(this);
    ui->pushButtonSave->setVisible(false);
    ui->label_error->setVisible(false);
    ui->lineEditGodS->setValidator( new QIntValidator(1, 1000000, this));
    ui->lineEditGodS->setPlaceholderText("Вводятся только цифры");

    ui->lineEdit_GodPo->setValidator( new QIntValidator(1, 1000000, this));
    ui->lineEdit_GodPo->setPlaceholderText("Вводятся только цифры");



    dal_prepodcontrol = new Dal_prepodcontrol(this);
    dal_studentsControl = new Dal_studentsControl(this);
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    ui->comboBoxProver->setEnabled(false);
    QSqlQueryModel *comboPrepod = new QSqlQueryModel(this);
    comboPrepod = dal_studentsControl->getSotrKaf(this->sotr_ids);
    ui->comboBox_Prepod->setModel(comboPrepod);
    ui->comboBox_Prepod->setModelColumn(1);
    ui->comboBox_Prepod->setCurrentIndex(-1);

    ui->dateTimeEdit->setDateTime(QDateTime::currentDateTime());

    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }


    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->setColumnHidden(3,true);
    ui->tableWidget->setColumnHidden(4,true);
    ui->tableWidget->setItemDelegateForColumn(2, new transactiondatedelegatewithousec(ui->tableWidget));

    if (select == 1)
    {

        ui->pushButtonSave->setVisible(false);
        ui->groupBox->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
        this->sel_ct = 1;
        this->setWindowTitle(tr("Добавление данных"));
        ui->lineEditGodS->setText(GodS1);
        ui->lineEdit_GodPo->setText(GodPo1);
        ui->lineEditGodS->setEnabled(false);
        ui->lineEdit_GodPo->setEnabled(false);
        //                ui->listView_Ispoln->setCurrentIndex(ListViewSotr->index(0, 2));
        //        ui->listView_Ispoln->selectionModel()->selectedIndexes();
        //        ui->listView_Ispoln->selectionModel()->select();

    }

    if(id!=-1)
    {
        ui->groupBox->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
        this->isEdit = true;
        this->setWindowTitle(tr("Редактирование"));
        this->rec_id = id;

        if (! dal_main->checkConnection())
        {
            QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
            return;
        }
        QSqlQuery *query = new QSqlQuery;
        query = dal_prepodcontrol->getCurrentGrafVzaimoPos(this->rec_id);
        ui->comboBoxProver->setEnabled(false);
        ui->lineEditGodS->setEnabled(false);
        ui->lineEdit_GodPo->setEnabled(false);

        ui->pushButtonAddToTableWidjet->setVisible(false);
//        ui->tableWidget->setVisible(false);
//        ui->pushButton_AddDataBase->setVisible(false);
        ui->frame->setVisible(false);
        this->resize(30,100);
//        ui->groupBox->resize(60,500);
        this->adjustSize();
        ui->pushButtonSave->setVisible(true);

        ui->lineEditGodS->setText(query->value(4).toString());
        ui->lineEdit_GodPo->setText(query->value(5).toString());
        ui->comboBoxProver->setEnabled(true);
        QSqlQueryModel *comboProver = new QSqlQueryModel(this);
        comboProver = dal_studentsControl->getSotrKaf(query->value(1).toInt());
        ui->comboBoxProver->setModel(comboProver);
        ui->comboBoxProver->setModelColumn(1);
        ui->comboBoxProver->setCurrentIndex(-1);

        ui->dateTimeEdit->setDateTime(query->value(2).toDateTime());
        for (int j = 0; j < ui->comboBox_Prepod->count(); j ++)
        {
            if (comboPrepod->index(j, 0).data().toInt() == query->value(1).toInt())
            {
                ui->comboBox_Prepod->setCurrentIndex(j);
                ui->comboBox_Prepod->view()->setCurrentIndex(comboPrepod->index(j, 0));
                break;
            }
        }

        for (int j = 0; j < ui->comboBoxProver->count(); j ++)
        {
            if (comboProver->index(j, 0).data().toInt() == query->value(3).toInt())
            {
                ui->comboBoxProver->setCurrentIndex(j);
                ui->comboBoxProver->view()->setCurrentIndex(comboProver->index(j, 0));
                break;
            }
        }

    }


}

AddGrafVzaimopos::~AddGrafVzaimopos()
{
    delete ui;
}

void AddGrafVzaimopos::on_comboBox_Prepod_activated(int index)
{
    ui->comboBoxProver->setEnabled(true);
    this->sotr_ids = ui->comboBox_Prepod->model()->index(ui->comboBox_Prepod->currentIndex(), 0).data().toInt();
    QSqlQueryModel *comboProver = new QSqlQueryModel(this);
    comboProver = dal_studentsControl->getSotrKaf(this->sotr_ids);
    ui->comboBoxProver->setModel(comboProver);
    ui->comboBoxProver->setModelColumn(1);
    ui->comboBoxProver->setCurrentIndex(-1);
}

void AddGrafVzaimopos::on_pushButtonAddToTableWidjet_clicked()
{
    qDebug()<<ui->dateTimeEdit->dateTime();
    bool error = false;

    if (ui->lineEditGodS->text()=="")
    {
        error = true;
    }

    if(ui->lineEdit_GodPo->text().isEmpty())
    {
        error = true;
    }


    if(ui->comboBoxProver->currentIndex()==-1)
    {
        error = true;
    }

    if(ui->comboBox_Prepod->currentIndex()==-1)
    {
        error = true;
    }


    for(int i=0; i<ui->tableWidget->rowCount();i++)
    {
        if(ui->comboBox_Prepod->model()->index(ui->comboBox_Prepod->currentIndex(),0).data().toInt()==ui->tableWidget->item(i,3)->text().toInt() && \
                ui->dateTimeEdit->dateTime().toString("yyyy-MM-ddThh:mm")==ui->tableWidget->item(i,2)->text())
        {
            QMessageBox::warning(this,tr("Ошибка"),tr("Выбранному преподавателю\n уже назначен проверяющий на выбранный период времени"));
            return;
        }
    }

    for(int i=0; i<ui->tableWidget->rowCount();i++)
    {
        if(ui->comboBoxProver->model()->index(ui->comboBoxProver->currentIndex(),0).data().toInt()==ui->tableWidget->item(i,4)->text().toInt() && \
                ui->dateTimeEdit->dateTime().toString("yyyy-MM-ddThh:mm")==ui->tableWidget->item(i,2)->text())
        {
            QMessageBox::warning(this,tr("Ошибка"),tr("Проверяющий не может проверять\n в двух местах одновременно"));
            return;
        }
    }

    for(int i=0; i<ui->tableWidget->rowCount();i++)
    {
        if(ui->comboBox_Prepod->model()->index(ui->comboBox_Prepod->currentIndex(),0).data().toInt()==ui->tableWidget->item(i,4)->text().toInt() && \
                ui->dateTimeEdit->dateTime().toString("yyyy-MM-ddThh:mm")==ui->tableWidget->item(i,2)->text())
        {
            QMessageBox::warning(this,tr("Ошибка"),tr("Проверяющий не может проверять\n и вести пару"));
            return;
        }
    }

    if( dal_prepodcontrol->ProverkaPrepod(ui->comboBox_Prepod->model()->index(ui->comboBox_Prepod->currentIndex(),0).data().toInt(), ui->dateTimeEdit->dateTime())==true)
    {
        QMessageBox::warning(this,tr("Ошибка"),tr("Выбранному преподавателю\n уже назначен проверяющий на выбранный период времени"));
        return;
    }

    if( dal_prepodcontrol->ProverProveraush(ui->comboBoxProver->model()->index(ui->comboBoxProver->currentIndex(),0).data().toInt(), ui->dateTimeEdit->dateTime())==true)
    {
        QMessageBox::warning(this,tr("Ошибка"),tr("Проверяющий не может проверять\n в двух местах одновременно"));
        return;
    }

    if(ui->lineEditGodS->text().toInt()>ui->lineEdit_GodPo->text().toInt() || ui->lineEditGodS->text().toInt()==ui->lineEdit_GodPo->text().toInt())
    {
        QMessageBox::warning(this,tr("Ошибка"),tr("Не корректно введено начало и конец учебного года"));
        return;
    }

    if(this->sel_ct==1)
    {
        if( dal_prepodcontrol->ProverkaPrepod(ui->comboBox_Prepod->model()->index(ui->comboBox_Prepod->currentIndex(),0).data().toInt(), ui->dateTimeEdit->dateTime())==true)
        {
            QMessageBox::warning(this,tr("Ошибка"),tr("Выбранному преподавателю\n уже назначен проверяющий на выбранный период времени"));
            return;
        }

        if( dal_prepodcontrol->ProverProveraush(ui->comboBoxProver->model()->index(ui->comboBoxProver->currentIndex(),0).data().toInt(), ui->dateTimeEdit->dateTime())==true)
        {
            QMessageBox::warning(this,tr("Ошибка"),tr("Проверяющий не может проверять\n в двух местах одновременно"));
            return;
        }
    }

    if (error==true)
    {
        ui->label_error->setVisible(true);
        return;
    }
    else
    {
        ui->label_error->setVisible(false);
        ui->lineEditGodS->setEnabled(false);
        ui->lineEdit_GodPo->setEnabled(false);
        ui->comboBoxProver->setEnabled(false);
        addRMToTableWidget(ui->comboBox_Prepod->model()->index(ui->comboBox_Prepod->currentIndex(),1).data().toString(),
                           ui->comboBoxProver->model()->index(ui->comboBoxProver->currentIndex(),1).data().toString(),
                           ui->dateTimeEdit->dateTime(),
                           ui->comboBox_Prepod->model()->index(ui->comboBox_Prepod->currentIndex(),0).data().toInt(),
                           ui->comboBoxProver->model()->index(ui->comboBoxProver->currentIndex(),0).data().toInt()
                           );
        ui->comboBoxProver->setCurrentIndex(-1);
        ui->comboBox_Prepod->setCurrentIndex(-1);
    }
}

void AddGrafVzaimopos::addRMToTableWidget(QString FIOPrepod, QString FIOProv, QDateTime DataProv, int FIOPrepod_id, int FIOProv_id)
{
    int row = ui->tableWidget->rowCount();//кол-во записей в table
    ui->tableWidget->insertRow(row);//добавим новую запись
    //добавление новых элементов
    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(FIOPrepod));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(FIOProv));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem(DataProv.toString("yyyy-MM-ddThh:mm")));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(QString::number(FIOPrepod_id)));
    ui->tableWidget->setItem(row, 4, new QTableWidgetItem(QString::number(FIOProv_id)));
}

void AddGrafVzaimopos::on_pushButton_AddDataBase_clicked()
{
    if(ui->tableWidget->rowCount()==0)
    {
        QMessageBox::information(this,tr("Информация"),tr("Добавьте данные"));
        return;
    }
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    int rowcount = ui->tableWidget->rowCount();
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    if(this->sel_ct!=1)
    {
        if( dal_prepodcontrol->ProverkaGoda(ui->lineEditGodS->text(), ui->lineEdit_GodPo->text())==true)
        {
            QMessageBox::information(this, tr("Информация"), tr("Запись данных не возможна\n смените год создания взаимопосещения"));
            return;
        }
    }

    for(int i=0; i < rowcount; i++)
        dal_prepodcontrol->addGrafikVzaimopoa(ui->tableWidget->item(i,3)->text().toInt(),
                                              ui->tableWidget->item(i,2)->text(),
                                              ui->tableWidget->item(i,4)->text().toInt(),
                                              ui->lineEditGodS->text(),
                                              ui->lineEdit_GodPo->text());

    if(this->sel_ct!=1)
    {
        if (! dal_main->checkConnection())
        {
            QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
            return;
        }
        QString Gods = ui->lineEditGodS->text() +" - " + ui->lineEdit_GodPo->text();
        dal_prepodcontrol->addSozdGrafKaf(Gods,
                                          ui->lineEditGodS->text(),
                                          ui->lineEdit_GodPo->text());
    }
    QMessageBox::information(this, tr("Запись данных"), tr("Данные успешно добавлены"));
    this->close();
}





void AddGrafVzaimopos::on_pushButtonSave_clicked()
{
    if( dal_prepodcontrol->ProverNaIzmenenia(ui->comboBoxProver->model()->index(ui->comboBoxProver->currentIndex(),0).data().toInt(), ui->dateTimeEdit->dateTime())==true)
    {
        QMessageBox::warning(this,tr("Ошибка"),tr("Проверяющий не может проверять\n в двух местах одновременно"));
        return;
    }

    if( dal_prepodcontrol->ProverNaIzmForPrepod(ui->comboBox_Prepod->model()->index(ui->comboBox_Prepod->currentIndex(),0).data().toInt(), ui->dateTimeEdit->dateTime())==true)
    {
        QMessageBox::warning(this,tr("Ошибка"),tr("Выбранному преподавателю\n уже назначен проверяющий на выбранный период времени"));
        return;
    }

    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    if (dal_prepodcontrol->editGrafkik(this->rec_id,\
                                       ui->comboBox_Prepod->model()->index(ui->comboBox_Prepod->currentIndex(), 0).data().toInt(), \
                                       ui->comboBoxProver->model()->index(ui->comboBoxProver->currentIndex(), 0).data().toInt(),\
                                       ui->dateTimeEdit->dateTime(),\
                                       ui->lineEditGodS->text(),\
                                       ui->lineEdit_GodPo->text()))
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
