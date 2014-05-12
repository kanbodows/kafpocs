#include "addoreditboleznistud.h"
#include "ui_addoreditboleznistud.h"
#include <QMessageBox>

AddOrEditBolezniStud::AddOrEditBolezniStud(QWidget *parent, int select, int id) :
    QDialog(parent),
    ui(new Ui::AddOrEditBolezniStud)
{
    ui->setupUi(this);
    this->group_id=0;
    this->stud_id=0;
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    this->isEdit = false;
    dal_studentsControl = new Dal_studentsControl(this);
    QSqlQueryModel *comboGroup = new QSqlQueryModel(this);
    comboGroup = dal_studentsControl->getComboGroup(0);
    ui->dateEditZabolel->setDate(QDate::currentDate());
    ui->dateEdit_Vizdoravil->setDate(QDate::currentDate());
    ui->comboBoxGroups->setModel(comboGroup);
    ui->comboBoxGroups->setModelColumn(1);
    ui->comboBoxGroups->setCurrentIndex(-1);
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
        int ids_student;


        QSqlQuery *query = new QSqlQuery;
        query = dal_studentsControl->CurrentBoleznStud(this->rec_id);
        ids_groups = query->value(1).toInt();
        ids_student = query->value(2).toInt();

        this->stud_id = ids_student;
        this->group_id = ids_groups;

        QSqlQueryModel *comboStudent = new QSqlQueryModel(this);
        comboStudent = dal_studentsControl->getStudentGroup(ids_groups);
        ui->comboBox_Stud->setModel(comboStudent);
        ui->comboBox_Stud->setModelColumn(1);
        ui->comboBox_Stud->setCurrentIndex(-1);
        ui->lineEditBolezn->setText(query->value(6).toString());
        ui->dateEditZabolel->setDate(query->value(7).toDate());
        ui->dateEdit_Vizdoravil->setDate(query->value(8).toDate());

        for (int j = 0; j < ui->comboBoxGroups->count(); j ++)
        {
            if (comboGroup->index(j, 0).data().toInt() == ids_groups)
            {
                ui->comboBoxGroups->setCurrentIndex(j);
                ui->comboBoxGroups->view()->setCurrentIndex(comboGroup->index(j, 0));
                break;
            }
        }
        for (int i = 0; i < ui->comboBox_Stud->count(); i ++)
        {
            if (comboStudent->index(i, 0).data().toInt() == ids_student)
            {
                ui->comboBox_Stud->setCurrentIndex(i);
                ui->comboBox_Stud->view()->setCurrentIndex(comboStudent->index(i, 0));
                break;
            }
        }
    }
}

AddOrEditBolezniStud::~AddOrEditBolezniStud()
{
    delete ui;
}

void AddOrEditBolezniStud::on_pushButtonOK_clicked()
{
    if(ui->dateEditZabolel->date()==ui->dateEdit_Vizdoravil->date())
    {
        QMessageBox::warning(this, tr("Ошибка"),tr("Дата заболевания и Дата выздоравлени\n    не должны быть одинаковы"));
        return;
    }

    if(ui->dateEditZabolel->date()>ui->dateEdit_Vizdoravil->date())
    {
        QMessageBox::warning(this, tr("Ошибка"),tr("Дата заболевания должна быть ниже Даты выздоравлени"));
        return;
    }

    bool error = false;
    if (ui->comboBoxGroups->currentIndex()==-1)
    {
        error = true;
    }

    if (ui->comboBox_Stud->currentIndex()==-1)
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
            if (dal_studentsControl->BolezniStudAdd(this->stud_id,\
                                                    ui->lineEditBolezn->text(),\
                                                    ui->dateEditZabolel->date(),\
                                                    ui->dateEdit_Vizdoravil->date()))
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
            if (dal_studentsControl->editBoleznStud(this->rec_id,\
                                                    this->stud_id,\
                                                    ui->lineEditBolezn->text(),\
                                                    ui->dateEditZabolel->date(),\
                                                    ui->dateEdit_Vizdoravil->date()))
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

void AddOrEditBolezniStud::on_comboBoxGroups_activated(int index)
{
    this->group_id = ui->comboBoxGroups->model()->index(ui->comboBoxGroups->currentIndex(),0).data().toInt();
    QSqlQueryModel *comboStudent = new QSqlQueryModel(this);
    comboStudent = dal_studentsControl->getStudentGroup(this->group_id);
    ui->comboBox_Stud->setModel(comboStudent);
    ui->comboBox_Stud->setModelColumn(1);
    ui->comboBox_Stud->setCurrentIndex(-1);
}

void AddOrEditBolezniStud::on_comboBox_Stud_activated(int index)
{
    this->stud_id = ui->comboBox_Stud->model()->index(ui->comboBox_Stud->currentIndex(),0).data().toInt();
}
