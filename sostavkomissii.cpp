#include "sostavkomissii.h"
#include "ui_sostavkomissii.h"
#include <QMessageBox>
#include "delegates/textnndelegate.h"
#include "delegates/datedelegate.h"
bool SostavKomissii::isOpen = false;
SostavKomissii::SostavKomissii(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SostavKomissii)
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
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    this->nomerRaporta = 0;
    SostavKomModel = dal_prepodcontrol->getSostavKom(this->nomerRaporta);
    ui->tableView_sostavKomissii->setModel(SostavKomModel);
    for (int i = 0; i < 15; ++i)
        ui->tableView_sostavKomissii->setColumnHidden(i,true);
    ui->tableView_sostavKomissii->setColumnHidden(16,true);
    ui->tableView_sostavKomissii->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_sostavKomissii->horizontalHeader()->setStretchLastSection(true);
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ui->tableView_sostavKomissii->addAction(ui->actionEdit);
    ui->tableView_sostavKomissii->addAction(ui->actionDelete);
}

SostavKomissii::~SostavKomissii()
{
    isOpen = false;
    delete ui;
}

void SostavKomissii::refreshData()
{
    if (!dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    SostavKomModel = dal_prepodcontrol->getSostavKom(this->nomerRaporta);
    ui->tableView_sostavKomissii->setModel(SostavKomModel);
    if(ui->groupBox_search->isVisible())
    {
        if(ui->tableView_sostavKomissii->model()->rowCount()>0)
        {
            ui->label_result->setVisible(false);
            ui->label_naideno->setVisible(true);
            ui->label_naideno->setText("    Найдено: " + QString::number(ui->tableView_sostavKomissii->model()->rowCount()));
        }
        else
        {
            ui->label_naideno->setVisible(true);
            ui->label_result->setVisible(true);
            ui->label_naideno->setText("    Найдено: 0");
        }
    }
}

void SostavKomissii::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        this->nomerRaporta = 0;
        ui->tableView_sostavKomissii->setModel(SostavKomModel);
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
     }
}

void SostavKomissii::on_pushButton_clear_clicked()
{
    this->nomerRaporta = 0;
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
    SostavKomModel = dal_prepodcontrol->getSostavKom(this->nomerRaporta);
    ui->tableView_sostavKomissii->setModel(SostavKomModel);

}

void SostavKomissii::on_pushButton_find_clicked()
{
    this->refreshData();
}

void SostavKomissii::on_pushButton_del_clicked()
{
    if (! ui->tableView_sostavKomissii->model()->index(ui->tableView_sostavKomissii->currentIndex().row(), 0).data().toInt())
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
        if (dal_prepodcontrol->deleteSostavKom(ui->tableView_sostavKomissii->model()->index(ui->tableView_sostavKomissii->currentIndex().row(), 0).data().toInt()))
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

void SostavKomissii::on_pushButton_update_clicked()
{
    this->refreshData();
}

void SostavKomissii::on_pushButton_add_clicked()
{
    try
    {
        AddOrEditSostavKomform = new AddOrEditSostavKom(this, INSERT);
        AddOrEditSostavKomform->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void SostavKomissii::on_lineEdit_nomerRaporta_editingFinished()
{
    this->nomerRaporta = ui->lineEdit_nomerRaporta->text().toInt();
}

void SostavKomissii::on_pushButton_edit_clicked()
{
    if (! ui->tableView_sostavKomissii->model()->index(ui->tableView_sostavKomissii->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите запись из таблицы"));
        return;
    }
    int id = ui->tableView_sostavKomissii->model()->index(ui->tableView_sostavKomissii->currentIndex().row(), 0).data().toInt();
   qDebug()<<id;
    try
    {
        AddOrEditSostavKomform = new AddOrEditSostavKom(this, EDIT, id);
        AddOrEditSostavKomform->exec();
    }
    catch (...)
    {
        return;
    }

    this->refreshData();
}

void SostavKomissii::on_actionEdit_triggered()
{
    on_pushButton_edit_clicked();
}

void SostavKomissii::on_actionDelete_triggered()
{
    on_pushButton_del_clicked();
}

void SostavKomissii::on_tableView_sostavKomissii_clicked(const QModelIndex &index)
{
    if(SostavKomModel->rowCount()>0)
    {
        ui->label_pr->setText(SostavKomModel->record(ui->tableView_sostavKomissii->currentIndex().row()).value(1).toString());
        ui->label_sekr->setText(SostavKomModel->record(ui->tableView_sostavKomissii->currentIndex().row()).value(3).toString());
        ui->label_ch1->setText(SostavKomModel->record(ui->tableView_sostavKomissii->currentIndex().row()).value(5).toString());
        ui->label_ch2->setText(SostavKomModel->record(ui->tableView_sostavKomissii->currentIndex().row()).value(7).toString());
        ui->label_ch3->setText(SostavKomModel->record(ui->tableView_sostavKomissii->currentIndex().row()).value(9).toString());
        ui->label_ch4->setText(SostavKomModel->record(ui->tableView_sostavKomissii->currentIndex().row()).value(11).toString());
        ui->label_ch4->setText(SostavKomModel->record(ui->tableView_sostavKomissii->currentIndex().row()).value(13).toString());
        ui->label_predd->setText(SostavKomModel->record(ui->tableView_sostavKomissii->currentIndex().row()).value(2).toString());
        ui->label_sekrd->setText(SostavKomModel->record(ui->tableView_sostavKomissii->currentIndex().row()).value(4).toString());
        ui->label_ch1d->setText(SostavKomModel->record(ui->tableView_sostavKomissii->currentIndex().row()).value(6).toString());
        ui->label_ch2d->setText(SostavKomModel->record(ui->tableView_sostavKomissii->currentIndex().row()).value(8).toString());
        ui->label_ch3d->setText(SostavKomModel->record(ui->tableView_sostavKomissii->currentIndex().row()).value(10).toString());
        ui->label_ch4d->setText(SostavKomModel->record(ui->tableView_sostavKomissii->currentIndex().row()).value(12).toString());
        ui->label_ch4d->setText(SostavKomModel->record(ui->tableView_sostavKomissii->currentIndex().row()).value(14).toString());
        ui->label_raport->setText(SostavKomModel->record(ui->tableView_sostavKomissii->currentIndex().row()).value(16).toString());
    }
}
