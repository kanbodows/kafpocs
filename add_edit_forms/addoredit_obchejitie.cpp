#include "addoredit_obchejitie.h"
#include "ui_addoredit_obchejitie.h"

addoredit_obchejitie::addoredit_obchejitie(QWidget *parent, bool isEdit, int id_obch) :
    QDialog(parent),
    id_obch(id_obch),
    isEdit(isEdit),
    ui(new Ui::addoredit_obchejitie)
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
    if(this->isEdit)
    {
        this->setWindowTitle("Редактирование записи");
        QSqlQuery *query = new QSqlQuery;
        query = dal_studentsControl->getCurrentObchejitie(this->id_obch);
        for (int j = 0; j < ui->comboBox_group->count(); j++)
        {
            if (ui->comboBox_group->model()->index(j, 0).data().toInt() == query->value(6).toInt())
            {
                ui->comboBox_group->setCurrentIndex(j);
                ui->comboBox_group->view()->setCurrentIndex(ui->comboBox_group->model()->index(j, 0));
                break;
            }
        }
        ui->comboBox_student->setModel(dal_studentsControl->getStudentGroup(query->value(6).toInt()));
        ui->comboBox_student->setModelColumn(1);
        ui->comboBox_student->setCurrentIndex(-1);
        for (int i = 0; i < ui->comboBox_student->count(); i++)
        {
            if (ui->comboBox_student->model()->index(i, 0).data().toInt() == query->value(7).toInt())
            {
                ui->comboBox_student->setCurrentIndex(i);
                ui->comboBox_student->view()->setCurrentIndex(ui->comboBox_student->model()->index(i, 0));
                break;
            }
        }
        ui->plainTextEdit_adres->setPlainText(query->value(3).toString());
        ui->doubleSpinBox_oplata->setValue(query->value(4).toDouble());
        ui->plainTextEdit_otziv->setPlainText(query->value(5).toString());
    }
}

addoredit_obchejitie::~addoredit_obchejitie()
{
    delete ui;
}

void addoredit_obchejitie::on_comboBox_group_currentIndexChanged(int index)
{
    ui->comboBox_student->setModel(dal_studentsControl->getStudentGroup(ui->comboBox_group->model()->index(index,0).data().toInt()));
    ui->comboBox_student->setModelColumn(1);
    ui->comboBox_student->setCurrentIndex(-1);
}

void addoredit_obchejitie::on_pushButton_cancel_clicked()
{
    this->close();
}

void addoredit_obchejitie::on_pushButton_ok_clicked()
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
            if (dal_studentsControl->addObchejitie(ui->comboBox_student->model()->index(ui->comboBox_student->currentIndex(),0).data().toInt(),
                                                   ui->plainTextEdit_adres->toPlainText(),
                                                   ui->doubleSpinBox_oplata->value(),
                                                   ui->plainTextEdit_otziv->toPlainText()))
            {

                QMessageBox::information(this, tr("Информация"), tr("Данные успешно добавлена"));
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
            if (dal_studentsControl->editObchejitie(this->id_obch,
                                                    ui->comboBox_student->model()->index(ui->comboBox_student->currentIndex(),0).data().toInt(),
                                                    ui->plainTextEdit_adres->toPlainText(),
                                                    ui->doubleSpinBox_oplata->value(),
                                                    ui->plainTextEdit_otziv->toPlainText()))
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


