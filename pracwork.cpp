#include "pracwork.h"
#include "ui_pracwork.h"
#include "delegates/datedelegate.h"
#include <QMessageBox>
bool PracWork::isOpen = false;

PracWork::PracWork(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PracWork)
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
    this->Student="";
    this->Group="";
    this->dateBeginning = QDate();
    this->dateEnd = ui->dateEdit_end_2->date();
    this->isDate = false;
    this->type_prac=0;
    pracWorkModels = dal_studentsControl->getPracWork(this->Student, this->Group, this->dateBeginning, this->dateEnd, this->type_prac);
    ui->tableView_pracwork->setModel(pracWorkModels);
    ui->tableView_pracwork->setColumnHidden(0,true);
    ui->tableView_pracwork->setColumnHidden(1,true);
    ui->tableView_pracwork->setColumnHidden(2,true);
    ui->tableView_pracwork->setColumnHidden(3,true);
    ui->tableView_pracwork->setColumnHidden(4,true);
    ui->tableView_pracwork->setColumnHidden(8,true);
    ui->tableView_pracwork->setColumnHidden(9,true);
    ui->tableView_pracwork->setColumnHidden(10,true);
    ui->tableView_pracwork->setColumnHidden(11,true);
    ui->tableView_pracwork->setColumnHidden(13,true);
    ui->tableView_pracwork->setColumnHidden(14,true);

    ui->tableView_pracwork->setColumnHidden(16,true);

    ui->tableView_pracwork->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_pracwork->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_pracwork->setItemDelegateForColumn(15, new dateDelegate(ui->tableView_pracwork));
    ui->tableView_pracwork->addAction(ui->actionDetails);
    ui->tableView_pracwork->addAction(ui->actionDel);
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);

    QSqlQueryModel *comboTypePract = new QSqlQueryModel(this);
    comboTypePract = dal_studentsControl->getComboTypePrac();
    ui->comboBox_type_practika->setModel(comboTypePract);
    ui->comboBox_type_practika->setModelColumn(1);
    ui->comboBox_type_practika->setCurrentIndex(-1);

}

PracWork::~PracWork()
{
    isOpen = false;
    delete ui;
}

void PracWork::refreshData()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableView_pracwork->setModel(dal_studentsControl->getPracWork(this->Student, this->Group, this->dateBeginning, this->dateEnd, this->type_prac));

    if(this->vidim==true)
    {
        if(ui->tableView_pracwork->model()->rowCount()>0)
        {
            this->naideno.append("                                   Найдено: " + QString::number(ui->tableView_pracwork->model()->rowCount()));
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

void PracWork::on_pushButton_search_2_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        this->Student="";
        this->Group="";
        this->dateBeginning = QDate();
        this->isDate = false;
        this->type_prac=0;
        ui->lineEdit_student_2->clear();
        ui->lineEdit_group_2->clear();
        ui->comboBox_type_practika->setCurrentIndex(-1);
        QSqlQueryModel *comboTypePract = new QSqlQueryModel(this);
        comboTypePract = dal_studentsControl->getComboTypePrac();
        ui->comboBox_type_practika->setModel(comboTypePract);
        ui->comboBox_type_practika->setModelColumn(1);
        ui->comboBox_type_practika->setCurrentIndex(-1);
        this->refreshData();
    }
    else
    {
        ui->groupBox_search->setVisible(true);
        ui->dateEdit_end_2->setDate(QDate::currentDate());
    }
    on_pushButton_clear_2_clicked();

}

void PracWork::on_pushButton_clear_2_clicked()
{
    this->dateBeginning = QDate();
    this->isDate = false;
    this->Student="";
    this->Group="";
    this->type_prac=0;
    ui->lineEdit_student_2->clear();
    ui->lineEdit_group_2->clear();
    ui->comboBox_type_practika->setCurrentIndex(-1);
    ui->label_naideno->setVisible(false);
    ui->label_result->setVisible(false);
    this->vidim=false;
    this->refreshData();
}

void PracWork::on_pushButton_find_2_clicked()
{
    this->vidim=true;
    this->Student = ui->lineEdit_student_2->text();
    this->Group = ui->lineEdit_group_2->text();
    this->type_prac = ui->comboBox_type_practika->model()->index(ui->comboBox_type_practika->currentIndex(),0).data().toInt();
    if (this->isDate==true)
    {
        this->dateBeginning = ui->dateEdit_2->date();
        this->dateEnd = ui->dateEdit_end_2->date();
    }
    this->refreshData();
}

void PracWork::on_dateEdit_2_dateChanged(const QDate &date)
{
    isDate = true;
}

void PracWork::on_dateEdit_end_2_dateChanged(const QDate &date)
{
    isDate = false;
}

void PracWork::on_comboBox_type_practika_activated(int index)
{
    this->type_prac = ui->comboBox_type_practika->model()->index(ui->comboBox_type_practika->currentIndex(),0).data().toInt();
}

void PracWork::on_pushButton_del_2_clicked()
{
    if (! ui->tableView_pracwork->model()->index(ui->tableView_pracwork->currentIndex().row(), 0).data().toInt())
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
        if (dal_studentsControl->deletePracWork(ui->tableView_pracwork->model()->index(ui->tableView_pracwork->currentIndex().row(), 0).data().toInt()))
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

void PracWork::on_pushButton_update_2_clicked()
{
    this->vidim=false;
    this->refreshData();
}

void PracWork::on_pushButton_add_2_clicked()
{
    try
    {
        addoreditform = new AddOrEditPracWork(this, INSERT);
        addoreditform->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void PracWork::on_pushButton_edit_2_clicked()
{

    if (! ui->tableView_pracwork->model()->index(ui->tableView_pracwork->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите запись из таблицы"));
        return;
    }
    int id = ui->tableView_pracwork->model()->index(ui->tableView_pracwork->currentIndex().row(), 0).data().toInt();
    try
    {
        addoreditform = new AddOrEditPracWork(this, EDIT, id);
        addoreditform->exec();
    }
    catch (...)
    {
        return;
    }

    this->refreshData();
}

void PracWork::on_tableView_pracwork_doubleClicked(const QModelIndex &index)
{
     on_pushButton_edit_2_clicked();
}

void PracWork::on_actionDetails_triggered()
{
    if (! ui->tableView_pracwork->model()->index(ui->tableView_pracwork->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите запись из таблицы"));
        return;
    }
    int id = ui->tableView_pracwork->model()->index(ui->tableView_pracwork->currentIndex().row(), 0).data().toInt();
    try
    {
        addoreditform = new AddOrEditPracWork(this, EDIT, id, VIEWSV);
        addoreditform->exec();
    }
    catch (...)
    {
        return;
    }

    this->refreshData();
}

void PracWork::on_actionDel_triggered()
{
    on_pushButton_del_2_clicked();
}
