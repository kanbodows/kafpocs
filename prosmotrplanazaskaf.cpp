#include "prosmotrplanazaskaf.h"
#include "ui_prosmotrplanazaskaf.h"
#include <QMessageBox>
#include "delegates/checkboxdelegate.h"

ProsmotrPlanaZasKaf::ProsmotrPlanaZasKaf(QWidget *parent, QString GodSS, QString GodPo) :
    QDialog(parent),
    ui(new Ui::ProsmotrPlanaZasKaf)
{
    ui->setupUi(this);

    this->GodStart = GodSS;
    this->GodEnd = GodPo;
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    ProsmotrPlanaZasKafModel = new QSqlTableModel(this);
    ProsmotrPlanaZasKafModel->setTable("is_prosmotrPlana");
    ProsmotrPlanaZasKafModel->setHeaderData(1,Qt::Horizontal,tr("Вопросы заседания"));
    ProsmotrPlanaZasKafModel->setHeaderData(2,Qt::Horizontal,tr("Исполнители"));
    ProsmotrPlanaZasKafModel->setHeaderData(3,Qt::Horizontal,tr("Сроки исполнения"));
    ProsmotrPlanaZasKafModel->setHeaderData(4,Qt::Horizontal,tr("Отметка о выполнении"));

    ProsmotrPlanaZasKafModel->setFilter("God_s = " + GodSS + " and God_po = " + GodPo);
    ProsmotrPlanaZasKafModel->select();

    ui->tableViewProsmotrPlana->setModel(ProsmotrPlanaZasKafModel);
    ui->tableViewProsmotrPlana->setColumnHidden(0,true);
    ui->tableViewProsmotrPlana->setColumnHidden(5,true);
    ui->tableViewProsmotrPlana->setColumnHidden(6,true);
    ui->tableViewProsmotrPlana->setItemDelegateForColumn(4, new checkboxdelegate(ui->tableViewProsmotrPlana));
    ui->tableViewProsmotrPlana->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableViewProsmotrPlana->horizontalHeader()->setStretchLastSection(true);
    ui->tableViewProsmotrPlana->verticalHeader()->hide();
    ui->tableViewProsmotrPlana->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableViewProsmotrPlana->addAction(ui->actionPostavitGalochku);
    ui->pushButton_Edit->setVisible(false);
}


ProsmotrPlanaZasKaf::~ProsmotrPlanaZasKaf()
{
    delete ui;
}

void ProsmotrPlanaZasKaf::on_pushButtonAdd_clicked()
{
    try
    {
        AddOrEditPlanZasKafForm = new AddOrEditPlanZasKaf(this, INSERT, -1, this->GodStart, this->GodEnd);
        AddOrEditPlanZasKafForm->exec();
    }
    catch (...)
    {
        return;
    }
    ProsmotrPlanaZasKafModel->setFilter("God_s = " + this->GodStart + " and God_po = " + this->GodEnd);
    ProsmotrPlanaZasKafModel->select();
}

void ProsmotrPlanaZasKaf::on_pushButton_Edit_clicked()
{
    if(ui->tableViewProsmotrPlana->model()->index(ui->tableViewProsmotrPlana->currentIndex().row(),4).data().toInt()==0)
    {
    if (! ui->tableViewProsmotrPlana->model()->index(ui->tableViewProsmotrPlana->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите запись из таблицы"));
        return;
    }
    int id = ui->tableViewProsmotrPlana->model()->index(ui->tableViewProsmotrPlana->currentIndex().row(), 0).data().toInt();
    try
    {
        AddOrEditPlanZasKafForm = new AddOrEditPlanZasKaf(this, EDIT, id);
        AddOrEditPlanZasKafForm->exec();
    }
    catch (...)
    {
        return;
    }
    ProsmotrPlanaZasKafModel->setFilter("God_s = " + this->GodStart + " and God_po = " + this->GodEnd);
    ProsmotrPlanaZasKafModel->select();
    }
    else
        QMessageBox::information(this, tr("Информация"), tr("Выбранный вопрос является закрытым"));

}

void ProsmotrPlanaZasKaf::on_actionPostavitGalochku_triggered()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    int id = ui->tableViewProsmotrPlana->model()->index(ui->tableViewProsmotrPlana->currentIndex().row(), 0).data().toInt();
    dal_prepodcontrol->updateGalochkaInProsmotrplana(id);
    ProsmotrPlanaZasKafModel->setFilter("God_s = " + this->GodStart + " and God_po = " + this->GodEnd);
    ProsmotrPlanaZasKafModel->select();
}

void ProsmotrPlanaZasKaf::on_pushButton_Del_clicked()
{
    if (!ui->tableViewProsmotrPlana->model()->index(ui->tableViewProsmotrPlana->currentIndex().row(), 0).data().isValid())
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
        ProsmotrPlanaZasKafModel->removeRow(ui->tableViewProsmotrPlana->currentIndex().row());
        ProsmotrPlanaZasKafModel->submitAll();
        ProsmotrPlanaZasKafModel->setFilter("God_s = " + this->GodStart + " and God_po = " + this->GodEnd);
        ProsmotrPlanaZasKafModel->select();
    }
    else
    {
        return;
    }

}

void ProsmotrPlanaZasKaf::on_pushButton_upd_clicked()
{
    ProsmotrPlanaZasKafModel->setFilter("God_s = " + this->GodStart + " and God_po = " + this->GodEnd);
    ProsmotrPlanaZasKafModel->select();
}
