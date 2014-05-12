#include "course_work.h"
#include "ui_course_work.h"
#include <QMessageBox>
#include "delegates/textnndelegate.h"
#include "delegates/datedelegate.h"

bool Course_work::isOpen = false;
Course_work::Course_work(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Course_work)
{
    ui->setupUi(this);

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

    groupsModel = new QSqlTableModel(this);
    groupsModel->setTable("is_group");
    disciplineModel = new QSqlTableModel(this);
    disciplineModel->setTable("is_discipline");
    dal_studentsControl = new Dal_studentsControl(this);
    this->discipline=0;
    this->group_id = 0;
    ui->tableView->setModel(dal_studentsControl->getCourseWorks(this->group_id, this->discipline, "", 0));
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnHidden(1,true);
    ui->tableView->setColumnHidden(2,true);
    ui->tableView->setColumnHidden(3,true);
    ui->tableView->setColumnHidden(4,true);
    ui->tableView->setColumnHidden(10,true);
    ui->tableView->setColumnHidden(14,true);
    ui->tableView->setColumnHidden(15,true);
    ui->tableView->setItemDelegateForColumn(12, new dateDelegate(ui->tableView));
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ui->tableView->addAction(ui->actionEdit);
    ui->tableView->addAction(ui->actionDelete);
}

Course_work::~Course_work()
{
    isOpen=false;
    delete ui;
}

void Course_work::on_pushButton_add_clicked()
{
    try
    {
        addoredit_course_work * form = new addoredit_course_work(this, false, -1);
        form->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void Course_work::on_pushButton_del_clicked()
{
    if (! ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().toInt())
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
        if (dal_studentsControl->deleteCourseWork(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().toInt()))
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

void Course_work::on_pushButton_update_clicked()
{
    this->refreshData();
}

void Course_work::on_pushButton_find_clicked()
{
    this->refreshData();
}

void Course_work::on_pushButton_clear_clicked()
{
    this->discipline = 0;
    this->group_id = 0;
    ui->lineEdit_tema->clear();
    ui->label_naideno->clear();
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    if (!dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableView->setModel(dal_studentsControl->getCourseWorks(this->group_id, this->discipline, ui->lineEdit_tema->text(), 0));
    ui->comboBox_discipline->setModel(disciplineModel);
    ui->comboBox_discipline->setModelColumn(1);
    ui->comboBox_discipline->setCurrentIndex(-1);
    ui->comboBox_group->setModel(groupsModel);
    ui->comboBox_group->setModelColumn(1);
    ui->comboBox_group->setCurrentIndex(-1);
}

void Course_work::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        this->discipline = 0;
        this->group_id = 0;
        ui->lineEdit_tema->clear();
        ui->label_naideno->setVisible(false);
        on_pushButton_update_clicked();
    }
    else
    {
        if (! dal_main->checkConnection())
        {
            QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
            return;
        }
        ui->groupBox_search->setVisible(true);
        groupsModel->select();
        disciplineModel->select();
        ui->comboBox_group->setModel(groupsModel);
        ui->comboBox_group->setModelColumn(1);
        ui->comboBox_group->setCurrentIndex(-1);
        ui->comboBox_discipline->setModel(disciplineModel);
        ui->comboBox_discipline->setModelColumn(1);
        ui->comboBox_discipline->setCurrentIndex(-1);
    }
}

void Course_work::on_comboBox_group_activated(int index)
{
    this->group_id = ui->comboBox_group->model()->index(index,0).data().toInt();
}

void Course_work::on_comboBox_discipline_activated(int index)
{
    this->discipline = ui->comboBox_discipline->model()->index(index,0).data().toInt();
}

void Course_work::on_pushButton_edit_clicked()
{
    if (!ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка удаления"), tr("Ни одной записи не выбрано"));
        return;
    }
    try
    {
        addoredit_course_work * form = new addoredit_course_work(this, true, ui->tableView->model()->index(ui->tableView->currentIndex().row(),0).data().toInt());
        form->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void Course_work::on_tableView_doubleClicked(const QModelIndex &index)
{
    on_pushButton_edit_clicked();
}

void Course_work::refreshData()
{
    if (!dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableView->setModel(dal_studentsControl->getCourseWorks(this->group_id, this->discipline, "", 0));
    if(ui->groupBox_search->isVisible())
    {
        if(ui->tableView->model()->rowCount()>0)
        {
            ui->label_result->setVisible(false);
            ui->label_naideno->setVisible(true);
            ui->label_naideno->setText("    Найдено: " + QString::number(ui->tableView->model()->rowCount()));
        }
        else
        {
            ui->label_naideno->setVisible(true);
            ui->label_result->setVisible(true);
            ui->label_naideno->setText("    Найдено: 0");
        }
    }
}

void Course_work::on_actionEdit_triggered()
{
    on_pushButton_edit_clicked();
}

void Course_work::on_actionDelete_triggered()
{
    on_pushButton_del_clicked();
}
