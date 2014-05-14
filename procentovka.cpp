#include "procentovka.h"
#include "ui_procentovka.h"
#include <QMessageBox>
#include "delegates/word_wrap_delegate.h"

bool procentovka::isOpen = false;

procentovka::procentovka(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::procentovka)
{
    ui->setupUi(this);
    dal_studentsControl = new Dal_studentsControl(this);
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    if (! isOpen)
    {
        isOpen = true;
    }
    else
    {
        QMessageBox::warning(this, tr("Ошибка"), tr("Окно уже открыто"));
        throw std::exception();
    }
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        this->isOpen = false;
        throw std::exception();
    }
    ui->comboBox_rukovod->setModel(dal_prepodcontrol->getcomboPrepod());
    ui->comboBox_rukovod->setModelColumn(1);
    ui->comboBox_rukovod->setCurrentIndex(-1);
    this->rukovod = 0;
    ProcentkaModels = dal_studentsControl->getProcentka(this->rukovod);
    ui->tableViewProcentovka->setModel(ProcentkaModels);
    ui->tableViewProcentovka->setColumnHidden(0,true);
    ui->tableViewProcentovka->setColumnHidden(1,true);
    ui->tableViewProcentovka->setColumnHidden(2,true);
    ui->tableViewProcentovka->setColumnHidden(3,true);
    ui->tableViewProcentovka->setColumnHidden(4,true);
    ui->tableViewProcentovka->setColumnHidden(5,true);
    ui->tableViewProcentovka->setColumnHidden(6,true);
    ui->tableViewProcentovka->setColumnHidden(7,true);
    ui->tableViewProcentovka->setColumnHidden(8,true);
    ui->tableViewProcentovka->setColumnHidden(9,true);
    ui->tableViewProcentovka->setColumnHidden(10,true);
    ui->tableViewProcentovka->setColumnHidden(11,true);
    ui->tableViewProcentovka->setColumnHidden(13,true);
    ui->tableViewProcentovka->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableViewProcentovka->horizontalHeader()->setStretchLastSection(true);


    //    ui->tableViewPlObrazUslugi->addAction(ui->actionEdit);
    //    ui->tableViewPlObrazUslugi->addAction(ui->actionDelete);
}

procentovka::~procentovka()
{
    isOpen = false;
    delete ui;
}

void procentovka::refreshData()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableViewProcentovka->setModel(dal_studentsControl->getProcentka(this->rukovod));
}

void procentovka::on_comboBox_rukovod_activated(int index)
{
    this->rukovod = ui->comboBox_rukovod->model()->index(index,0).data().toInt();
    this->refreshData();
}

void procentovka::on_pushButton_add_clicked()
{
    if(ui->comboBox_rukovod->currentIndex()==-1)
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите руководителя"));
        return;
    }

    if(!ui->tableViewProcentovka->currentIndex().isValid())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите студента"));
        return;
    }

    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select * from is_procentovka where stud_id = " + QString::number(ui->tableViewProcentovka->model()->index(ui->tableViewProcentovka->currentIndex().row(),3).data().toInt()));
    query->exec();
    query->first();
    if(query->size()>0)
    {
        QMessageBox::information(this, tr("Информация"), tr("Данному студенту уже проставлены даты процентовок"));
        return;
    }

    int id = ui->tableViewProcentovka->model()->index(ui->tableViewProcentovka->currentIndex().row(),3).data().toInt();
    try
    {
        add_or_edit_procentovkaform = new add_or_edit_procentovka(this, INSERT, id);
        add_or_edit_procentovkaform->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}


void procentovka::on_pushButton_clicked()
{
    if(!ui->tableViewProcentovka->currentIndex().isValid())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите студента"));
        return;
    }
    int id = ui->tableViewProcentovka->model()->index(ui->tableViewProcentovka->currentIndex().row(),3).data().toInt();
    try
    {
        Prosmotr_redaktirovanie_procentovokForm = new Prosmotr_redaktirovanie_procentovok(this, id, true);
        Prosmotr_redaktirovanie_procentovokForm->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void procentovka::on_pushButton_edit_clicked()
{

    if(ui->comboBox_rukovod->currentIndex()==-1)
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите руководителя"));
        return;
    }


    if(!ui->tableViewProcentovka->currentIndex().isValid())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите студента"));
        return;
    }


    int id = ui->tableViewProcentovka->model()->index(ui->tableViewProcentovka->currentIndex().row(),3).data().toInt();
    try
    {
        Prosmotr_redaktirovanie_procentovokForm = new Prosmotr_redaktirovanie_procentovok(this, id, false);
        Prosmotr_redaktirovanie_procentovokForm->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}
