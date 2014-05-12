#include "discipline.h"
#include "ui_discipline.h"
#include <QMessageBox>
#include "delegates/textnndelegate.h"
#include "delegates/checkboxdelegate.h"

bool Discipline::isOpen = false;
Discipline::Discipline(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Discipline)
{
    ui->setupUi(this);
    if (!this->isOpen)
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
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    this->semestr_id = 0;
    ui->tableView_discipline->setModel(dal_prepodcontrol->getDisciplines("", this->semestr_id));
    ui->tableView_discipline->setItemDelegateForColumn(6, new checkboxdelegate(ui->tableView_discipline));
    ui->tableView_discipline->setItemDelegateForColumn(7, new checkboxdelegate(ui->tableView_discipline));
    ui->tableView_discipline->setItemDelegateForColumn(8, new checkboxdelegate(ui->tableView_discipline));
    ui->tableView_discipline->setItemDelegateForColumn(9, new checkboxdelegate(ui->tableView_discipline));
    ui->tableView_discipline->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_discipline->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView_discipline->setColumnHidden(0,true);
    ui->tableView_discipline->setColumnHidden(2,true);
    ui->tableView_discipline->setColumnHidden(11,true);
    ui->tableView_discipline->addAction(ui->actionEditDiscipline);
    ui->tableView_discipline->addAction(ui->actionDelDiscipline);
    ui->tableView_knigi->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView_metodichki->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView_prepod->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_prepod->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ComboBoxSemestrModel = new QSqlTableModel(this);
    ComboBoxSemestrModel->setTable("is_semmestr");
}

Discipline::~Discipline()
{
    this->isOpen = false;
    delete ui;
}

void Discipline::on_pushButton_add_clicked()
{
    try
    {
        addoredit_discipline * form = new addoredit_discipline(this);
        form->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
    on_tableView_discipline_clicked();
}

void Discipline::on_pushButton_del_clicked()
{
    if (!ui->tableView_discipline->model()->index(ui->tableView_discipline->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка удаления"), tr("Ни одной записи не выбрано"));
        return;
    }
    if (QMessageBox::warning(this, tr("Удаление записи"), tr("Вы уверены, что хотите удалить запись? \n Восстановить запись невозможно"),
                             QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        if (! dal_main->checkConnection())
        {
            QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
            return;
        }
        if (dal_prepodcontrol->deleteDiscipline(ui->tableView_discipline->model()->index(ui->tableView_discipline->currentIndex().row(), 0).data().toInt()))
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

void Discipline::on_pushButton_update_clicked()
{
    this->refreshData();
}

void Discipline::on_comboBox_semestr_activated(int index)
{
    this->semestr_id = ui->comboBox_semestr->model()->index(index,0).data().toInt();
}

void Discipline::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        this->semestr_id = 0;
        ui->lineEdit_nazvanie->clear();
        ui->label_naideno->setVisible(false);
        this->refreshData();
    }
    else
    {
        if (! dal_main->checkConnection())
        {
            QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
            return;
        }
        ui->groupBox_search->setVisible(true);
        ComboBoxSemestrModel->select();
        ui->comboBox_semestr->setModel(ComboBoxSemestrModel);
        ui->comboBox_semestr->setModelColumn(2);
        ui->comboBox_semestr->setCurrentIndex(-1);
        this->semestr_id = 0;
    }
}

void Discipline::on_pushButton_find_clicked()
{
    this->refreshData();
}

void Discipline::on_pushButton_clear_clicked()
{
    this->semestr_id = 0;
    ui->lineEdit_nazvanie->clear();
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableView_discipline->setModel(dal_prepodcontrol->getDisciplines(ui->lineEdit_nazvanie->text(), semestr_id));
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ui->comboBox_semestr->setModel(ComboBoxSemestrModel);
    ui->comboBox_semestr->setModelColumn(2);
    ui->comboBox_semestr->setCurrentIndex(-1);
}

void Discipline::on_tableView_discipline_clicked()
{
    ui->tableView_metodichki->setModel(dal_prepodcontrol->getMetodichki(ui->tableView_discipline->model()->index(ui->tableView_discipline->currentIndex().row(),0).data().toInt()));
    ui->tableView_metodichki->setColumnHidden(0,true);
    ui->tableView_metodichki->setColumnHidden(6,true);
    ui->tableView_knigi->setModel(dal_prepodcontrol->getKnigi(ui->tableView_discipline->model()->index(ui->tableView_discipline->currentIndex().row(),0).data().toInt()));
    ui->tableView_knigi->setColumnHidden(0,true);
    ui->tableView_knigi->setColumnHidden(6,true);
    ui->tableView_knigi->setColumnHidden(7,true);
    ui->tableView_prepod->setModel(dal_prepodcontrol->getPredmetPrepod(ui->tableView_discipline->model()->index(ui->tableView_discipline->currentIndex().row(),0).data().toInt()));
    ui->tableView_prepod->setColumnHidden(0,true);
    ui->tableView_prepod->setColumnHidden(6,true);
    ui->tableView_prepod->setItemDelegateForColumn(3,new checkboxdelegate(ui->tableView_prepod));
    ui->tableView_prepod->setItemDelegateForColumn(4,new checkboxdelegate(ui->tableView_prepod));
    ui->tableView_prepod->setItemDelegateForColumn(5,new checkboxdelegate(ui->tableView_prepod));
}

void Discipline::on_pushButton_edit_clicked()
{
    if(!ui->tableView_discipline->model()->index(ui->tableView_discipline->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::information(this, tr("Информация"), tr("Не выбрано ни одной записи"));
        return;
    }
    try
    {
        addoredit_discipline * form = new addoredit_discipline(this, true,  ui->tableView_discipline->model()->index(ui->tableView_discipline->currentIndex().row(),0).data().toInt());
        form->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
    on_tableView_discipline_clicked();
}

void Discipline::on_actionEditDiscipline_triggered()
{
    on_pushButton_edit_clicked();
}

void Discipline::on_actionDelDiscipline_triggered()
{
    on_pushButton_del_clicked();
}

void Discipline::on_pushButton_setPrepod_clicked()
{
    if (!ui->tableView_discipline->model()->index(ui->tableView_discipline->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка удаления"), tr("Для начала выберите дисциплину"));
        return;
    }
    try
    {
        addoredit_prepodDiscipline * form = new addoredit_prepodDiscipline(this,ui->tableView_discipline->model()->index(ui->tableView_discipline->currentIndex().row(),0).data().toInt());
        form->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
    on_tableView_discipline_clicked();
}

void Discipline::on_tableView_discipline_doubleClicked(const QModelIndex &index)
{
//    on_pushButton_edit_clicked();
}

void Discipline::refreshData()
{
    if (!dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableView_discipline->setModel(dal_prepodcontrol->getDisciplines(ui->lineEdit_nazvanie->text(), this->semestr_id));
    if(ui->groupBox_search->isVisible())
    {
        if(ui->tableView_discipline->model()->rowCount()>0)
        {
            ui->label_result->setVisible(false);
            ui->label_naideno->setVisible(true);
            ui->label_naideno->setText("    Найдено: " + QString::number(ui->tableView_discipline->model()->rowCount()));
        }
        else
        {
            ui->label_naideno->setVisible(true);
            ui->label_result->setVisible(true);
            ui->label_naideno->setText("    Найдено: 0");
        }
    }
}
