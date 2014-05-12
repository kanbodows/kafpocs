#include "olimpiada.h"
#include "ui_olimpiada.h"
#include "delegates/datedelegate.h"
#include <QMessageBox>

bool Olimpiada::isOpen = false;
Olimpiada::Olimpiada(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Olimpiada)
{
    ui->setupUi(this);
    ui->lineEdit_zanMesto->setValidator( new QIntValidator(1, 1000000, this));
    ui->lineEdit_zanMesto->setPlaceholderText("Вводятся только цифры");
    this->count=0;
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
    this->vidim=false;
    dal_studentsControl = new Dal_studentsControl(this);
    this->Student="";
    this->Predmet="";
    this->ZanyatoeMesto=0;
    olimpModels = dal_studentsControl->getOlimpiada(this->Student, this->Predmet, this->ZanyatoeMesto);
    ui->tableView_olimp->setModel(olimpModels);
    ui->tableView_olimp->setColumnHidden(0,true);
    ui->tableView_olimp->setColumnHidden(1,true);
    ui->tableView_olimp->setColumnHidden(2,true);
    ui->tableView_olimp->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_olimp->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_olimp->setItemDelegateForColumn(6, new dateDelegate(ui->tableView_olimp));
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ui->tableView_olimp->addAction(ui->actionEdit);
    ui->tableView_olimp->addAction(ui->actionDelete);
}

Olimpiada::~Olimpiada()
{
    isOpen = false;
    delete ui;
}

void Olimpiada::on_pushButton_add_clicked()
{
    try
    {
        addoreditform = new AddOrEditOlimp(this, INSERT);
        addoreditform->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void Olimpiada::refreshData()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableView_olimp->setModel(dal_studentsControl->getOlimpiada(this->Student, this->Predmet, this->ZanyatoeMesto));
    if(this->vidim==true)
    {
        if(ui->tableView_olimp->model()->rowCount()>0)
        {
            this->naideno.append("    Найдено: " + QString::number(ui->tableView_olimp->model()->rowCount()));
            ui->label_result->setVisible(false);
            ui->label_naideno->setVisible(true);
            ui->label_naideno->setText(this->naideno);
            this->naideno.clear();
        }
        else
        {
            ui->label_naideno->setVisible(true);
            this->naideno.append("    Найдено: 0");
            ui->label_result->setVisible(true);
            ui->label_naideno->setText(this->naideno);
            this->naideno.clear();
        }
    }
}

void Olimpiada::on_pushButton_update_clicked()
{
    this->vidim=false;
    this->refreshData();
}

void Olimpiada::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        this->Student="";
        this->Predmet="";
        this->ZanyatoeMesto=0;
        ui->lineEdit_student->clear();
        ui->lineEdit_predmet->clear();
        ui->lineEdit_zanMesto->clear();
        this->refreshData();
    }
    else
    {
        ui->groupBox_search->setVisible(true);
    }
}

void Olimpiada::on_pushButton_find_clicked()
{
    this->vidim=true;
    this->Student = ui->lineEdit_student->text();
    this->Predmet = ui->lineEdit_predmet->text();
    this->ZanyatoeMesto = ui->lineEdit_zanMesto->text().toInt();
    this->refreshData();
}

void Olimpiada::on_pushButton_clear_clicked()
{
    this->Student="";
    this->Predmet="";
    this->ZanyatoeMesto=0;
    ui->lineEdit_student->clear();
    ui->lineEdit_predmet->clear();
    ui->lineEdit_zanMesto->clear();
    ui->label_naideno->setVisible(false);
    ui->label_result->setVisible(false);
    this->vidim=false;
    this->refreshData();
}

void Olimpiada::on_pushButton_edit_clicked()
{
    if (! ui->tableView_olimp->model()->index(ui->tableView_olimp->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите запись из таблицы"));
        return;
    }
    int id = ui->tableView_olimp->model()->index(ui->tableView_olimp->currentIndex().row(), 1).data().toInt();
    try
    {
        addoreditform = new AddOrEditOlimp(this, EDIT, id);
        addoreditform->exec();
    }
    catch (...)
    {
        return;
    }

    this->refreshData();
}

void Olimpiada::on_pushButton_del_clicked()
{
    if (! ui->tableView_olimp->model()->index(ui->tableView_olimp->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::warning(this, tr("Ошибка удаления"), tr("Ни одной записи не выбрано"));
        return;
    }
    if (QMessageBox::warning(this, tr("Удаление записи"), tr("Вы уверены, что хотите удалить запись? \n Восстановить запись невозможно"),
                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        if (! dal_main->checkConnection())
        {
            QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
            return;
        }
        if (dal_studentsControl->deleteOlimp(ui->tableView_olimp->model()->index(ui->tableView_olimp->currentIndex().row(), 1).data().toInt()))
        {
            this->refreshData();
            QMessageBox::information(this, tr("Удаление"), tr("Данные успешно удалены"));
        }
        else
        {
            QMessageBox::information(this, tr("Удаление"), tr("Не удалось удалить данные, попробуйте еще раз"));
            this->refreshData();
        }
    }
    else
    {
        this->refreshData();
        return;
    }
}

void Olimpiada::on_tableView_olimp_doubleClicked(const QModelIndex &index)
{
   on_pushButton_edit_clicked();
}

void Olimpiada::on_actionEdit_triggered()
{
    on_pushButton_edit_clicked();
}

void Olimpiada::on_actionDelete_triggered()
{
    on_pushButton_del_clicked();
}
