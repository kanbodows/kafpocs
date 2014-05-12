#include "student.h"
#include "ui_student.h"
#include <QMessageBox>
#include "delegates/textnndelegate.h"
#include "delegates/checkboxdelegate.h"
#include "delegates/datedelegate.h"

bool Student::isOpen = false;
Student::Student(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Student)
{
    ui->setupUi(this);
    ui->lineEdit_name->setText("");
    this->gruppa = 0;
    this->spec = 0;
    this->vidim = false;
    if (! this->isOpen)
    {
        this->isOpen = true;
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
    dal_studentsControl = new Dal_studentsControl(this);
    ui->tableView->setModel(dal_studentsControl->getStudents(0, 0, ""));
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setColumnHidden(1, true);
    ui->tableView->setColumnHidden(2, true);
    ui->tableView->setColumnHidden(3, true);
    for (int i = 12; i < 25; ++i)
        ui->tableView->setColumnHidden(i, true);
    ui->tableView->setItemDelegateForColumn(9, new dateDelegate(ui->tableView));
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->addAction(ui->actionEdit);
    ui->tableView->addAction(ui->actionDelete);
    ui->comboBox_gruppa->setEnabled(false);
}

Student::~Student()
{
    this->isOpen=false;
    delete ui;
}

void Student::on_pushButton_add_clicked()
{
    try
    {
        addoredit_student * form = new addoredit_student(this);
        form->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void Student::on_pushButton_del_clicked()
{

    this->vidim = false;
    if (! ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::warning(this, tr("Ошибка удаления"), tr("Ни одной записи не выбрано"));
        return;
    }
    if (QMessageBox::warning(this, tr("Удаление записи"), tr("Вы уверены, что хотите удалить запись? \n Восстановить запись невозможно"),
                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        if (dal_studentsControl->deleteStudent(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().toInt()))
        {
            QMessageBox::information(this, tr("Удаление"), tr("Данные успешно удалены"));
        }
        else
        {
            QMessageBox::information(this, tr("Удаление"), tr("Не удалось удалить данные, попробуйте еще раз"));
        }
    }
    this->refreshData();
}

void Student::on_pushButton_update_clicked()
{
    this->vidim = false;
    this->refreshData();
}

void Student::on_pushButton_find_clicked()
{
    this->vidim = true;
    this->refreshData();
}

void Student::on_pushButton_clear_clicked()
{
    ui->label_naideno->clear();
    ui->lineEdit_name->clear();
    this->gruppa = 0;
    this->spec = 0;
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    ui->comboBox_spec->setModel(dal_studentsControl->getSpec());
    ui->comboBox_spec->setModelColumn(1);
    ui->comboBox_spec->setCurrentIndex(-1);
    ui->comboBox_gruppa->setCurrentIndex(-1);
    ui->comboBox_gruppa->setEnabled(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ui->tableView->setModel(dal_studentsControl->getStudents(this->spec, this->gruppa, ui->lineEdit_name->text()));
}

void Student::refreshData()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableView->setModel(dal_studentsControl->getStudents(this->spec, this->gruppa, ui->lineEdit_name->text()));
    if(this->vidim)
    {
        if(ui->groupBox_search->isVisible())
        {
            if(ui->tableView->model()->rowCount()>0)
            {
                ui->label_result->setVisible(false);
            }
            else
            {
                ui->label_result->setVisible(true);
            }
            ui->label_naideno->setVisible(true);
            ui->label_naideno->setText("    Найдено: " + QString::number(ui->tableView->model()->rowCount()));
        }
    }
}


void Student::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        ui->comboBox_gruppa->setCurrentIndex(-1);
        ui->comboBox_spec->setCurrentIndex(-1);
        ui->comboBox_gruppa->setEnabled(false);
        ui->label_naideno->setVisible(false);
        ui->lineEdit_name->clear();
        this->gruppa = 0;
        this->spec = 0;
        this->refreshData();
    }
    else
    {
        ui->groupBox_search->setVisible(true);
        ui->comboBox_spec->setModel(dal_studentsControl->getSpec());
        ui->comboBox_spec->setModelColumn(1);
        ui->comboBox_spec->setCurrentIndex(-1);
    }
}

void Student::on_pushButton_edit_clicked()
{
    if(!ui->tableView->currentIndex().isValid())
    {
        QMessageBox::information(this, tr("Информация"), tr("Не выбрано ни одной записи"));
        return;
    }
    try
    {
        addoredit_student * form = new addoredit_student(this, true,  ui->tableView->model()->index(ui->tableView->currentIndex().row(),0).data().toInt());
        form->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void Student::on_tableView_doubleClicked(const QModelIndex &index)
{
    on_pushButton_edit_clicked();
}

void Student::on_comboBox_gruppa_activated(int index)
{
    this->gruppa = ui->comboBox_gruppa->model()->index(ui->comboBox_gruppa->currentIndex(), 0).data().toInt();
}

void Student::on_actionEdit_triggered()
{
    on_pushButton_edit_clicked();
}

void Student::on_actionDelete_triggered()
{
    on_pushButton_del_clicked();
}

void Student::on_comboBox_spec_activated(const QString &arg1)
{
    this->spec = ui->comboBox_spec->model()->index(ui->comboBox_spec->currentIndex(), 0).data().toInt();
    ui->comboBox_gruppa->setModel(dal_studentsControl->getComboGroup(this->spec));
    ui->comboBox_gruppa->setModelColumn(1);
    ui->comboBox_gruppa->setCurrentIndex(-1);
    ui->comboBox_gruppa->setEnabled(true);
}
