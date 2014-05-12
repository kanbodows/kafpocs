#include "boleznistudentov.h"
#include "ui_boleznistudentov.h"
#include <QMessageBox>
#include "delegates/datedelegate.h"
bool BolezniStudentov::isOpen = false;
BolezniStudentov::BolezniStudentov(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BolezniStudentov)
{
    ui->setupUi(this);
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
    this->id_groip = 0;
    this->bolezn = "";
    this->dateBeginning = QDate();
    this->dateEnd = ui->dateEdit_end->date();
    this->isDate = false;
    boleznistudModels = dal_studentsControl->getBolezniStud(this->id_groip, this->bolezn, this->dateBeginning, this->dateEnd);
    ui->tableView_studBolez->setModel(boleznistudModels);


    ui->tableView_studBolez->setColumnHidden(0,true);
    ui->tableView_studBolez->setColumnHidden(1,true);
    ui->tableView_studBolez->setColumnHidden(2,true);
    ui->tableView_studBolez->setColumnHidden(3,true);
    ui->tableView_studBolez->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_studBolez->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_studBolez->setItemDelegateForColumn(7, new dateDelegate(ui->tableView_studBolez));
    ui->tableView_studBolez->setItemDelegateForColumn(8, new dateDelegate(ui->tableView_studBolez));
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);

    ui->tableView_studBolez->addAction(ui->actionEdit);
    ui->tableView_studBolez->addAction(ui->actionDelete);
}

BolezniStudentov::~BolezniStudentov()
{
    this->isOpen = false;
    delete ui;
}

void BolezniStudentov::refreshData()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableView_studBolez->setModel(dal_studentsControl->getBolezniStud(this->id_groip, this->bolezn, this->dateBeginning, this->dateEnd));

            if(this->vidim==true)
    {
        if(ui->tableView_studBolez->model()->rowCount()>0)
        {
            this->naideno.append("                                   Найдено: " + QString::number(ui->tableView_studBolez->model()->rowCount()));
            ui->label_result->setVisible(false);
            ui->label_naideno->setVisible(true);
            ui->label_naideno->setText(this->naideno);
            this->naideno.clear();
        }
        else
        {
            ui->label_naideno->setVisible(true);
            this->naideno.append("                                   Найдено: 0");
            ui->label_result->setVisible(true);
            ui->label_naideno->setText(this->naideno);
            this->naideno.clear();
        }
    }
}

void BolezniStudentov::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        this->id_groip=0;
        this->bolezn="";
        this->dateBeginning = QDate();
        this->dateEnd = ui->dateEdit_end->date();
        this->isDate = false;
        ui->comboBox_groups->setCurrentIndex(-1);
        ui->lineEdit_bolezn->clear();
        this->refreshData();
    }
    else
    {
        ui->groupBox_search->setVisible(true);
        QSqlQueryModel *comboGroup = new QSqlQueryModel(this);
        comboGroup = dal_studentsControl->getComboGroup(0);
        ui->comboBox_groups->setModel(comboGroup);
        ui->comboBox_groups->setModelColumn(1);
        ui->comboBox_groups->setCurrentIndex(-1);
        ui->dateEdit_end->setDate(QDate::currentDate());
    }
    on_pushButton_clear_clicked();
}

void BolezniStudentov::on_pushButton_clear_clicked()
{
    this->dateBeginning = QDate();
    this->isDate = false;
    this->id_groip=0;
    this->bolezn="";
    ui->comboBox_groups->setCurrentIndex(-1);
    ui->lineEdit_bolezn->clear();
    ui->label_naideno->setVisible(false);
    ui->label_result->setVisible(false);
    this->vidim=false;
    this->refreshData();
}

void BolezniStudentov::on_pushButton_find_clicked()
{
    this->vidim = true;
    this->bolezn = ui->lineEdit_bolezn->text();
    if (this->isDate==true)
    {
        this->dateBeginning = ui->dateEdit_start->date();
        this->dateEnd = ui->dateEdit_end->date();
    }
    this->refreshData();
}

void BolezniStudentov::on_dateEdit_start_dateChanged(const QDate &date)
{
    this->isDate = true;
}

void BolezniStudentov::on_dateEdit_end_dateChanged(const QDate &date)
{
    this->isDate = true;
}

void BolezniStudentov::on_pushButton_del_clicked()
{
    if (! ui->tableView_studBolez->model()->index(ui->tableView_studBolez->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::information(this, tr("Информация"), tr("Ни одной записи не выбрано"));
        return;
    }
    if (QMessageBox::information(this, tr("Информация"), tr("Вы уверены, что хотите удалить запись? \n Восстановить запись невозможно"),
                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        if (! dal_main->checkConnection())
        {
            QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
            return;
        }
        if (dal_studentsControl->deleteBolezniSotr(ui->tableView_studBolez->model()->index(ui->tableView_studBolez->currentIndex().row(), 0).data().toInt()))
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

void BolezniStudentov::on_pushButton_update_clicked()
{
    this->vidim=false;
    this->refreshData();
}

void BolezniStudentov::on_comboBox_groups_activated(int index)
{
    this->id_groip = ui->comboBox_groups->model()->index(ui->comboBox_groups->currentIndex(),0).data().toInt();
}

void BolezniStudentov::on_pushButton_add_clicked()
{
    try
    {
        AddOrEditBolezniStudform = new AddOrEditBolezniStud(this, INSERT);
        AddOrEditBolezniStudform->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void BolezniStudentov::on_pushButton_edit_clicked()
{
    if (! ui->tableView_studBolez->model()->index(ui->tableView_studBolez->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите запись из таблицы"));
        return;
    }
    int id = ui->tableView_studBolez->model()->index(ui->tableView_studBolez->currentIndex().row(), 0).data().toInt();
    try
    {
        AddOrEditBolezniStudform = new AddOrEditBolezniStud(this, EDIT, id);
        AddOrEditBolezniStudform->exec();
    }
    catch (...)
    {
        return;
    }

    this->refreshData();
}

void BolezniStudentov::on_actionEdit_triggered()
{
    on_pushButton_edit_clicked();
}

void BolezniStudentov::on_actionDelete_triggered()
{
    on_pushButton_del_clicked();
}

void BolezniStudentov::on_tableView_studBolez_doubleClicked(const QModelIndex &index)
{
    on_pushButton_edit_clicked();
}
