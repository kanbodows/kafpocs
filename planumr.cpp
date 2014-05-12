#include "planumr.h"
#include "ui_planumr.h"
#include "delegates/word_wrap_delegate.h"
#include <QMessageBox>
bool PlanUMR::isOpen = false;

PlanUMR::PlanUMR(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlanUMR)
{
    ui->setupUi(this);
    ui->lineEdit_god->setValidator( new QIntValidator(1, 1000000, this));
    ui->lineEdit_god->setPlaceholderText("Вводятся только цифры");
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

    dal_prepodcontrol = new Dal_prepodcontrol(this);

    PlanUMRModels = dal_prepodcontrol->getPlanUmr("");
    ui->tableViewPlanUmr->setModel(PlanUMRModels);
    ui->tableViewPlanUmr->setColumnHidden(0,true);
    ui->tableViewPlanUmr->setColumnHidden(7,true);
    ui->tableViewPlanUmr->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableViewPlanUmr->horizontalHeader()->setStretchLastSection(true);
    ui->tableViewPlanUmr->verticalHeader()->hide();
    ui->tableViewPlanUmr->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableViewPlanUmr->setItemDelegateForColumn(3,new TextEditDelegate(ui->tableViewPlanUmr));

  //  ui->tableViewPlanUmr->addAction(ui->actionEdit);
   // ui->tableViewPlanUmr->addAction(ui->actionDelete);
}

PlanUMR::~PlanUMR()
{
    delete ui;
    isOpen = false;
}

void PlanUMR::refreshData()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableViewPlanUmr->setModel(dal_prepodcontrol->getPlanUmr(this->god));

//    if(this->vidim==true)
//    {
//        if(ui->tableView_jurnalVzaimopos->model()->rowCount()>0)
//        {
//            this->naideno.append("                                   Найдено: " + QString::number(ui->tableView_jurnalVzaimopos->model()->rowCount()));
//            ui->label_result->setVisible(false);
//            ui->label_naideno->setVisible(true);
//            ui->label_naideno->setText(this->naideno);
//            this->naideno.clear();
//        }
//        else
//        {
//            ui->label_naideno->setVisible(true);
//            this->naideno.append("                                   Найдено: 0");
//            ui->label_result->setVisible(true);
//            ui->label_naideno->setText(this->naideno);
//            this->naideno.clear();
//        }
//    }
}

void PlanUMR::on_pushButton_Del_clicked()
{
//    if(ui->tableViewPlanUmr->model()->index(ui->tableViewPlanUmr->currentIndex().row(),1).data().toInt()==dal_main->getCurrentEmployee())
//    {
        ui->pushButton_Del->setEnabled(true);
        //ui->pushButton_edit->setEnabled(true);
        if (! ui->tableViewPlanUmr->model()->index(ui->tableViewPlanUmr->currentIndex().row(), 0).data().toInt())
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
            if (dal_prepodcontrol->deleteZapPlanUmr(ui->tableViewPlanUmr->model()->index(ui->tableViewPlanUmr->currentIndex().row(), 0).data().toInt()))
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
//    }
//    else
//    {
//        ui->pushButton_Del->setEnabled(false);
//        //ui->pushButton_edit->setEnabled(false);
//        QMessageBox::information(this,tr("Внимание"),tr("Вы не можете удалить не свою запись"));
//    }

}

void PlanUMR::on_pushButton_upd_clicked()
{
        this->refreshData();
}

void PlanUMR::on_pushButtonAdd_clicked()
{
    try
    {
        AddOrEditPlanUmrform = new AddOrEditPlanUmr(this, INSERT);
        AddOrEditPlanUmrform->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void PlanUMR::on_lineEdit_god_editingFinished()
{
 this->god = ui->lineEdit_god->text();
    this->refreshData();
}

void PlanUMR::on_pushButton_Edit_clicked()
{
    if (! ui->tableViewPlanUmr->model()->index(ui->tableViewPlanUmr->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите запись из таблицы"));
        return;
    }
    int id = ui->tableViewPlanUmr->model()->index(ui->tableViewPlanUmr->currentIndex().row(), 0).data().toInt();
    try
    {
         AddOrEditPlanUmrform = new AddOrEditPlanUmr(this, EDIT, id);
         AddOrEditPlanUmrform->exec();
    }
    catch (...)
    {
        return;
    }

    this->refreshData();
}
