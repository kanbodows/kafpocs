#include "povyshkval.h"
#include "ui_povyshkval.h"
#include "delegates/datedelegate.h"
#include <QMessageBox>
#include <QFileDialog>
#include <qfiledialog.h>
#include <QFile>
#include <QDebug>

bool PovyshKval::isOpen = false;
PovyshKval::PovyshKval(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PovyshKval)
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
    ui->tableView_povyshKval->setModel(dal_prepodcontrol->getPovyshKval("","",""));
    ui->tableView_povyshKval->setColumnHidden(0,true);
    ui->tableView_povyshKval->setColumnHidden(1,true);
    ui->tableView_povyshKval->setColumnHidden(10,true);
    ui->tableView_povyshKval->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_povyshKval->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_povyshKval->setItemDelegateForColumn(3, new dateDelegate(ui->tableView_povyshKval));
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ui->tableView_povyshKval->addAction(ui->actionOpenFiles);
    ui->tableView_povyshKval->addAction(ui->actionEdit);
    ui->tableView_povyshKval->addAction(ui->actionDelete);
}

PovyshKval::~PovyshKval()
{
    isOpen = false;
    delete ui;
}

void PovyshKval::refreshData()
{
    if (!dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableView_povyshKval->setModel(dal_prepodcontrol->getPovyshKval(ui->lineEdit_sotr->text(), ui->lineEdit_tema->text(), ui->lineEdit_kurs->text()));
    if(ui->groupBox_search->isVisible())
    {
        if(ui->tableView_povyshKval->model()->rowCount()>0)
        {
            ui->label_result->setVisible(false);
            ui->label_naideno->setVisible(true);
            ui->label_naideno->setText("    Найдено: " + QString::number(ui->tableView_povyshKval->model()->rowCount()));
        }
        else
        {
            ui->label_naideno->setVisible(true);
            ui->label_result->setVisible(true);
            ui->label_naideno->setText("    Найдено: 0");
        }
    }
}

void PovyshKval::on_pushButton_add_clicked()
{
    try
    {
        Addoredit_PovyshKval *form = new Addoredit_PovyshKval(this);
        form->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void PovyshKval::on_pushButton_update_clicked()
{
    this->refreshData();
}

void PovyshKval::on_pushButton_del_clicked()
{
    if(ui->tableView_povyshKval->model()->index(ui->tableView_povyshKval->currentIndex().row(),1).data().toInt()==dal_main->getCurrentEmployee())
    {
        ui->pushButton_del->setEnabled(true);
        ui->pushButton_edit->setEnabled(true);
    if (! ui->tableView_povyshKval->model()->index(ui->tableView_povyshKval->currentIndex().row(), 0).data().toInt())
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
        if (dal_prepodcontrol->deletePovyskKval(ui->tableView_povyshKval->model()->index(ui->tableView_povyshKval->currentIndex().row(), 0).data().toInt()))
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
    else
    {
        ui->pushButton_del->setEnabled(false);
        ui->pushButton_edit->setEnabled(false);
        QMessageBox::information(this,tr("Внимание"),tr("Вы не можете удалить не свою запись"));
    }
}

void PovyshKval::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        ui->lineEdit_sotr->clear();
        ui->lineEdit_tema->clear();
        ui->lineEdit_kurs->clear();
        this->refreshData();
    }
    else
    {
        ui->groupBox_search->setVisible(true);
    }
}

void PovyshKval::on_pushButton_find_clicked()
{
    this->refreshData();
}

void PovyshKval::on_pushButton_clear_clicked()
{
    ui->lineEdit_sotr->clear();
    ui->lineEdit_tema->clear();
    ui->lineEdit_kurs->clear();
    ui->label_naideno->setVisible(false);
    ui->label_result->setVisible(false);
    this->refreshData();
}

void PovyshKval::on_pushButton_edit_clicked()
{\
    if(ui->tableView_povyshKval->model()->index(ui->tableView_povyshKval->currentIndex().row(),1).data().toInt()==dal_main->getCurrentEmployee())
    {
        ui->pushButton_del->setEnabled(true);
        ui->pushButton_edit->setEnabled(true);
    if (! ui->tableView_povyshKval->model()->index(ui->tableView_povyshKval->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите запись из таблицы"));
        return;
    }
    try
    {
        Addoredit_PovyshKval* form = new Addoredit_PovyshKval(this, true, ui->tableView_povyshKval->model()->index(ui->tableView_povyshKval->currentIndex().row(), 0).data().toInt());
        form->exec();
    }
    catch (...)
    {
        return;
    }

    this->refreshData();
}
    else
    {
        ui->pushButton_del->setEnabled(false);
        ui->pushButton_edit->setEnabled(false);
        QMessageBox::information(this,tr("Внимание"),tr("Вы не можете редактировать не свою запись"));
    }
}

void PovyshKval::on_tableView_povyshKval_doubleClicked(const QModelIndex &index)
{
    on_pushButton_edit_clicked();
}

void PovyshKval::on_actionEdit_triggered()
{
    on_pushButton_edit_clicked();
}

void PovyshKval::on_actionDelete_triggered()
{
    on_pushButton_del_clicked();
}

void PovyshKval::on_actionOpenFiles_triggered()
{
    QProcess* proc=new QProcess(this);
    proc->start("explorer " + ui->tableView_povyshKval->model()->index(ui->tableView_povyshKval->currentIndex().row(),10).data().toString());
}

void PovyshKval::on_tableView_povyshKval_clicked(const QModelIndex &index)
{
    if(ui->tableView_povyshKval->model()->index(ui->tableView_povyshKval->currentIndex().row(),1).data().toInt()==dal_main->getCurrentEmployee())
    {
        ui->pushButton_del->setEnabled(true);
        ui->pushButton_edit->setEnabled(true);
    }
    else
    {
        ui->pushButton_del->setEnabled(false);
        ui->pushButton_edit->setEnabled(false);
    }
}
