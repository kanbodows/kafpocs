#include "addoredit_srez.h"
#include "ui_addoredit_srez.h"

addoredit_srez::addoredit_srez(QWidget *parent, bool isEdit, int id_srez) :
    QDialog(parent),
    id_srez(id_srez),
    isEdit(isEdit),
    ui(new Ui::addoredit_srez)
{
    ui->setupUi(this);
    ui->lineEdit_god->setDate(QDate::currentDate());
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    dal_studentscontrol = new Dal_studentsControl(this);
    ui->lineEdit_god->setDate(QDate::currentDate());
    ui->comboBox_discipline_1->setModel(dal_studentscontrol->getDiscipl());
    ui->comboBox_discipline_1->setModelColumn(1);
    ui->comboBox_discipline_1->setCurrentIndex(-1);
    ui->comboBox_discipline_2->setModel(dal_studentscontrol->getDiscipl());
    ui->comboBox_discipline_2->setModelColumn(1);
    ui->comboBox_discipline_2->setCurrentIndex(-1);
    ui->comboBox_discipline_3->setModel(dal_studentscontrol->getDiscipl());
    ui->comboBox_discipline_3->setModelColumn(1);
    ui->comboBox_discipline_3->setCurrentIndex(-1);
    ui->comboBox_discipline_4->setModel(dal_studentscontrol->getDiscipl());
    ui->comboBox_discipline_4->setModelColumn(1);
    ui->comboBox_discipline_4->setCurrentIndex(-1);
    ui->comboBox_discipline_5->setModel(dal_studentscontrol->getDiscipl());
    ui->comboBox_discipline_5->setModelColumn(1);
    ui->comboBox_discipline_5->setCurrentIndex(-1);
    ui->comboBox_gruppa->setModel(dal_studentscontrol->getComboGroup(0));
    ui->comboBox_gruppa->setModelColumn(1);
    ui->comboBox_gruppa->setCurrentIndex(-1);
    ui->label_error->setVisible(false);
    if(this->isEdit)
    {
        this->setWindowTitle("Редактирование записи");
        QSqlQuery *query = new QSqlQuery;
        query = dal_studentscontrol->getCurrentSrez(this->id_srez);

        for (int i = 0; i < ui->comboBox_gruppa->count(); i++)
        {
            if (ui->comboBox_gruppa->model()->index(i, 0).data().toInt() == query->value(1).toInt())
            {
                ui->comboBox_gruppa->setCurrentIndex(i);
                ui->comboBox_gruppa->view()->setCurrentIndex(ui->comboBox_gruppa->model()->index(i, 0));
                break;
            }
        }        
        ui->lineEdit_god->setDate(query->value(2).toDate());
        for (int j = 0; j < ui->comboBox_discipline_1->count(); j++)
        {
            if (ui->comboBox_discipline_1->model()->index(j, 0).data().toInt() == query->value(3).toInt())
            {
                ui->comboBox_discipline_1->setCurrentIndex(j);
                ui->comboBox_discipline_1->view()->setCurrentIndex(ui->comboBox_discipline_1->model()->index(j, 0));
                break;
            }
        }
        for (int j = 0; j < ui->comboBox_discipline_2->count(); j++)
        {
            if (ui->comboBox_discipline_2->model()->index(j, 0).data().toInt() == query->value(4).toInt())
            {
                ui->comboBox_discipline_2->setCurrentIndex(j);
                ui->comboBox_discipline_2->view()->setCurrentIndex(ui->comboBox_discipline_2->model()->index(j, 0));
                break;
            }
        }
        for (int j = 0; j < ui->comboBox_discipline_3->count(); j++)
        {
            if (ui->comboBox_discipline_3->model()->index(j, 0).data().toInt() == query->value(5).toInt())
            {
                ui->comboBox_discipline_3->setCurrentIndex(j);
                ui->comboBox_discipline_3->view()->setCurrentIndex(ui->comboBox_discipline_3->model()->index(j, 0));
                break;
            }
        }
        for (int j = 0; j < ui->comboBox_discipline_4->count(); j++)
        {
            if (ui->comboBox_discipline_4->model()->index(j, 0).data().toInt() == query->value(6).toInt())
            {
                ui->comboBox_discipline_4->setCurrentIndex(j);
                ui->comboBox_discipline_4->view()->setCurrentIndex(ui->comboBox_discipline_4->model()->index(j, 0));
                break;
            }
        }
        for (int j = 0; j < ui->comboBox_discipline_5->count(); j++)
        {
            if (ui->comboBox_discipline_5->model()->index(j, 0).data().toInt() == query->value(7).toInt())
            {
                ui->comboBox_discipline_5->setCurrentIndex(j);
                ui->comboBox_discipline_5->view()->setCurrentIndex(ui->comboBox_discipline_5->model()->index(j, 0));
                break;
            }
        }
    }
}

addoredit_srez::~addoredit_srez()
{
    delete ui;
}

void addoredit_srez::on_pushButton_cancel_clicked()
{
    this->close();
}

void addoredit_srez::on_pushButton_ok_clicked()
{
    if (ui->comboBox_discipline_1->currentIndex() == -1 || ui->comboBox_gruppa->currentIndex() == -1)
    {
        ui->label_error->setVisible(true);
        return;
    }
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    if (! this->isEdit)
    {
        if (dal_studentscontrol->addSrez(ui->comboBox_gruppa->model()->index(ui->comboBox_gruppa->currentIndex(),0).data().toInt(),
                                         ui->lineEdit_god->date(),
                                         ui->comboBox_discipline_1->model()->index(ui->comboBox_discipline_1->currentIndex(),0).data().toInt(),
                                         ui->comboBox_discipline_2->model()->index(ui->comboBox_discipline_2->currentIndex(),0).data().toInt(),
                                         ui->comboBox_discipline_3->model()->index(ui->comboBox_discipline_3->currentIndex(),0).data().toInt(),
                                         ui->comboBox_discipline_4->model()->index(ui->comboBox_discipline_4->currentIndex(),0).data().toInt(),
                                         ui->comboBox_discipline_5->model()->index(ui->comboBox_discipline_5->currentIndex(),0).data().toInt()))
        {

            QMessageBox::information(this, tr("Информация"), tr("Запись успешно добавлена"));
        }
        else
        {
            QMessageBox::warning(this, tr("Ошибка"), tr("Данные не были добавлены в базу данных"));
        }
    }
    else
    {
        if (dal_studentscontrol->editSrez(this->id_srez,
                                          ui->comboBox_gruppa->model()->index(ui->comboBox_gruppa->currentIndex(),0).data().toInt(),
                                          ui->lineEdit_god->date(),
                                          ui->comboBox_discipline_1->model()->index(ui->comboBox_discipline_1->currentIndex(),0).data().toInt(),
                                          ui->comboBox_discipline_2->model()->index(ui->comboBox_discipline_2->currentIndex(),0).data().toInt(),
                                          ui->comboBox_discipline_3->model()->index(ui->comboBox_discipline_3->currentIndex(),0).data().toInt(),
                                          ui->comboBox_discipline_4->model()->index(ui->comboBox_discipline_4->currentIndex(),0).data().toInt(),
                                          ui->comboBox_discipline_5->model()->index(ui->comboBox_discipline_5->currentIndex(),0).data().toInt()))
        {
            QMessageBox::information(this, tr("Информация"), tr("Данные успешно отредактированы"));
        }
        else
        {
            QMessageBox::warning(this, tr("Ошибка"), tr("Данные не были изменены"));
        }
    }
    this->close();
}



void addoredit_srez::on_pushButton_choose_clicked()
{
    //    this->NameFails = "";
    //    QString filename = QFileDialog::getOpenFileName(this,tr("open file"),"/home","All files(*.*)");
    //    filename = filename.replace("/" , "\\");

    //    this->UrlFail = filename;

    //    QString name =  QFileInfo(filename).baseName();
    //    ui->lineEdit_NameFile->setText(name);
    //    this->NameFails = name;

}
