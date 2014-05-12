#include "platnieobrazovatuslgi.h"
#include "ui_platnieobrazovatuslgi.h"
#include <QMessageBox>
#include "delegates/word_wrap_delegate.h"

bool PlatnieObrazovatUslgi::isOpen = false;

PlatnieObrazovatUslgi::PlatnieObrazovatUslgi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PlatnieObrazovatUslgi)
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

    ObrazovatKursModels = dal_prepodcontrol->getPlatnieObrazUslugi(this->NazvanieKursa, this->vidKursa);
    ui->tableViewPlObrazUslugi->setModel(ObrazovatKursModels);
    ui->tableViewPlObrazUslugi->setColumnHidden(0,true);
    ui->tableViewPlObrazUslugi->setColumnHidden(1,true);

    ui->tableViewPlObrazUslugi->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableViewPlObrazUslugi->horizontalHeader()->setStretchLastSection(true);
    ui->tableViewPlObrazUslugi->verticalHeader()->hide();
    ui->tableViewPlObrazUslugi->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ui->tableViewPlObrazUslugi->setItemDelegateForColumn(2, new TextEditDelegate(ui->tableViewPlObrazUslugi));

    ui->tableViewPlObrazUslugi->addAction(ui->actionEdit);
    ui->tableViewPlObrazUslugi->addAction(ui->actionDelete);
}

PlatnieObrazovatUslgi::~PlatnieObrazovatUslgi()
{
    isOpen = false;
    delete ui;
}

void PlatnieObrazovatUslgi::refreshData()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableViewPlObrazUslugi->setModel(dal_prepodcontrol->getPlatnieObrazUslugi(this->NazvanieKursa, this->vidKursa));

    if(this->vidim==true)
    {
        if(ui->tableViewPlObrazUslugi->model()->rowCount()>0)
        {
            this->naideno.append("                                   Найдено: " + QString::number(ui->tableViewPlObrazUslugi->model()->rowCount()));
            ui->label_result->setVisible(false);
            ui->label_naideno->setVisible(true);
            ui->label_naideno->setText(this->naideno);
            this->naideno.clear();
        }
        else
        {
            ui->label_naideno->setVisible(true);
            this->naideno.append("                                   Найдено: 0");
            ui->label_result->setVisible(true);
            ui->label_naideno->setText(this->naideno);
            this->naideno.clear();
        }
    }
}

void PlatnieObrazovatUslgi::on_pushButtonSearch_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        ui->label_naideno->setVisible(false);
        this->NazvanieKursa="";
        this->vidKursa="";
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

        this->NazvanieKursa = "";
        this->vidKursa = "";
        ui->lineEdit_nazvanieKursa->clear();
        ui->comboBoxVidKursa->setCurrentIndex(-1);
        this->refreshData();
    }
}

void PlatnieObrazovatUslgi::on_pushButton_find_clicked()
{
    this->vidim=true;
    this->NazvanieKursa = ui->lineEdit_nazvanieKursa->text();
    this->refreshData();
}

void PlatnieObrazovatUslgi::on_comboBoxVidKursa_activated(const QString &arg1)
{
    this->vidKursa = arg1;
}

void PlatnieObrazovatUslgi::on_pushButton_clear_clicked()
{
    this->NazvanieKursa = "";
    this->vidKursa = "";
    ui->comboBoxVidKursa->setCurrentIndex(-1);

    ui->lineEdit_nazvanieKursa->clear();
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
   this->refreshData();
}

void PlatnieObrazovatUslgi::on_pushButton_upd_clicked()
{
    this->vidim=false;
    this->refreshData();
}

void PlatnieObrazovatUslgi::on_pushButton_4_clicked()
{
    if(ui->tableViewPlObrazUslugi->model()->index(ui->tableViewPlObrazUslugi->currentIndex().row(),1).data().toInt()==dal_main->getCurrentEmployee())
    {
        ui->pushButton_Edit->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
    this->vidim = false;
    if (! ui->tableViewPlObrazUslugi->model()->index(ui->tableViewPlObrazUslugi->currentIndex().row(), 0).data().toInt())
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
        if (dal_prepodcontrol->deleteObrazKursi(ui->tableViewPlObrazUslugi->model()->index(ui->tableViewPlObrazUslugi->currentIndex().row(), 0).data().toInt()))
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
    else
    {
        ui->pushButton_Edit->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        QMessageBox::information(this,tr("Внимание"),tr("Вы не можете удалить не свою запись"));
    }
}

void PlatnieObrazovatUslgi::on_pushButtonAdd_clicked()
{
    try
    {
        add_or_edit_PlatnieObrazovatKursiform = new add_or_edit_PlatnieObrazovatKursi(this, INSERT);
        add_or_edit_PlatnieObrazovatKursiform->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void PlatnieObrazovatUslgi::on_pushButton_Edit_clicked()
{
    if(ui->tableViewPlObrazUslugi->model()->index(ui->tableViewPlObrazUslugi->currentIndex().row(),1).data().toInt()==dal_main->getCurrentEmployee())
    {
        ui->pushButton_Edit->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
    if (! ui->tableViewPlObrazUslugi->model()->index(ui->tableViewPlObrazUslugi->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите запись из таблицы"));
        return;
    }
    int id = ui->tableViewPlObrazUslugi->model()->index(ui->tableViewPlObrazUslugi->currentIndex().row(), 0).data().toInt();
    try
    {
        add_or_edit_PlatnieObrazovatKursiform = new add_or_edit_PlatnieObrazovatKursi(this, EDIT, id);
        add_or_edit_PlatnieObrazovatKursiform->exec();
    }
    catch (...)
    {
        return;
    }

    this->refreshData();
    }
    else
    {
        ui->pushButton_Edit->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
        QMessageBox::information(this,tr("Внимание"),tr("Вы не можете редактировать не свою запись"));
    }
}

void PlatnieObrazovatUslgi::on_actionDelete_triggered()
{
on_pushButton_4_clicked();
}

void PlatnieObrazovatUslgi::on_actionEdit_triggered()
{
    on_pushButton_Edit_clicked();
}

void PlatnieObrazovatUslgi::on_tableViewPlObrazUslugi_doubleClicked(const QModelIndex &index)
{
    on_pushButton_Edit_clicked();
}

void PlatnieObrazovatUslgi::on_tableViewPlObrazUslugi_clicked(const QModelIndex &index)
{
    if(ui->tableViewPlObrazUslugi->model()->index(ui->tableViewPlObrazUslugi->currentIndex().row(),1).data().toInt()==dal_main->getCurrentEmployee())
    {
        ui->pushButton_Edit->setEnabled(true);
        ui->pushButton_4->setEnabled(true);
    }
    else
    {
        ui->pushButton_Edit->setEnabled(false);
        ui->pushButton_4->setEnabled(false);
    }
}
