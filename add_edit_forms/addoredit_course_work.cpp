#include "addoredit_course_work.h"
#include "ui_addoredit_course_work.h"

addoredit_course_work::addoredit_course_work(QWidget *parent, bool isEdit, int id_course_work) :
    QDialog(parent),
    id_course_work(id_course_work),
    isEdit(isEdit),
    discipline_id(discipline_id),
    ui(new Ui::addoredit_course_work)
{
    ui->setupUi(this);
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    dal_studentsControl = new Dal_studentsControl(this);
    ui->data_sdachi->setDate(QDate::currentDate());
    ui->comboBox_discipline->setModel(dal_studentsControl->getDiscipl());
    ui->comboBox_discipline->setModelColumn(1);
    ui->comboBox_discipline->setCurrentIndex(-1);
    ui->comboBox_sotr->setModel(dal_prepodcontrol->getcomboPrepod());
    ui->comboBox_sotr->setModelColumn(1);
    ui->comboBox_sotr->setCurrentIndex(-1);
    ui->comboBox_gruppa->setModel(dal_studentsControl->getComboGroup(0));
    ui->comboBox_gruppa->setModelColumn(1);
    ui->comboBox_gruppa->setCurrentIndex(-1);
    ui->label_error->setVisible(false);
    if(this->isEdit)
    {
        this->setWindowTitle("Редактирование записи");
        QSqlQuery *query = new QSqlQuery;
        query = dal_studentsControl->getCurrentCourseWork(this->id_course_work);
        for (int j = 0; j < ui->comboBox_discipline->count(); j++)
        {
            if (ui->comboBox_discipline->model()->index(j, 0).data().toInt() == query->value(1).toInt())
            {
                ui->comboBox_discipline->setCurrentIndex(j);
                ui->comboBox_discipline->view()->setCurrentIndex(ui->comboBox_discipline->model()->index(j, 0));
                break;
            }
        }
        for (int j = 0; j < ui->comboBox_sotr->count(); j++)
        {
            if (ui->comboBox_sotr->model()->index(j, 0).data().toInt() == query->value(4).toInt())
            {
                ui->comboBox_sotr->setCurrentIndex(j);
                ui->comboBox_sotr->view()->setCurrentIndex(ui->comboBox_sotr->model()->index(j, 0));
                break;
            }
        }
        for (int i = 0; i < ui->comboBox_gruppa->count(); i++)
        {
            if (ui->comboBox_gruppa->model()->index(i, 0).data().toInt() == query->value(2).toInt())
            {
                ui->comboBox_gruppa->setCurrentIndex(i);
                ui->comboBox_gruppa->view()->setCurrentIndex(ui->comboBox_gruppa->model()->index(i, 0));
                break;
            }
        }
        ui->comboBox_student->setModel(dal_studentsControl->getStudentGroup(query->value(2).toInt()));
        ui->comboBox_student->setModelColumn(1);
        ui->comboBox_student->setCurrentIndex(-1);
        for (int i = 0; i < ui->comboBox_student->count(); i++)
        {
            if (ui->comboBox_student->model()->index(i, 0).data().toInt() == query->value(3).toInt())
            {
                ui->comboBox_student->setCurrentIndex(i);
                ui->comboBox_student->view()->setCurrentIndex(ui->comboBox_student->model()->index(i, 0));
                break;
            }
        }        
        ui->lineEdit_nazv->setText(query->value(9).toString());
        ui->lineEdit_ocenka->setText(query->value(11).toString());
//        ui->lineEdit_file->setText(query->value(10).toString());
        ui->spinBox_kolvo->setValue(query->value(13).toInt());
        ui->data_sdachi->setDate(query->value(12).toDate());
    }
}

addoredit_course_work::~addoredit_course_work()
{
    delete ui;
}

void addoredit_course_work::on_pushButton_cancel_clicked()
{
    this->close();
}

void addoredit_course_work::on_pushButton_ok_clicked()
{
    if (ui->comboBox_gruppa->currentIndex()==-1 || ui->comboBox_sotr->currentIndex()==-1 || ui->comboBox_student->currentIndex()==-1 || ui->comboBox_discipline->currentIndex()==-1 || ui->lineEdit_nazv->text().isEmpty())
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
        if (dal_studentsControl->addCourseWork(ui->comboBox_discipline->model()->index(ui->comboBox_discipline->currentIndex(),0).data().toInt(),
                                               ui->comboBox_student->model()->index(ui->comboBox_student->currentIndex(),0).data().toInt(),
                                               ui->comboBox_sotr->model()->index(ui->comboBox_sotr->currentIndex(),0).data().toInt(),
                                               ui->lineEdit_nazv->text(),
                                               "",
                                               ui->lineEdit_ocenka->text().toInt(),
                                               ui->data_sdachi->date(),
                                               ui->spinBox_kolvo->value()))
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
        if (dal_studentsControl->editCourseWork(this->id_course_work,
                                                ui->comboBox_discipline->model()->index(ui->comboBox_discipline->currentIndex(),0).data().toInt(),
                                                ui->comboBox_student->model()->index(ui->comboBox_student->currentIndex(),0).data().toInt(),
                                                ui->comboBox_sotr->model()->index(ui->comboBox_sotr->currentIndex(),0).data().toInt(),
                                                ui->lineEdit_nazv->text(),
                                                "",
                                                ui->lineEdit_ocenka->text().toInt(),
                                                ui->data_sdachi->date(),
                                                ui->spinBox_kolvo->value()))
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

void addoredit_course_work::on_comboBox_gruppa_currentIndexChanged(int index)
{
    ui->comboBox_student->setModel(dal_studentsControl->getStudentGroup(ui->comboBox_gruppa->model()->index(ui->comboBox_gruppa->currentIndex(),0).data().toInt()));
    ui->comboBox_student->setModelColumn(1);
    ui->comboBox_student->setCurrentIndex(-1);
}
