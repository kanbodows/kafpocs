#include "addoredit_student.h"
#include "ui_addoredit_student.h"
#include <QFileDialog>
#include <QDebug>
addoredit_student::addoredit_student(QWidget *parent, bool isEdit, int id_stud) :
    QDialog(parent),
    id_stud(id_stud),
    isEdit(isEdit),
    ui(new Ui::addoredit_student)
{
    ui->setupUi(this);
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    dal_studentsControl = new Dal_studentsControl(this);

    ui->comboBox_spec->setModel(dal_studentsControl->getSpec());
    ui->comboBox_spec->setModelColumn(1);
    ui->comboBox_spec->setCurrentIndex(-1);

    ui->comboBox_group->setEnabled(false);

    ui->comboBox_group->setModel(dal_studentsControl->getComboGroup(0));
    ui->comboBox_group->setModelColumn(1);
    ui->comboBox_group->setCurrentIndex(-1);

    ui->comboBox_formaObuc->setModel(dal_studentsControl->getFormaObuch());
    ui->comboBox_formaObuc->setModelColumn(1);
    ui->comboBox_formaObuc->setCurrentIndex(-1);

    ui->comboBox_formaOplaty->setModel(dal_studentsControl->getFormaOplaty());
    ui->comboBox_formaOplaty->setModelColumn(1);
    ui->comboBox_formaOplaty->setCurrentIndex(-1);

    ui->label_error->setVisible(false);
    if(this->isEdit)
    {
        this->setWindowTitle("Редактирование записи");
        QSqlQuery *query = new QSqlQuery;
        query = dal_studentsControl->getCurrentStudent(this->id_stud);

//        this->id_spec = ui->comboBox_spec->model()->index(index,0).data().toInt();
//         ui->comboBox_group->setEnabled(true);
//         ui->comboBox_group->setModel(dal_studentsControl->getComboGroup(this->id_spec));
//         ui->comboBox_group->setModelColumn(1);
//         ui->comboBox_group->setCurrentIndex(-1);


        for (int j = 0; j < ui->comboBox_spec->count(); j++)
        {
            if (ui->comboBox_spec->model()->index(j, 0).data().toInt() == query->value(12).toInt())
            {
                ui->comboBox_spec->setCurrentIndex(j);
                ui->comboBox_spec->view()->setCurrentIndex(ui->comboBox_spec->model()->index(j, 0));
                break;
            }
        }


        for (int j = 0; j < ui->comboBox_group->count(); j++)
        {
            if (ui->comboBox_group->model()->index(j, 0).data().toInt() == query->value(1).toInt())
            {
                ui->comboBox_group->setCurrentIndex(j);
                ui->comboBox_group->view()->setCurrentIndex(ui->comboBox_group->model()->index(j, 0));
                break;
            }
        }

        for (int j = 0; j < ui->comboBox_formaObuc->count(); j++)
        {
            if (ui->comboBox_formaObuc->model()->index(j, 0).data().toInt() == query->value(2).toInt())
            {
                ui->comboBox_formaObuc->setCurrentIndex(j);
                ui->comboBox_formaObuc->view()->setCurrentIndex(ui->comboBox_formaObuc->model()->index(j, 0));
                break;
            }
        }

        for (int j = 0; j < ui->comboBox_formaOplaty->count(); j++)
        {
            if (ui->comboBox_formaOplaty->model()->index(j, 0).data().toInt() == query->value(3).toInt())
            {
                ui->comboBox_formaOplaty->setCurrentIndex(j);
                ui->comboBox_formaOplaty->view()->setCurrentIndex(ui->comboBox_formaOplaty->model()->index(j, 0));
                break;
            }
        }
        ui->fio_lineEdit->setText(query->value(4).toString());
        ui->dateEdit_roj->setDate(query->value(9).toDate());
        ui->dateEdit_reg->setDate(query->value(13).toDate());
        ui->pasport_lineEdit->setText(query->value(14).toString());
        ui->tel_lineEdit->setText(query->value(10).toString());
        ui->rodTel_lineEdit->setText(query->value(15).toString());
        ui->email_lineEdit->setText(query->value(11).toString());
        ui->rodEmail_lineEdit->setText(query->value(16).toString());
        ui->adresProj_lineEdit->setText(query->value(17).toString());
        ui->adresProp_lineEdit->setText(query->value(18).toString());
        ui->shkola_lineEdit->setText(query->value(24).toString());
        ui->skidka_doubleSpinBox->setValue(query->value(19).toDouble());
        ui->otchislen_checkBox->setChecked(query->value(20).toBool());
        ui->label_foto->setPixmap(query->value(21).toString());
        ui->starosta->setChecked(query->value(22).toBool());
        ui->dateEdit_dataPost->setDate(query->value(23).toDate());
        this->foto = query->value(21).toString();
        ui->label_foto->setScaledContents(true);
    }
}

addoredit_student::~addoredit_student()
{
    delete ui;
}

void addoredit_student::on_pushButton_ok_clicked()
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
    if (ui->comboBox_formaObuc->currentIndex()==-1)
    {
        error = true;
    }
    else
    {
        error = false;
    }
    if (ui->comboBox_formaOplaty->currentIndex()==-1)
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
            QFileInfo fi(this->foto);
            QString name = fi.fileName();
            QFile::copy(this->foto, "C:\\Dropbox\\" + name);
            this->foto = "C:\\Dropbox\\" + name;
            if (dal_studentsControl->addStudent(ui->fio_lineEdit->text(),
                                                ui->comboBox_group->model()->index(ui->comboBox_group->currentIndex(),0).data().toInt(),
                                                ui->dateEdit_roj->date(),
                                                ui->dateEdit_reg->date(),
                                                ui->pasport_lineEdit->text(),
                                                ui->tel_lineEdit->text(),
                                                ui->rodTel_lineEdit->text(),
                                                ui->email_lineEdit->text(),
                                                ui->rodEmail_lineEdit->text(),
                                                ui->adresProj_lineEdit->text(),
                                                ui->adresProp_lineEdit->text(),
                                                ui->shkola_lineEdit->text(),
                                                ui->comboBox_formaObuc->model()->index(ui->comboBox_formaObuc->currentIndex(),0).data().toInt(),
                                                ui->skidka_doubleSpinBox->value(),
                                                ui->comboBox_formaOplaty->model()->index(ui->comboBox_formaOplaty->currentIndex(),0).data().toInt(),
                                                ui->otchislen_checkBox->isChecked(),
                                                this->foto,
                                                ui->starosta->isChecked(),
                                                ui->dateEdit_dataPost->date()
                                                ))
            {

                QMessageBox::information(this, tr("Информация"), tr("Студент успешно добавлен"));
            }
            else
            {
                QMessageBox::warning(this, tr("Ошибка"), tr("Данные не были добавлены в базу данных"));
                return;
            }
        }

        else
        {
            if (! dal_main->checkConnection())
            {
                QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
                return;
            }
            QFileInfo fi(this->foto);
            QString name = fi.fileName();
            QFile::copy(this->foto, "C:\\Dropbox\\" + name);
            this->foto = "C:\\Dropbox\\" + name;
            if (dal_studentsControl->editStudent(id_stud,
                                                 ui->fio_lineEdit->text(),
                                                 ui->comboBox_group->model()->index(ui->comboBox_group->currentIndex(),0).data().toInt(),
                                                 ui->dateEdit_roj->date(),
                                                 ui->dateEdit_reg->date(),
                                                 ui->pasport_lineEdit->text(),
                                                 ui->tel_lineEdit->text(),
                                                 ui->rodTel_lineEdit->text(),
                                                 ui->email_lineEdit->text(),
                                                 ui->rodEmail_lineEdit->text(),
                                                 ui->adresProj_lineEdit->text(),
                                                 ui->adresProp_lineEdit->text(),
                                                 ui->shkola_lineEdit->text(),
                                                 ui->comboBox_formaObuc->model()->index(ui->comboBox_formaObuc->currentIndex(),0).data().toInt(),
                                                 ui->skidka_doubleSpinBox->value(),
                                                 ui->comboBox_formaOplaty->model()->index(ui->comboBox_formaOplaty->currentIndex(),0).data().toInt(),
                                                 ui->otchislen_checkBox->checkState(),
                                                 this->foto,
                                                 ui->starosta->isChecked(),
                                                 ui->dateEdit_dataPost->date()
                                                ))
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

void addoredit_student::on_pushButton_choosePhoto_clicked()
{
    this->foto = "";
    QString filename = QFileDialog::getOpenFileName(this,tr("open foto"),"/home","Image files(*.png *.jpg *.bmp)");
    filename = filename.replace("/" , "\\");
    ui->label_foto->setPixmap(filename);
    ui->label_foto->setScaledContents(true);
    this->foto = filename;
    if(this->foto!="")
        ui->label_foto->setText("");
}

void addoredit_student::on_pushButton_cancel_clicked()
{
    this->close();
}

void addoredit_student::on_comboBox_spec_activated(int index)
{
   this->id_spec = ui->comboBox_spec->model()->index(index,0).data().toInt();
    ui->comboBox_group->setEnabled(true);
    ui->comboBox_group->setModel(dal_studentsControl->getComboGroup(this->id_spec));
    ui->comboBox_group->setModelColumn(1);
    ui->comboBox_group->setCurrentIndex(-1);

}
