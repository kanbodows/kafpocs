#include "knigi.h"
#include "ui_knigi.h"
#include <QMessageBox>
#include "delegates/textnndelegate.h"
#include "delegates/datedelegate.h"

bool Knigi::isOpen = false;
Knigi::Knigi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Knigi)
{
    ui->setupUi(this);
    if (!isOpen)
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
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    knigiModel = dal_prepodcontrol->getKniga("",0);
    ui->tableView->setModel(knigiModel);
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnHidden(1,true);
    ui->tableView->setColumnHidden(3,true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->tableView->setItemDelegateForColumn(7, new dateDelegate(ui->tableView));
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    disciplineModel = new QSqlTableModel(this);
    disciplineModel->setTable("is_discipline");
    this->discipline = 0;
    ui->tableView->addAction(ui->actionEdit);
    ui->tableView->addAction(ui->actionDelete);
}

Knigi::~Knigi()
{
    isOpen = false;
    delete ui;
}

void Knigi::on_pushButton_add_clicked()
{
    try
    {
        addoredit_knigi * form = new addoredit_knigi(this,false,-1);
        form->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void Knigi::on_pushButton_edit_clicked()
{
    if (!ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка удаления"), tr("Ни одной записи не выбрано"));
        return;
    }
    try
    {
        addoredit_knigi * form = new addoredit_knigi(this,true,ui->tableView->model()->index(ui->tableView->currentIndex().row(),0).data().toInt());
        form->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void Knigi::on_pushButton_del_clicked()
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
        if (dal_prepodcontrol->deleteKniga(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().toInt()))
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

void Knigi::on_pushButton_update_clicked()
{
this->refreshData();
}

void Knigi::on_pushButton_find_clicked()
{
    this->refreshData();
}

void Knigi::on_pushButton_clear_clicked()
{
    this->discipline = 0;
    this->nazvanies = "";
    ui->label_naideno->clear();
    ui->lineEdit_nazvanieKnigi->clear();
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ui->tableView->setModel(knigiModel);
    ui->comboBox_discipline->setModel(disciplineModel);
    ui->comboBox_discipline->setModelColumn(1);
    ui->comboBox_discipline->setCurrentIndex(-1);
}

void Knigi::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        this->discipline = 0;
        ui->tableView->setModel(knigiModel);
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
        disciplineModel->select();
        ui->comboBox_discipline->setModel(disciplineModel);
        ui->comboBox_discipline->setModelColumn(1);
        ui->comboBox_discipline->setCurrentIndex(-1);
    }
}

void Knigi::on_comboBox_discipline_activated(int index)
{
    this->discipline = ui->comboBox_discipline->model()->index(ui->comboBox_discipline->currentIndex(),0).data().toInt();
}

void Knigi::on_tableView_doubleClicked(const QModelIndex &index)
{
    on_pushButton_edit_clicked();
}

void Knigi::refreshData()
{
    if (!dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableView->setModel(dal_prepodcontrol->getKniga(this->nazvanies, this->discipline));
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

void Knigi::on_lineEdit_nazvanieKnigi_editingFinished()
{
    this->nazvanies = ui->lineEdit_nazvanieKnigi->text();
}

void Knigi::on_actionEdit_triggered()
{
    on_pushButton_edit_clicked();
}

void Knigi::on_actionDelete_triggered()
{
    on_pushButton_del_clicked();
}
