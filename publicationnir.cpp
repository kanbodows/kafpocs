#include "publicationnir.h"
#include "ui_publicationnir.h"
#include <QMessageBox>
#include <QFileDialog>
#include <qfiledialog.h>
#include <QFile>
#include <QDebug>
#include "delegates/datedelegate.h"
bool PublicationNir::isOpen = false;

PublicationNir::PublicationNir(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PublicationNir)
{
    ui->setupUi(this);
    ui->lineEdit_NomerNira->setValidator( new QIntValidator(1, 1000000, this));
    ui->lineEdit_NomerNira->setPlaceholderText("Вводятся только цифры");
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
    this->NomerProekta = "";
    this->naimenovanieFaila = "";
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    PublicationNirModel = dal_prepodcontrol->getPublication(this->NomerProekta, this->naimenovanieFaila);
    ui->tableView_pubNir->setModel(PublicationNirModel);
    ui->tableView_pubNir->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_pubNir->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_pubNir->setColumnHidden(0,true);
    ui->tableView_pubNir->setColumnHidden(1,true);
    ui->tableView_pubNir->setColumnHidden(4,true);
    ui->tableView_pubNir->addAction(ui->actionOpenFail);
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->tableView_pubNir->addAction(ui->actionEdit);
    ui->tableView_pubNir->addAction(ui->actionDelete);
    ui->tableView_pubNir->setItemDelegateForColumn(9, new dateDelegate(ui->tableView_pubNir));
}

PublicationNir::~PublicationNir()
{
    isOpen=false;
    delete ui;
}

void PublicationNir::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        ui->lineEdit_nazvanie->clear();
        ui->lineEdit_NomerNira->clear();
        ui->label_naideno->setVisible(false);
        refreshData();
    }
    else
    {
        ui->groupBox_search->setVisible(true);
    }
}

void PublicationNir::refreshData()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableView_pubNir->setModel(dal_prepodcontrol->getPublication(ui->lineEdit_NomerNira->text(), ui->lineEdit_nazvanie->text()));
    if(ui->groupBox_search->isVisible() == true)
    {
        if(ui->tableView_pubNir->model()->rowCount()>0)
        {
            ui->label_result->setVisible(false);
        }
        else
        {
            ui->label_result->setVisible(true);
        }
        ui->label_naideno->setVisible(true);
        ui->label_naideno->setText("    Найдено: " + QString::number(ui->tableView_pubNir->model()->rowCount()));
    }
    ui->tableView_pubNir->setColumnHidden(0,true);
    ui->tableView_pubNir->setColumnHidden(1,true);
    ui->tableView_pubNir->setColumnHidden(4,true);
}

void PublicationNir::on_pushButton_find_clicked()
{
    this->refreshData();
}

void PublicationNir::on_pushButton_clear_clicked()
{
    ui->label_naideno->clear();
    ui->lineEdit_nazvanie->clear();
    ui->lineEdit_NomerNira->clear();
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    this->refreshData();
}

void PublicationNir::on_pushButton_del_clicked()
{
    if (! ui->tableView_pubNir->model()->index(ui->tableView_pubNir->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::information(this, tr("Информация"), tr("Ни одной записи не выбрано"));
        return;
    }
    if (QMessageBox::information(this, tr("Информация"), tr("        Вы уверены, что хотите удалить запись? \n Все данные связанные с записью также будут удалены"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        if (! dal_main->checkConnection())
        {
            QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
            return;
        }
        this->files = this->ui->tableView_pubNir->model()->index(ui->tableView_pubNir->currentIndex().row(), 4).data().toString();
        QFileInfo fi(this->files);
        QString nam = fi.fileName();
        QFile("C:\\Dropbox\\" + nam).remove();
        if (dal_prepodcontrol->deletePublicationNir(ui->tableView_pubNir->model()->index(ui->tableView_pubNir->currentIndex().row(), 0).data().toInt()))
        {
            this->refreshData();
            QMessageBox::information(this, tr("Удаление"), tr("Данные успешно удалены"));


        }
        else
        {
            QMessageBox::information(this, tr("Информация"), tr("Не удалось удалить данные, попробуйте еще раз"));
            this->refreshData();
        }
    }
    else
    {
        this->refreshData();
        return;
    }
}

void PublicationNir::on_actionOpenFail_triggered()
{
    QProcess* proc=new QProcess(this);
    proc->start("explorer " + ui->tableView_pubNir->model()->index(ui->tableView_pubNir->currentIndex().row(),4).data().toString());
}

void PublicationNir::on_pushButton_update_clicked()
{
    this->refreshData();
}

void PublicationNir::on_pushButton_add_clicked()
{
    try
    {
        AddOrEditPublicationNir * AddOrEditPublicationNirform = new AddOrEditPublicationNir(this, INSERT);
        AddOrEditPublicationNirform->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void PublicationNir::on_pushButtonEdit_clicked()
{
    if(!ui->tableView_pubNir->currentIndex().isValid())
    {
        QMessageBox::information(this, tr("Информация"), tr("Не выбрано ни одной записи"));
        return;
    }
    try
    {
        AddOrEditPublicationNir * AddOrEditPublicationNirform = new AddOrEditPublicationNir(this, EDIT,  ui->tableView_pubNir->model()->index(ui->tableView_pubNir->currentIndex().row(),0).data().toInt());
        AddOrEditPublicationNirform->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void PublicationNir::on_actionEdit_triggered()
{
    on_pushButtonEdit_clicked();
}

void PublicationNir::on_actionDelete_triggered()
{
    on_pushButton_del_clicked();
}
