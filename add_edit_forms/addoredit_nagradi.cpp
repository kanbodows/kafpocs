#include "addoredit_nagradi.h"
#include "ui_addoredit_nagradi.h"

Addoredit_Nagradi::Addoredit_Nagradi(QWidget *parent, bool isEdit, int id_nagradi) :
    QDialog(parent),
    id_nagradi(id_nagradi),
    isEdit(isEdit),
    ui(new Ui::Addoredit_Nagradi)
{
    ui->setupUi(this);
    ui->frame->setVisible(false);
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    ui->comboBox_sotr->setModel(dal_prepodcontrol->getcomboPrepod());
    ui->comboBox_sotr->setModelColumn(1);
    ui->comboBox_sotr->setCurrentIndex(-1);
    ui->label_error->setVisible(false);
    ui->dateEdit->setDate(QDate::currentDate());
    if(this->isEdit)
    {
        this->setWindowTitle("Редактирование записи");
        QSqlQuery *query = new QSqlQuery;
        query = dal_prepodcontrol->getCurrentNagradi(this->id_nagradi);
        for (int j = 0; j < ui->comboBox_sotr->count(); j++)
        {
            if (ui->comboBox_sotr->model()->index(j, 0).data().toInt() == query->value(1).toInt())
            {
                ui->comboBox_sotr->setCurrentIndex(j);
                ui->comboBox_sotr->view()->setCurrentIndex(ui->comboBox_sotr->model()->index(j, 0));
                break;
            }
        }
        ui->lineEdit_nagrada->setText(query->value(2).toString());
        ui->dateEdit->setDate(query->value(3).toDate());
        ui->lineEdit_file->setText(query->value(4).toString());
    }
    this->adjustSize();
}

Addoredit_Nagradi::~Addoredit_Nagradi()
{
    delete ui;
}

void Addoredit_Nagradi::on_pushButton_cancel_clicked()
{
    this->close();
}

void Addoredit_Nagradi::on_pushButton_ok_clicked()
{
    if (ui->comboBox_sotr->currentIndex()==-1 || ui->lineEdit_nagrada->text().isEmpty())
    {
        ui->label_error->setVisible(true);
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
            if (dal_prepodcontrol->addNagradi(ui->comboBox_sotr->model()->index(ui->comboBox_sotr->currentIndex(),0).data().toInt(),
                                              ui->lineEdit_nagrada->text(),
                                              ui->dateEdit->date(),
                                              ui->lineEdit_file->text()))
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
            if (dal_prepodcontrol->editNagradi(this->id_nagradi,
                                               ui->comboBox_sotr->model()->index(ui->comboBox_sotr->currentIndex(),0).data().toInt(),
                                               ui->lineEdit_nagrada->text(),
                                               ui->dateEdit->date(),
                                               ui->lineEdit_file->text()))
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



void Addoredit_Nagradi::on_pushButton_choose_clicked()
{

}
