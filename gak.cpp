#include "gak.h"
#include "ui_gak.h"
#include <QMessageBox>
#include "delegates/datedelegate.h"

bool GAK::isOpen = false;
GAK::GAK(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::GAK)
{
    ui->setupUi(this);
    this->vidim=false;
    ui->lineEdit_nomerPrikaza->setValidator( new QIntValidator(1, 1000000, this));
    ui->lineEdit_nomerPrikaza->setPlaceholderText("Вводятся только цифры");
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
    dal_studentsControl = new Dal_studentsControl(this);
    this->nomer_prikaza = 0;
    GAKModel = dal_studentsControl->getGAK(this->nomer_prikaza, 0);
    ui->tableView_gak->setModel(GAKModel);
    for (int i = 0; i < 10; ++i)
        ui->tableView_gak->setColumnHidden(i, true);

    ui->tableView_gak->setItemDelegateForColumn(10, new dateDelegate(ui->tableView_gak));
    ui->tableView_gak->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_gak->horizontalHeader()->setStretchLastSection(true);
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ui->tableView_gak->addAction(ui->actionEdit);
    ui->tableView_gak->addAction(ui->actionDelete);
}

GAK::~GAK()
{
    isOpen = false;
    delete ui;
}

void GAK::refreshData()
{
    if (!dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    GAKModel = dal_studentsControl->getGAK(this->nomer_prikaza, 0);
    ui->tableView_gak->setModel(GAKModel);
    if(this->vidim==true)
    {
        if(ui->groupBox_search->isVisible())
        {
            if(ui->tableView_gak->model()->rowCount()>0)
            {
                ui->label_result->setVisible(false);
                ui->label_naideno->setVisible(true);
                ui->label_naideno->setText("    Найдено: " + QString::number(ui->tableView_gak->model()->rowCount()));
            }
            else
            {
                ui->label_naideno->setVisible(true);
                ui->label_result->setVisible(true);
                ui->label_naideno->setText("    Найдено: 0");
            }
        }
    }
}

void GAK::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        this->nomer_prikaza = 0;
        ui->tableView_gak->setModel(GAKModel);
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

void GAK::on_pushButton_clear_clicked()
{
    this->nomer_prikaza = 0;
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
    GAKModel = dal_studentsControl->getGAK(this->nomer_prikaza, 0);
    ui->tableView_gak->setModel(GAKModel);
}

void GAK::on_pushButton_find_clicked()
{
    this->vidim = true;
    this->refreshData();
}

void GAK::on_pushButton_del_clicked()
{
    this->vidim = false;
    if (! ui->tableView_gak->model()->index(ui->tableView_gak->currentIndex().row(), 0).data().toInt())
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
        if (dal_studentsControl->deleteGAK(ui->tableView_gak->model()->index(ui->tableView_gak->currentIndex().row(), 0).data().toInt()))
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

void GAK::on_pushButton_update_clicked()
{
    this->vidim = false;
    this->refreshData();
}

void GAK::on_lineEdit_nomerPrikaza_editingFinished()
{
    this->nomer_prikaza = ui->lineEdit_nomerPrikaza->text().toInt();
}

void GAK::on_pushButton_add_clicked()
{
    try
    {
        AddOrEditGAKform = new AddOrEditGAK(this, INSERT);
        AddOrEditGAKform->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void GAK::on_pushButton_edit_clicked()
{
    if (! ui->tableView_gak->model()->index(ui->tableView_gak->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите запись из таблицы"));
        return;
    }
    int id = ui->tableView_gak->model()->index(ui->tableView_gak->currentIndex().row(), 0).data().toInt();
    try
    {
        AddOrEditGAKform = new AddOrEditGAK(this, EDIT, id);
        AddOrEditGAKform->exec();
    }
    catch (...)
    {
        return;
    }

    this->refreshData();
}

void GAK::on_tableView_gak_clicked(const QModelIndex &index)
{
    this->kom = ui->tableView_gak->model()->index(ui->tableView_gak->currentIndex().row(), 1).data().toInt();
    dal_studentsControl = new Dal_studentsControl(this);
    SostKomGAKModel = dal_studentsControl->getSostavKom(this->kom);
    if(SostKomGAKModel->rowCount()>0)
    {
        ui->label_pr->setText(SostKomGAKModel->record(0).value(1).toString());
        ui->label_sekr->setText(SostKomGAKModel->record(0).value(3).toString());
        ui->label_ch1->setText(SostKomGAKModel->record(0).value(5).toString());
        ui->label_ch2->setText(SostKomGAKModel->record(0).value(7).toString());
        ui->label_ch3->setText(SostKomGAKModel->record(0).value(9).toString());
        ui->label_ch4->setText(SostKomGAKModel->record(0).value(11).toString());
        ui->label_ch5->setText(SostKomGAKModel->record(0).value(13).toString());
        ui->label_predd->setText(SostKomGAKModel->record(0).value(2).toString());
        ui->label_sekrd->setText(SostKomGAKModel->record(0).value(4).toString());
        ui->label_ch1d->setText(SostKomGAKModel->record(0).value(6).toString());
        ui->label_ch2d->setText(SostKomGAKModel->record(0).value(8).toString());
        ui->label_ch3d->setText(SostKomGAKModel->record(0).value(10).toString());
        ui->label_ch4d->setText(SostKomGAKModel->record(0).value(12).toString());
        ui->label_ch5d->setText(SostKomGAKModel->record(0).value(14).toString());
        ui->label_raport->setText(SostKomGAKModel->record(0).value(16).toString());
    }
    ui->tableView_bally->setModel(dal_studentsControl->getGakBally(0, 0, ui->tableView_gak->model()->index(ui->tableView_gak->currentIndex().row(), 0).data().toInt()));
    ui->tableView_bally->setColumnHidden(0,true);
    ui->tableView_bally->setColumnHidden(1,true);
    ui->tableView_bally->setColumnHidden(2,true);
    ui->tableView_bally->setColumnHidden(3,true);
    ui->tableView_bally->model()->setHeaderData(7, Qt::Horizontal, ui->tableView_gak->model()->index(ui->tableView_gak->currentIndex().row(), 6).data().toString());
    ui->tableView_bally->model()->setHeaderData(8, Qt::Horizontal, ui->tableView_gak->model()->index(ui->tableView_gak->currentIndex().row(), 7).data().toString());
    ui->tableView_bally->model()->setHeaderData(9, Qt::Horizontal, ui->tableView_gak->model()->index(ui->tableView_gak->currentIndex().row(), 8).data().toString());
    ui->tableView_bally->model()->setHeaderData(10, Qt::Horizontal, ui->tableView_gak->model()->index(ui->tableView_gak->currentIndex().row(), 9).data().toString());
    ui->tableView_bally->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView_bally->horizontalHeader()->setStretchLastSection(true);
}

void GAK::on_actionEdit_triggered()
{
    on_pushButton_edit_clicked();
}

void GAK::on_actionDelete_triggered()
{
    on_pushButton_del_clicked();
}

void GAK::on_pushButton_addBally_clicked()
{
    if (! ui->tableView_gak->model()->index(ui->tableView_gak->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите запись из таблицы"));
        return;
    }
    try
    {
        addoredit_gak_bally *form = new addoredit_gak_bally(this,  ui->tableView_gak->model()->index(ui->tableView_gak->currentIndex().row(), 0).data().toInt());
        form->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}
