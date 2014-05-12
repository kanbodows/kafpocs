#include "addoreditstat.h"
#include "ui_addoreditstat.h"
#include<QMessageBox>

AddOrEditStat::AddOrEditStat(QWidget *parent, int select, int id) :
    QDialog(parent),
    ui(new Ui::AddOrEditStat)
{
    ui->setupUi(this);
    this->id_avtor=0;
    this->id_soavtors=0;
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    this->isEdit = false;
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    QSqlQueryModel *comboAvtor = new QSqlQueryModel(this);
    comboAvtor = dal_prepodcontrol->getComboAvtor();



    ui->dateEdit->setDate(QDate::currentDate());

    ui->comboBox_avtor->setModel(comboAvtor);
    ui->comboBox_avtor->setModelColumn(1);
    ui->comboBox_avtor->setCurrentIndex(-1);

    ui->label_error->setVisible(false);

    QSqlQueryModel *comboSoAvtorModel = new QSqlQueryModel(this);
    comboSoAvtorModel = dal_prepodcontrol->getComboSoAvtor(this->id_avtor);
    ui->comboBox_soAvtor->setModel(comboSoAvtorModel);
    ui->comboBox_soAvtor->setModelColumn(1);
    ui->comboBox_soAvtor->setCurrentIndex(-1);

    if (select == 1)
    {
        this->setWindowTitle(tr("Добавить новую статью"));
    }
    else
    {
        this->isEdit = true;
        this->setWindowTitle(tr("Редактирование записи"));
        this->rec_id = id;
        int ids_avtor;
        int ids_soavtor;
        QSqlQuery *query = new QSqlQuery;
        query = dal_prepodcontrol->getCurrentStatia(this->rec_id);
        ids_avtor = query->value(1).toInt();
        ids_soavtor = query->value(2).toInt();
        ui->dateEdit->setDate(query->value(4).toDate());
        ui->plainTextEdit_tema->setPlainText(query->value(3).toString());
        for (int i = 0; i < ui->comboBox_avtor->count(); i ++)
        {
            if (comboAvtor->index(i, 0).data().toInt() == ids_avtor)
            {
                ui->comboBox_avtor->setCurrentIndex(i);
                ui->comboBox_avtor->view()->setCurrentIndex(comboAvtor->index(i, 0));
                break;
            }
        }
        for (int i = 0; i < ui->comboBox_soAvtor->count(); i ++)
        {
            if (comboSoAvtorModel->index(i, 0).data().toInt() == ids_soavtor)
            {
                ui->comboBox_soAvtor->setCurrentIndex(i);
                ui->comboBox_soAvtor->view()->setCurrentIndex(comboSoAvtorModel->index(i, 0));
                break;
            }
        }
    }
}

AddOrEditStat::~AddOrEditStat()
{
    delete ui;
}

void AddOrEditStat::on_pushButton_add_clicked()
{
    bool error;
    if (ui->comboBox_avtor->currentIndex()==-1)
    {
        error = true;
    }
    else
    {
        error = false;
    }
    if (ui->plainTextEdit_tema->toPlainText().isEmpty())
    {
        error = true;
    }
    else
    {
        error = false;
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
            if (dal_prepodcontrol->getStatiaAdd(ui->comboBox_avtor->model()->index(ui->comboBox_avtor->currentIndex(),0).data().toInt(), ui->comboBox_soAvtor->model()->index(ui->comboBox_soAvtor->currentIndex(), 0).data().toInt(), ui->plainTextEdit_tema->toPlainText(),ui->dateEdit->date()))
            {
                this->close();
                QMessageBox::information(this, tr("Информация"), tr("Статья успешно добавлена"));
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
            this->id_soavtors = ui->comboBox_soAvtor->model()->index(ui->comboBox_soAvtor->currentIndex(), 0).data().toInt();
            if (dal_prepodcontrol->editStat(this->rec_id, ui->comboBox_avtor->model()->index(ui->comboBox_avtor->currentIndex(),0).data().toInt(), this->id_soavtors, ui->plainTextEdit_tema->toPlainText(), ui->dateEdit->date()))
            {
                this->close();
                QMessageBox::information(this, tr("Информация"), tr("Данные успешно отредактированы"));
            }
            else
            {
                this->close();
                QMessageBox::warning(this, tr("Ошибка"), tr("Данные не были изменены"));
            }
        }

    }
}

void AddOrEditStat::on_comboBox_avtor_currentIndexChanged(int index)
{
    this->id_avtor = ui->comboBox_avtor->model()->index(index,0).data().toInt();

    QSqlQueryModel *comboSoAvtorModel = new QSqlQueryModel(this);
    comboSoAvtorModel = dal_prepodcontrol->getComboSoAvtor(this->id_avtor);
    ui->comboBox_soAvtor->setModel(comboSoAvtorModel);
    ui->comboBox_soAvtor->setModelColumn(1);
    ui->comboBox_soAvtor->setCurrentIndex(-1);
    this->id_soavtors = 0;
}

void AddOrEditStat::on_comboBox_soAvtor_activated(int index)
{
    if(ui->comboBox_avtor->currentIndex()==-1)
    {
        QMessageBox::information(this, tr("Информация"), tr("Вначале выберите автора"));
        ui->comboBox_soAvtor->setCurrentIndex(-1);
        return;
    }
    this->id_soavtors = ui->comboBox_soAvtor->model()->index(ui->comboBox_soAvtor->currentIndex(), 0).data().toInt();
}

void AddOrEditStat::on_pushButton_cancel_clicked()
{
    this->close();
    if (this->isEdit)
    {
        QMessageBox::information(this, tr("Информация"), tr("Статья не была изменена"));
    }
    else
    {
        QMessageBox::information(this, tr("Информация"), tr("Статья не добавлена"));
    }
}
