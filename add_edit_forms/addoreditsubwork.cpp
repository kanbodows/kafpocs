#include "addoreditsubwork.h"
#include "ui_addoreditsubwork.h"

AddOrEditSubWork::AddOrEditSubWork(QWidget *parent, int select, int id) :
    QDialog(parent),
    ui(new Ui::AddOrEditSubWork)
{
    ui->setupUi(this);
      ui->label_error->setVisible(false);
    this->prepd_id=0;
    this->discp_id=0;
    this->sem_id = 0;
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    this->isEdit = false;

    dal_prepodcontrol = new Dal_prepodcontrol(this);

    QSqlQueryModel *comboPrepod = new QSqlQueryModel(this);
    comboPrepod = dal_prepodcontrol->getcomboPrepod();
    ui->comboBox_prepod->setModel(comboPrepod);
    ui->comboBox_prepod->setModelColumn(1);
    ui->comboBox_prepod->setCurrentIndex(-1);


    QSqlQueryModel *comboPredm = new QSqlQueryModel(this);
    comboPredm = dal_prepodcontrol->getcomboPredm();
    ui->comboBox_predm->setModel(comboPredm);
    ui->comboBox_predm->setModelColumn(1);
    ui->comboBox_predm->setCurrentIndex(-1);


    QSqlQueryModel *comboSemmestr = new QSqlQueryModel(this);
    comboSemmestr = dal_prepodcontrol->getcomboSemmestr();
    ui->comboBox_semestr->setModel(comboSemmestr);
    ui->comboBox_semestr->setModelColumn(2);
    ui->comboBox_semestr->setCurrentIndex(-1);


    if (select == 1)
    {
        this->sel=false;
        this->setWindowTitle(tr("Добавить новую запись"));
    }
   else
    {
        this->isEdit = true;
        this->sel=true;
        this->setWindowTitle(tr("Редактирование записи"));
        this->rec_id = id;
        int ids_prepod;
        int ids_discipl;
        int ids_semestr;


        QSqlQuery *query = new QSqlQuery;
        query = dal_prepodcontrol->getCurrentSubWork(this->rec_id);
        ids_prepod = query->value(3).toInt();
        ids_discipl = query->value(1).toInt();
        ids_semestr = query->value(2).toInt();
        ui->dateEdit->setDate(query->value(7).toDate());


        for (int j = 0; j < ui->comboBox_prepod->count(); j ++)
        {
            if (comboPrepod->index(j, 0).data().toInt() == ids_prepod)
            {
                ui->comboBox_prepod->setCurrentIndex(j);
                ui->comboBox_prepod->view()->setCurrentIndex(comboPrepod->index(j, 0));
                break;
            }
        }

        for (int i = 0; i < ui->comboBox_predm->count(); i ++)
        {
            if (comboPredm->index(i, 0).data().toInt() == ids_discipl)
            {
                ui->comboBox_predm->setCurrentIndex(i);
                ui->comboBox_predm->view()->setCurrentIndex(comboPredm->index(i, 0));
                break;
            }
        }

        for (int i = 0; i < ui->comboBox_semestr->count(); i ++)
        {
            if (comboSemmestr->index(i, 0).data().toInt() == ids_semestr)
            {
                ui->comboBox_semestr->setCurrentIndex(i);
                ui->comboBox_semestr->view()->setCurrentIndex(comboSemmestr->index(i, 0));
                break;
            }
        }
    }
}

AddOrEditSubWork::~AddOrEditSubWork()
{
    delete ui;
}

void AddOrEditSubWork::on_pushButton_ok_3_clicked()
{
    bool error = false;
    if (ui->comboBox_prepod->currentIndex()==-1)
    {
        error = true;
    }

    if (ui->comboBox_predm->currentIndex()==-1)
    {
        error = true;
    }

    if (ui->comboBox_semestr->currentIndex()==-1)
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
            if (dal_prepodcontrol->getSubWorkAdd(ui->comboBox_prepod->model()->index(ui->comboBox_prepod->currentIndex(), 0).data().toInt(), \
                                                 ui->comboBox_predm->model()->index(ui->comboBox_predm->currentIndex(), 0).data().toInt(), \
                                                 ui->comboBox_semestr->model()->index(ui->comboBox_semestr->currentIndex(), 0).data().toInt(),\
                                                 ui->dateEdit->date()))
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
            if (dal_prepodcontrol->editSubWork(this->rec_id,\
                                               ui->comboBox_predm->model()->index(ui->comboBox_predm->currentIndex(), 0).data().toInt(), \
                                               ui->comboBox_semestr->model()->index(ui->comboBox_semestr->currentIndex(), 0).data().toInt(),\
                                               ui->dateEdit->date(),\
                                               ui->comboBox_prepod->model()->index(ui->comboBox_prepod->currentIndex(), 0).data().toInt()))
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
