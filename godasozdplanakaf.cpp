#include "godasozdplanakaf.h"
#include "ui_godasozdplanakaf.h"
#include <QMessageBox>

bool GodaSozdPlanaKaf::isOpen = false;
GodaSozdPlanaKaf::GodaSozdPlanaKaf(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GodaSozdPlanaKaf)
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
    GodSozdPlKafModel = new QSqlTableModel(this);
    GodSozdPlKafModel->setTable("is_GodSozdPlanaKaf");
    GodSozdPlKafModel->setHeaderData(1,Qt::Horizontal,tr("Учебный год создания плана заседаний кафедры"));

    GodSozdPlKafModel->select();

    ui->tableViewGodSozdPlKaf->setModel(GodSozdPlKafModel);
    ui->tableViewGodSozdPlKaf->setColumnHidden(0,true);
    ui->tableViewGodSozdPlKaf->setColumnHidden(2,true);
    ui->tableViewGodSozdPlKaf->setColumnHidden(3,true);
    ui->tableViewGodSozdPlKaf->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableViewGodSozdPlKaf->horizontalHeader()->setStretchLastSection(true);

}

GodaSozdPlanaKaf::~GodaSozdPlanaKaf()
{
    this->isOpen = false;
    delete ui;
}

void GodaSozdPlanaKaf::on_pushButton_createplan_clicked()
{
    try
    {
        AddOrEditPlanZasKafForm = new AddOrEditPlanZasKaf(this);
        AddOrEditPlanZasKafForm->exec();
    }
    catch (...)
    {
        return;
    }
    GodSozdPlKafModel->select();
}

void GodaSozdPlanaKaf::on_pushButton_viewPlan_clicked()
{
    if (! ui->tableViewGodSozdPlKaf->model()->index(ui->tableViewGodSozdPlKaf->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите запись из таблицы"));
        return;
    }
    QString godS = ui->tableViewGodSozdPlKaf->model()->index(ui->tableViewGodSozdPlKaf->currentIndex().row(), 2).data().toString();
    QString godPo = ui->tableViewGodSozdPlKaf->model()->index(ui->tableViewGodSozdPlKaf->currentIndex().row(), 3).data().toString();
    try
    {
        ProsmotrPlanaZasKafForm = new ProsmotrPlanaZasKaf(this, godS, godPo);
        ProsmotrPlanaZasKafForm->exec();
    }
    catch (...)
    {
        return;
    }

}

void GodaSozdPlanaKaf::on_pushButtonDelPlan_clicked()
{
    if (! ui->tableViewGodSozdPlKaf->model()->index(ui->tableViewGodSozdPlKaf->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите запись из таблицы"));
        return;
    }
    QString godS = ui->tableViewGodSozdPlKaf->model()->index(ui->tableViewGodSozdPlKaf->currentIndex().row(), 2).data().toString();
    QString godPo = ui->tableViewGodSozdPlKaf->model()->index(ui->tableViewGodSozdPlKaf->currentIndex().row(), 3).data().toString();
    dal_prepodcontrol = new Dal_prepodcontrol(this);

if (QMessageBox::warning(this, tr("Удаление данных"), tr("Вы уверены, что хотите удалить запись? \n Восстановить данные невозможно"),
                         QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
{
    dal_prepodcontrol->deletePlanKaf(godS,godPo);
    dal_prepodcontrol->deleteProsmotrPlanKaf(godS,godPo);
    GodSozdPlKafModel->select();
    QMessageBox::information(this, tr("Информация"), tr("Данные удалены"));
}

else
{
    QMessageBox::information(this, tr("Удаление"), tr("Не удалось удалить данные, попробуйте еще раз"));
}
    ui->tableViewGodSozdPlKaf->setModel(GodSozdPlKafModel);
}

void GodaSozdPlanaKaf::on_tableViewGodSozdPlKaf_doubleClicked(const QModelIndex &index)
{
   on_pushButton_viewPlan_clicked();
}

void GodaSozdPlanaKaf::on_actionDelPlanZasedKaf_triggered()
{
    on_pushButtonDelPlan_clicked();
}
