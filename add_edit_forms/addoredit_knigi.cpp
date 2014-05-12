#include "addoredit_knigi.h"
#include "ui_addoredit_knigi.h"
#include <QFileDialog>
#include <qfiledialog.h>
#include <QFile>

addoredit_knigi::addoredit_knigi(QWidget *parent, bool isEdit, int id_knigi, int discipline_id) :
    QDialog(parent),
    id_knigi(id_knigi),
    isEdit(isEdit),
    discipline_id(discipline_id),
    ui(new Ui::addoredit_knigi)
{
    ui->setupUi(this);
    ui->lineEdit_god->setDate(QDate::currentDate());
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    if(this->discipline_id == -1)// если добавляем через форму metodichki
    {
        ui->comboBox_discipline->setModel(dal_prepodcontrol->getDisciplines());
        ui->comboBox_discipline->setModelColumn(1);
        ui->comboBox_discipline->setCurrentIndex(-1);
    }
    else // иначе идет добавление на дисциплин где этот комбобокс не нужен, так как дисциплина была уже выбрана (this->discipline_id)
    {
        ui->comboBox_discipline->setVisible(false);
        ui->label->setVisible(false);
    }
    ui->comboBox_sotr->setModel(dal_prepodcontrol->getcomboPrepod());
    ui->comboBox_sotr->setModelColumn(1);
    ui->comboBox_sotr->setCurrentIndex(-1);
    ui->label_error->setVisible(false);
    if(this->isEdit)
    {
        this->setWindowTitle("Редактирование записи");
        QSqlQuery *query = new QSqlQuery;
        query = dal_prepodcontrol->getCurrentKniga(this->id_knigi);
        if(this->discipline_id == -1)// если добавляем через форму metodichki
        {
            for (int j = 0; j < ui->comboBox_discipline->count(); j++)
            {
                if (ui->comboBox_discipline->model()->index(j, 0).data().toInt() == query->value(6).toInt())
                {
                    ui->comboBox_discipline->setCurrentIndex(j);
                    ui->comboBox_discipline->view()->setCurrentIndex(ui->comboBox_discipline->model()->index(j, 0));
                    break;
                }
            }
        }
        for (int j = 0; j < ui->comboBox_sotr->count(); j++)
        {
            if (ui->comboBox_sotr->model()->index(j, 0).data().toInt() == query->value(7).toInt())
            {
                ui->comboBox_sotr->setCurrentIndex(j);
                ui->comboBox_sotr->view()->setCurrentIndex(ui->comboBox_sotr->model()->index(j, 0));
                break;
            }
        }
        ui->lineEdit_nazv->setText(query->value(1).toString());
        ui->lineEdit_avtor->setText(query->value(2).toString());
        ui->lineEdit_god->setDate(query->value(3).toDate());
        ui->lineEdit_izdatel->setText(query->value(4).toString());
        ui->lineEdit_file->setText(query->value(5).toString());
    }
}

addoredit_knigi::~addoredit_knigi()
{
    delete ui;
}

void addoredit_knigi::on_pushButton_cancel_clicked()
{
    this->close();
}

void addoredit_knigi::on_pushButton_ok_clicked()
{
    if (ui->lineEdit_nazv->text().isEmpty() || (this->discipline_id == -1 && ui->comboBox_discipline->currentIndex()==-1))
    {
        ui->label_error->setVisible(true);
        return;
    }
    else if(ui->comboBox_sotr->currentIndex()==-1 && ui->lineEdit_avtor->text().isEmpty())
    {
        QMessageBox::information(this, tr("Информация"), tr("Заполните поле 'Выберите автора с кафедры' или поле 'Введите автора'"));
        return;
    }
    else
    {
        if(this->discipline_id == -1) // если добавляем через форму metodichki
            this->discipline_id = ui->comboBox_discipline->model()->index(ui->comboBox_discipline->currentIndex(),0).data().toInt();
        if (! this->isEdit)
        {
            if (! dal_main->checkConnection())
            {
                QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
                return;
            }
//            QFileInfo fi(this->UrlFail);
//            QString names = fi.fileName();
//            QFile::copy(this->UrlFail, "C:\\Dropbox\\" + names);
//            this->UrlFail = "C:\\Dropbox\\" + names;

            if (dal_prepodcontrol->addKniga(this->discipline_id,
                                            ui->lineEdit_nazv->text(),
                                            ui->lineEdit_avtor->text(),
                                            ui->lineEdit_file->text(),
                                            ui->lineEdit_god->date(),
                                            ui->lineEdit_izdatel->text(),
                                            ui->comboBox_sotr->model()->index(ui->comboBox_sotr->currentIndex(),0).data().toInt()))
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
            if (! dal_main->checkConnection())
            {
                QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
                return;
            }
            if (dal_prepodcontrol->editKniga(this->id_knigi,
                                             this->discipline_id,//ui->comboBox_discipline->model()->index(ui->comboBox_discipline->currentIndex(),0).data().toInt(),
                                             ui->lineEdit_nazv->text(),
                                             ui->lineEdit_avtor->text(),
                                             ui->lineEdit_file->text(),
                                             ui->lineEdit_god->date(),
                                             ui->lineEdit_izdatel->text(),
                                             ui->comboBox_sotr->model()->index(ui->comboBox_sotr->currentIndex(),0).data().toInt()))
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
}



void addoredit_knigi::on_pushButton_choose_clicked()
{
//    this->NameFails = "";
//    QString filename = QFileDialog::getOpenFileName(this,tr("open file"),"/home","All files(*.*)");
//    filename = filename.replace("/" , "\\");

//    this->UrlFail = filename;

//    QString name =  QFileInfo(filename).baseName();
//    ui->lineEdit_file->setText(name);
//    this->NameFails = name;
}
