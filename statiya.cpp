#include "statiya.h"
#include "ui_statiya.h"
#include "delegates/datedelegate.h"
#include<QMessageBox>
bool Statiya::isOpen = false;
Statiya::Statiya(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Statiya)
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
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    this->Avtor="";
    this->soAvtor="";
    this->tema="";
    statiaModels = dal_prepodcontrol->getStatia(this->Avtor, this->soAvtor, this->tema);
    ui->tableView_statia->setModel(statiaModels);
    statiaModels->setHeaderData(3,Qt::Horizontal,tr("Автор статьи"));
    statiaModels->setHeaderData(4,Qt::Horizontal,tr("Соавтор статьи"));
    statiaModels->setHeaderData(5,Qt::Horizontal,tr("Название статьи"));
    statiaModels->setHeaderData(6,Qt::Horizontal,tr("Дата"));

    ui->tableView_statia->setColumnHidden(0,true);
    ui->tableView_statia->setColumnHidden(1,true);
    ui->tableView_statia->setColumnHidden(2,true);
    ui->tableView_statia->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_statia->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_statia->setItemDelegateForColumn(6, new dateDelegate(ui->tableView_statia));
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ui->tableView_statia->addAction(ui->actionEdit);
    ui->tableView_statia->addAction(ui->actionDelete);
}

Statiya::~Statiya()
{
    isOpen=false;
    delete ui;
}

void Statiya::refreshData()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableView_statia->setModel(dal_prepodcontrol->getStatia(this->Avtor, this->soAvtor, this->tema));
    if(this->vidim==true)
    {
        if(ui->tableView_statia->model()->rowCount()>0)
        {
            this->naideno.append("    Найдено: " + QString::number(ui->tableView_statia->model()->rowCount()));
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

void Statiya::on_pushButton_add_clicked()
{
    try
    {
        addoreditform = new AddOrEditStat(this, INSERT);
        addoreditform->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void Statiya::on_pushButton_update_clicked()
{
    this->vidim=false;
    this->refreshData();
}

void Statiya::on_pushButton_del_clicked()
{
    if (! ui->tableView_statia->model()->index(ui->tableView_statia->currentIndex().row(), 0).data().toInt())
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
        if (dal_prepodcontrol->deleteStatia(ui->tableView_statia->model()->index(ui->tableView_statia->currentIndex().row(), 0).data().toInt()))
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

void Statiya::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        this->Avtor="";
        this->soAvtor="";
        this->tema="";
        ui->lineEdit_avtor->clear();
        ui->lineEdit_nazvanie->clear();
        ui->lineEdit_soAvtor->clear();
        this->refreshData();
    }
    else
    {
        ui->groupBox_search->setVisible(true);
    }
}

void Statiya::on_pushButton_find_clicked()
{
    this->vidim=true;
    this->Avtor = ui->lineEdit_avtor->text();
    this->soAvtor = ui->lineEdit_soAvtor->text();
    this->tema = ui->lineEdit_nazvanie->text();
    this->refreshData();
}

void Statiya::on_pushButton_clear_clicked()
{
    this->Avtor="";
    this->soAvtor="";
    this->tema="";
    ui->lineEdit_avtor->clear();
    ui->lineEdit_nazvanie->clear();
    ui->lineEdit_soAvtor->clear();
    ui->label_naideno->setVisible(false);
    ui->label_result->setVisible(false);
    this->vidim=false;
    this->refreshData();
}

void Statiya::on_Statiya_accepted()
{
    ui->label_naideno->setVisible(false);
    ui->label_result->setVisible(false);
    this->refreshData();
}

void Statiya::on_pushButton_edit_clicked()
{
    if (! ui->tableView_statia->model()->index(ui->tableView_statia->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите запись из таблицы"));
        return;
    }
    int id = ui->tableView_statia->model()->index(ui->tableView_statia->currentIndex().row(), 0).data().toInt();
    try
    {
        addoreditform = new AddOrEditStat(this, EDIT, id);
        addoreditform->exec();
    }
    catch (...)
    {
        return;
    }

    this->refreshData();
}

void Statiya::on_tableView_statia_doubleClicked(const QModelIndex &index)
{
    on_pushButton_edit_clicked();
}

void Statiya::on_actionEdit_triggered()
{
    on_pushButton_edit_clicked();
}

void Statiya::on_actionDelete_triggered()
{
    on_pushButton_del_clicked();
}
