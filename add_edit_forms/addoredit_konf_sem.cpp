#include "addoredit_konf_sem.h"
#include "ui_addoredit_konf_sem.h"

addoredit_konf_sem::addoredit_konf_sem(QWidget *parent, bool isEdit, int id_konf) :
    QDialog(parent),
    id_konf(id_konf),
    isEdit(isEdit),
    ui(new Ui::addoredit_konf_sem)
{
    ui->setupUi(this);
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    dal_studentsControl = new Dal_studentsControl(this);
    ui->comboBox_group->setModel(dal_studentsControl->getComboGroup(0));
    ui->comboBox_group->setModelColumn(1);
    ui->comboBox_group->setCurrentIndex(-1);
    ui->label_error->setVisible(false);
    ui->lineEdit_mesto->setValidator( new QIntValidator(1, 1000000, this));
    ui->lineEdit_mesto->setPlaceholderText("Вводятся только цифры");
    if(! this->isEdit)
    {
        ui->dateEdit->setDate(QDate::currentDate());
    }
    else
    {
        this->setWindowTitle("Редактирование записи");
        QSqlQuery *query = new QSqlQuery;
        query = dal_studentsControl->getCurrentKonf_sem(this->id_konf);
        for (int j = 0; j < ui->comboBox_group->count(); j++)
        {
            if (ui->comboBox_group->model()->index(j, 0).data().toInt() == query->value(8).toInt())
            {
                ui->comboBox_group->setCurrentIndex(j);
                ui->comboBox_group->view()->setCurrentIndex(ui->comboBox_group->model()->index(j, 0));
                break;
            }
        }
        ui->comboBox_student->setModel(dal_studentsControl->getStudentGroup(query->value(8).toInt()));
        ui->comboBox_student->setModelColumn(1);
        ui->comboBox_student->setCurrentIndex(-1);
        for (int i = 0; i < ui->comboBox_student->count(); i++)
        {
            if (ui->comboBox_student->model()->index(i, 0).data().toInt() == query->value(9).toInt())
            {
                ui->comboBox_student->setCurrentIndex(i);
                ui->comboBox_student->view()->setCurrentIndex(ui->comboBox_student->model()->index(i, 0));
                break;
            }
        }
        ui->plainTextEdit_tema->setPlainText(query->value(3).toString());
        ui->lineEdit_comment->setText(query->value(4).toString());
        if(query->value(5).toInt()==0)
            ui->lineEdit_mesto->setText("");
        else
            ui->lineEdit_mesto->setText(query->value(5).toString());
        ui->lineEdit_sertifikat->setText(query->value(6).toString());
        ui->dateEdit->setDate(query->value(7).toDate());
    }
}

addoredit_konf_sem::~addoredit_konf_sem()
{
    delete ui;
}

void addoredit_konf_sem::on_comboBox_group_currentIndexChanged(int index)
{
    ui->comboBox_student->setModel(dal_studentsControl->getStudentGroup(ui->comboBox_group->model()->index(index,0).data().toInt()));
    ui->comboBox_student->setModelColumn(1);
    ui->comboBox_student->setCurrentIndex(-1);
}

void addoredit_konf_sem::on_pushButton_cancel_clicked()
{
    this->close();
}

void addoredit_konf_sem::on_pushButton_ok_clicked()
{
    bool error;
    if (ui->comboBox_group->currentIndex()==-1)
    {
        error = true;
    }
    else
    {
        error = false;
    }
    if (ui->comboBox_student->currentIndex()==-1)
    {
        error = true;
    }
    else
    {
        error = false;
    }
    if (error == true)
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
            if (dal_studentsControl->addKonfSem(ui->comboBox_student->model()->index(ui->comboBox_student->currentIndex(),0).data().toInt(),
                                                ui->plainTextEdit_tema->toPlainText(),
                                                ui->lineEdit_comment->text(),
                                                ui->lineEdit_mesto->text().toInt(),
                                                ui->lineEdit_sertifikat->text(),
                                                ui->dateEdit->date()))
            {

                QMessageBox::information(this, tr("Информация"), tr("Статья успешно добавлена"));
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
            if (dal_studentsControl->editKonfSem(this->id_konf,
                                                 ui->comboBox_student->model()->index(ui->comboBox_student->currentIndex(),0).data().toInt(),
                                                 ui->plainTextEdit_tema->toPlainText(),
                                                 ui->lineEdit_comment->text(),
                                                 ui->lineEdit_mesto->text().toInt(),
                                                 ui->lineEdit_sertifikat->text(),
                                                 ui->dateEdit->date()))
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


