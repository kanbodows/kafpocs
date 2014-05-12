#include "addoreditplanzaskaf.h"
#include "ui_addoreditplanzaskaf.h"
#include <QMessageBox>
#include"delegates/readonlydelegate.h"
#include <QDebug>
#include"delegates/word_wrap_delegate.h"

AddOrEditPlanZasKaf::AddOrEditPlanZasKaf(QWidget *parent, int select, int id, QString GodS1, QString GodPo1) :
    QDialog(parent),
    ui(new Ui::AddOrEditPlanZasKaf)
{
    ui->setupUi(this);
    ui->pushButtonSave->setVisible(false);
    ui->lineEditGodS->setValidator( new QIntValidator(1, 1000000, this));
    ui->lineEditGodS->setPlaceholderText("Вводятся только цифры");

    ui->lineEdit_GodPo->setValidator( new QIntValidator(1, 1000000, this));
    ui->lineEdit_GodPo->setPlaceholderText("Вводятся только цифры");

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
    ui->listView_Ispoln->setModel(ListViewSotr);
    ui->listView_Ispoln->setModelColumn(1);
    //    ui->comboBox_group->setCurrentIndex(-1);
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    if (select == 1)
    {
        this->sel_ct = 1;
        this->setWindowTitle(tr("Добавление вопросов"));
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
        this->isEdit = true;
        this->setWindowTitle(tr("Редактирование"));
        this->rec_id = id;
        if (! dal_main->checkConnection())
        {
            QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
            return;
        }
        QSqlQuery *query = new QSqlQuery;
        query = dal_prepodcontrol->getCurrentplanzasKaf(this->rec_id);
        ui->plainTextEditVoprosiZasedania->setPlainText(query->value(1).toString());
        ui->lineEditGodS->setText(query->value(5).toString());
        ui->lineEdit_GodPo->setText(query->value(6).toString());
        ui->lineEditSrokVipol->setText(query->value(3).toString());

        ui->lineEditGodS->setEnabled(false);
        ui->lineEdit_GodPo->setEnabled(false);
        ui->pushButtonAddTableWidjet->setVisible(false);
        ui->tableWidget->setVisible(false);
        ui->pushButton_AddToDataBase->setVisible(false);
        this->resize(100,500);
        ui->pushButtonSave->setVisible(true);
       // ui->listView_Ispoln->selectionModel()->setCurrentIndex(ListViewSotr->index(2,1),QItemSelectionModel::ClearAndSelect);
    }

    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->setItemDelegateForColumn(0, new TextEditDelegate(ui->tableWidget));
}

AddOrEditPlanZasKaf::~AddOrEditPlanZasKaf()
{
    delete ui;
}

void AddOrEditPlanZasKaf::on_pushButtonAddTableWidjet_clicked()
{ 
//        QString str = this->indexi;
//       QStringList list = str.split(' ');
//       list.removeAt(0);
//       qDebug() << list.join(' ');

   // ui->listView_Ispoln->selectionModel()->selectedIndexes();

    if(ui->checkBoxAllPrepod->isChecked()==false && ui->checkBox_Aspirant->isChecked()==false)
    {


        QString list; //Список для имен
//        QString newName;
//        QString d="";
//        QString index = ""; //Список для идентификаторов
        QModelIndexList mlist = ui->listView_Ispoln->selectionModel()->selectedIndexes();
        QSqlQueryModel *mod = (QSqlQueryModel*)ui->listView_Ispoln->model();//Получаем указатель на модель данных

//         QSqlQuery *query = new QSqlQuery;
//         int a[100];

        for(int i = 0;i < mlist.count();i++)
            //Получаем отображаемое имя
          {
                list.append(mlist.at(i).data(Qt::DisplayRole).toString() + ";\n");
//                newName = mlist.at(i).data(Qt::DisplayRole).toString();
//                qDebug()<<"newname = "<<newName;
//                if (! dal_main->checkConnection())
//                {
//                    QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
//                    return;
//                }

//                query->prepare("select * from is_sotrudniki  where st_fio like '%" + newName + "%'  ");
//                query->exec();
//                query->first();
//                ui->lineEdit->setText(query->value(0).toString());
//                index.append(ui->lineEdit->text() + " ");


            }
//qDebug()<<index;//"mlist"<<i<<mlist.at(i).data(Qt::DisplayRole).toString();

//for(int i=0; i < index.count(); i++)
//{
//    if(index[i]>='0' && index[i]<='9')
//    {
//        a[i]=index[i].digitValue();
//        while(index[i+1]!=' ')
//        {
//        i++;
//        a[i]=index[i].digitValue();
//        }
//    }
//    else
//        continue;
//    qDebug()<<"a[i]"<<a[i];
//}



            //Обращаемся к записи в модели данных и получаем из нее идентификатор
         // index.append(mod->record(mlist.value("id_sotr").toString());


        this->ispolnit = list;
    }
    bool error = false;
    if (ui->plainTextEditVoprosiZasedania->toPlainText()=="")
    {
        error = true;
    }

    if (ui->lineEditGodS->text()=="")
    {
        error = true;
    }

    if(ui->lineEdit_GodPo->text().isEmpty())
    {
        error = true;
    }


    if(ui->lineEditSrokVipol->text().isEmpty())
    {
        error = true;
    }

    if(this->ispolnit=="")
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
        ui->label_error->setVisible(false);
        addRMToTableWidget(ui->plainTextEditVoprosiZasedania->toPlainText(),
                           this->ispolnit,
                           ui->lineEditSrokVipol->text()
                           );
        ui->lineEditSrokVipol->setText("");
        ui->plainTextEditVoprosiZasedania->setPlainText("");
        ui->listView_Ispoln->selectionModel()->clear();

    }
}

void AddOrEditPlanZasKaf::on_checkBoxAllPrepod_clicked(bool checked)
{
    if(checked==true)
    {
        ui->checkBox_Aspirant->setEnabled(false);
        ui->listView_Ispoln->setEnabled(false);
        ui->checkBox_Aspirant->setChecked(false);
        ui->listView_Ispoln->selectionModel()->clear();
        this->ispolnit = "Все преподаватели";
    }
    else
    {
        ui->checkBox_Aspirant->setEnabled(true);
        ui->listView_Ispoln->setEnabled(true);
        this->ispolnit ="";
    }

}

void AddOrEditPlanZasKaf::on_checkBox_Aspirant_clicked(bool checked)
{
    if(checked==true)
    {
        ui->checkBoxAllPrepod->setEnabled(false);
        ui->listView_Ispoln->setEnabled(false);
        ui->checkBoxAllPrepod->setChecked(false);
        ui->listView_Ispoln->selectionModel()->clear();
        this->ispolnit = "Аспиранты";
    }
    else
    {
        ui->checkBoxAllPrepod->setEnabled(true);
        ui->listView_Ispoln->setEnabled(true);
        this->ispolnit ="";
    }
}

void AddOrEditPlanZasKaf::addRMToTableWidget(QString Vopros, QString Ispolnit, QString SrokiIspoln)
{
    int row = ui->tableWidget->rowCount();//кол-во записей в table
    ui->tableWidget->insertRow(row);//добавим новую запись
    //добавление новых элементов
    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(Vopros));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(Ispolnit));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem((SrokiIspoln)));
}

void AddOrEditPlanZasKaf::on_pushButtonCancel_clicked()
{
    this->close();
}

void AddOrEditPlanZasKaf::on_pushButton_AddToDataBase_clicked()
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
    for(int i=0; i < rowcount; i++)
        dal_prepodcontrol->addPlanZasKaf(ui->tableWidget->item(i,0)->text(),
                                         ui->tableWidget->item(i,1)->text(),
                                         ui->tableWidget->item(i,2)->text(),
                                         ui->lineEditGodS->text(),
                                         ui->lineEdit_GodPo->text());
    this->close();

    if(this->sel_ct!=1)
    {
        if (! dal_main->checkConnection())
        {
            QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
            return;
        }
        QString Gods = ui->lineEditGodS->text() +" - " + ui->lineEdit_GodPo->text();
        dal_prepodcontrol->addSozdPlanKaf(Gods,
                                          ui->lineEditGodS->text(),
                                          ui->lineEdit_GodPo->text());
    }

    QMessageBox::information(this, tr("Запись данных"), tr("Данные успешно добавлены"));
}

void AddOrEditPlanZasKaf::on_pushButtonSave_clicked()
{
    if(ui->checkBoxAllPrepod->isChecked()==false && ui->checkBox_Aspirant->isChecked()==false)
    {
        QString list; //Список для имен
        //QList index; //Список для идентификаторов
        QModelIndexList mlist = ui->listView_Ispoln->selectionModel()->selectedIndexes();
        //    QSqlQueryModel *mod = (QSqlQueryModel*)ui->listView_Ispoln->model();//Получаем указатель на модель данных
        for(int i = 0;i < mlist.count();i++){
            //Получаем отображаемое имя
            list.append(mlist.at(i).data(Qt::DisplayRole).toString() + ";\n");

            //Обращаемся к записи в модели данных и получаем из нее идентификатор
            // index.append(mod->record(mlist.row()).value("id").toInt());
        }

        this->ispolnit = list;
    }
    bool error = false;
    if (ui->plainTextEditVoprosiZasedania->toPlainText()=="")
    {
        error = true;
    }

    if (ui->lineEditGodS->text()=="")
    {
        error = true;
    }

    if(ui->lineEdit_GodPo->text().isEmpty())
    {
        error = true;
    }


    if(ui->lineEditSrokVipol->text().isEmpty())
    {
        error = true;
    }

    if(this->ispolnit=="")
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
        if (! dal_main->checkConnection())
        {
            QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
            return;
        }
        dal_prepodcontrol->editPlanZasKaf(this->rec_id,
                                          ui->lineEditGodS->text(),
                                          ui->lineEdit_GodPo->text(),
                                          ui->lineEditSrokVipol->text(),
                                          ui->plainTextEditVoprosiZasedania->toPlainText(),
                                          this->ispolnit);
    }
    this->close();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно изменены"));
}
