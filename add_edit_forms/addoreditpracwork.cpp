#include "addoreditpracwork.h"
#include "ui_addoreditpracwork.h"
#include<QMessageBox>

AddOrEditPracWork::AddOrEditPracWork(QWidget *parent, int select, int id, int viewsv) :
    QDialog(parent),
    ui(new Ui::AddOrEditPracWork)
{
    ui->setupUi(this);
    ui->lineEdit_ocenka->setValidator( new QIntValidator(1, 100, this));
    ui->lineEdit_ocenka->setPlaceholderText("Вводятся только цифры");
    this->stud_id=0;
    this->group_id=0;
    this->type_prac = 0;
    this->semmestrd = 0;
    this->rucKafedr = 0;
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    this->isEdit = false;
    dal_studentsControl = new Dal_studentsControl(this);

    QSqlQueryModel *comboGroup = new QSqlQueryModel(this);
    comboGroup = dal_studentsControl->getComboGroup(0);
    ui->dateEdit->setDate(QDate::currentDate());
    ui->comboBox_grupa->setModel(comboGroup);
    ui->comboBox_grupa->setModelColumn(1);
    ui->comboBox_grupa->setCurrentIndex(-1);
    ui->label_error->setVisible(false);

    QSqlQueryModel *combotypepr = new QSqlQueryModel(this);
    combotypepr = dal_studentsControl->getComboTypePrac();
    ui->comboBox_typePrac->setModel(combotypepr);
    ui->comboBox_typePrac->setModelColumn(1);
    ui->comboBox_typePrac->setCurrentIndex(-1);

    QSqlQueryModel *comboRucPrak = new QSqlQueryModel(this);
    comboRucPrak = dal_studentsControl->getcomborucPrac();
    ui->comboBox_rucKaf->setModel(comboRucPrak);
    ui->comboBox_rucKaf->setModelColumn(1);
    ui->comboBox_rucKaf->setCurrentIndex(-1);

    QSqlQueryModel *comboSemestr = new QSqlQueryModel(this);
    comboSemestr = dal_studentsControl->getcomboSem();
    ui->comboBox_semestr->setModel(comboSemestr);
    ui->comboBox_semestr->setModelColumn(2);
    ui->comboBox_semestr->setCurrentIndex(-1);

    QSqlQueryModel *comboStudent = new QSqlQueryModel(this);
    comboStudent = dal_studentsControl->getStudentGroup(this->group_id);
    ui->comboBox_student->setModel(comboStudent);
    ui->comboBox_student->setModelColumn(1);
    ui->comboBox_student->setCurrentIndex(-1);


    if (select == 1)
    {
        this->setWindowTitle(tr("Добавить новую запись"));
    }

    else
    {
        this->isEdit = true;
        if(viewsv==3)
        {
            this->setWindowTitle(tr("Подробная информация о практике"));
            ui->label_group->setText("Группа");
            ui->comboBox_grupa->setEnabled(false);
            ui->label_student->setText("Студент");
            ui->comboBox_student->setEnabled(false);
            ui->label_rukKaf->setText("Руководитель от кафедры");
            ui->comboBox_rucKaf->setEnabled(false);
            ui->label_semestr->setText("Семестр");
            ui->comboBox_semestr->setEnabled(false);
            ui->label_tip_prac->setText("Тип практики");
            ui->comboBox_typePrac->setEnabled(false);
            ui->label_nomer_prikaza->setText("Номер приказа");
            ui->lineEdit_nomPrikaza->setReadOnly(true);
            ui->label_MestoProhoj->setText("Место прохождения");
            ui->lineEdit_MestoProhoj->setReadOnly(true);
            ui->label_Ocenka->setText("Оценка");
            ui->lineEdit_ocenka->setReadOnly(true);
            ui->label_rucPred->setText("Руводитель от предприятия");
            ui->lineEdit_rukPredpr->setReadOnly(true);
            ui->label_otziv->setText("Отзыв");
            ui->plainTextEdit_otziv->setReadOnly(true);
            ui->label_datas->setText("Дата");
            ui->dateEdit->setReadOnly(true);
        }
        else
            this->setWindowTitle(tr("Редактирование записи"));
        this->rec_id = id;
        int ids_groups;
        int ids_student;
        int ids_typePr;
        int ids_semes;
        int ids_rukKaf;

        QSqlQuery *query = new QSqlQuery;
        query = dal_studentsControl->getCurrentPracWork(this->rec_id);

        ids_groups = query->value(4).toInt();
        ids_student = query->value(16).toInt();
        ids_rukKaf = query->value(2).toInt();
        ids_semes = query->value(3).toInt();
        ids_typePr = query->value(1).toInt();


        comboStudent = dal_studentsControl->getStudentGroup(ids_groups);
        ui->comboBox_student->setModel(comboStudent);
        ui->comboBox_student->setModelColumn(1);
        ui->comboBox_student->setCurrentIndex(-1);

        ui->lineEdit_rukPredpr->setText(query->value(10).toString());
        ui->lineEdit_MestoProhoj->setText(query->value(11).toString());
        ui->lineEdit_ocenka->setText(query->value(12).toString());
        ui->plainTextEdit_otziv->setPlainText(query->value(13).toString());
        ui->lineEdit_nomPrikaza->setText(query->value(14).toString());
        ui->dateEdit->setDate(query->value(15).toDate());

        for (int j = 0; j < ui->comboBox_grupa->count(); j ++)
        {
            if (comboGroup->index(j, 0).data().toInt() == ids_groups)
            {
                ui->comboBox_grupa->setCurrentIndex(j);
                ui->comboBox_grupa->view()->setCurrentIndex(comboGroup->index(j, 0));
                break;
            }
        }
        for (int i = 0; i < ui->comboBox_student->count(); i ++)
        {
            if (comboStudent->index(i, 0).data().toInt() == ids_student)
            {
                ui->comboBox_student->setCurrentIndex(i);
                ui->comboBox_student->view()->setCurrentIndex(comboStudent->index(i, 0));
                break;
            }
        }

        for (int i = 0; i < ui->comboBox_rucKaf->count(); i ++)
        {
            if (comboRucPrak->index(i, 0).data().toInt() == ids_rukKaf)
            {
                ui->comboBox_rucKaf->setCurrentIndex(i);
                ui->comboBox_rucKaf->view()->setCurrentIndex(comboRucPrak->index(i, 0));
                break;
            }
        }

        for (int i = 0; i < ui->comboBox_semestr->count(); i ++)
        {
            if (comboSemestr->index(i, 0).data().toInt() == ids_semes)
            {
                ui->comboBox_semestr->setCurrentIndex(i);
                ui->comboBox_semestr->view()->setCurrentIndex(comboSemestr->index(i, 0));
                break;
            }
        }

        for (int i = 0; i < ui->comboBox_typePrac->count(); i ++)
        {
            if (combotypepr->index(i, 0).data().toInt() == ids_typePr)
            {
                ui->comboBox_typePrac->setCurrentIndex(i);
                ui->comboBox_typePrac->view()->setCurrentIndex(combotypepr->index(i, 0));
                break;
            }
        }

    }
}

AddOrEditPracWork::~AddOrEditPracWork()
{
    delete ui;
}

void AddOrEditPracWork::on_comboBox_grupa_activated(int index)
{
    this->group_id = ui->comboBox_grupa->model()->index(ui->comboBox_grupa->currentIndex(),0).data().toInt();
    QSqlQueryModel *comboStudent = new QSqlQueryModel(this);
    comboStudent = dal_studentsControl->getStudentGroup(this->group_id);
    ui->comboBox_student->setModel(comboStudent);
    ui->comboBox_student->setModelColumn(1);
    ui->comboBox_student->setCurrentIndex(-1);
}

void AddOrEditPracWork::on_pushButton_ok_clicked()
{
    bool error = false;
    if (ui->comboBox_grupa->currentIndex()==-1)
    {
        error = true;
    }

    if (ui->comboBox_student->currentIndex()==-1)
    {
        error = true;
    }

    if (ui->comboBox_rucKaf->currentIndex()==-1)
    {
        error = true;
    }

    if (ui->comboBox_semestr->currentIndex()==-1)
    {
        error = true;
    }

    if (ui->comboBox_typePrac->currentIndex()==-1)
    {
        error = true;
    }



    if(ui->lineEdit_MestoProhoj->text().isEmpty())
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
            if (dal_studentsControl->getPracWorkAdd(ui->comboBox_typePrac->model()->index(ui->comboBox_typePrac->currentIndex(), 0).data().toInt(),ui->dateEdit->date(),ui->comboBox_semestr->model()->index(ui->comboBox_semestr->currentIndex(), 0).data().toInt(),ui->comboBox_student->model()->index(ui->comboBox_student->currentIndex(), 0).data().toInt(),ui->comboBox_rucKaf->model()->index(ui->comboBox_rucKaf->currentIndex(), 0).data().toInt(), ui->lineEdit_rukPredpr->text(),ui->lineEdit_MestoProhoj->text(),ui->lineEdit_ocenka->text(),ui->plainTextEdit_otziv->toPlainText(),ui->lineEdit_nomPrikaza->text()))
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
            if (dal_studentsControl->editPracWork(this->rec_id,\
                                                  ui->comboBox_typePrac->model()->index(ui->comboBox_typePrac->currentIndex(),0).data().toInt(),\
                                                  ui->dateEdit->date(),\
                                                  ui->comboBox_semestr->model()->index(ui->comboBox_semestr->currentIndex(),0).data().toInt(),\
                                                  ui->comboBox_student->model()->index(ui->comboBox_student->currentIndex(),0).data().toInt(),\
                                                  ui->comboBox_rucKaf->model()->index(ui->comboBox_rucKaf->currentIndex(),0).data().toInt(),\
                                                  ui->lineEdit_rukPredpr->text(),\
                                                  ui->lineEdit_MestoProhoj->text(),\
                                                  ui->lineEdit_ocenka->text(),\
                                                  ui->plainTextEdit_otziv->toPlainText(),\
                                                  ui->lineEdit_nomPrikaza->text()))
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

void AddOrEditPracWork::on_pushButton_cancel_clicked()
{
    this->close();
}
