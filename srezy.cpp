#include "srezy.h"
#include "ui_srezy.h"
#include <QMessageBox>
#include "delegates/textnndelegate.h"
#include "delegates/datedelegate.h"

bool Srezy::isOpen = false;
Srezy::Srezy(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Srezy)
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
    dal_studentsControl = new Dal_studentsControl(this);\
    ui->dateEdit_start->setDate(QDate(2000,0,0));
    ui->dateEdit_end->setDate(QDate::currentDate());
    ui->tableView->setModel(dal_studentsControl->getSrezy(0, 0,  ui->dateEdit_start->date(), ui->dateEdit_end->date()));
    ui->tableView->setColumnHidden(0,true);
    ui->tableView->setColumnHidden(1,true);
    ui->tableView->setColumnHidden(3,true);
    ui->tableView->setColumnHidden(4,true);
    ui->tableView->setColumnHidden(5,true);
    ui->tableView->setColumnHidden(6,true);
    ui->tableView->setColumnHidden(7,true);
    ui->tableView->setColumnHidden(10,true);
    ui->tableView->setItemDelegateForColumn(8, new dateDelegate(ui->tableView));
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_bally->horizontalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView->horizontalHeader()->setStretchLastSection(true);
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ui->tableView->addAction(ui->actionSetBally);
    ui->tableView->addAction(ui->actionEdit);
    ui->tableView->addAction(ui->actionDelete);
    this->gruppa = 0;
    this->spec  = 0;
    ui->comboBox_gruppa->setEnabled(false);
}

Srezy::~Srezy()
{
    isOpen=false;
    delete ui;
}

void Srezy::on_pushButton_add_clicked()
{
    try
    {
        addoredit_srez * form = new addoredit_srez(this,false,-1);
        form->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void Srezy::on_pushButton_del_clicked()
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
        if (dal_studentsControl->deleteSrez(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().toInt()))
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

void Srezy::on_pushButton_update_clicked()
{
    this->refreshData();
}

void Srezy::on_pushButton_find_clicked()
{
    this->refreshData();
}

void Srezy::on_pushButton_clear_clicked()
{
    this->gruppa = 0;
    this->spec = 0;
    ui->dateEdit_start->setDate(QDate(2000,0,0));
    ui->dateEdit_end->setDate(QDate::currentDate());
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
    ui->tableView->setModel(dal_studentsControl->getSrezy(this->spec, this->gruppa, ui->dateEdit_start->date(), ui->dateEdit_end->date()));
    ui->comboBox_spec->setModel(dal_studentsControl->getSpec());
    ui->comboBox_spec->setModelColumn(1);
    ui->comboBox_spec->setCurrentIndex(-1);
    ui->comboBox_gruppa->setCurrentIndex(-1);
    ui->comboBox_gruppa->setEnabled(false);
}

void Srezy::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        this->gruppa = 0;
        ui->dateEdit_start->setDate(QDate(2000,0,0));
        ui->dateEdit_end->setDate(QDate::currentDate());
        ui->tableView->setModel(dal_studentsControl->getSrezy(this->spec, this->gruppa, ui->dateEdit_start->date(), ui->dateEdit_end->date()));
        ui->label_naideno->setVisible(false);
        this->spec = 0;
        ui->comboBox_gruppa->setCurrentIndex(-1);
        ui->comboBox_spec->setCurrentIndex(-1);
        ui->comboBox_gruppa->setEnabled(false);
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
        ui->comboBox_spec->setModel(dal_studentsControl->getSpec());
        ui->comboBox_spec->setModelColumn(1);
        ui->comboBox_spec->setCurrentIndex(-1);

    }
}

void Srezy::on_pushButton_edit_clicked()
{
    if (!ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка удаления"), tr("Ни одной записи не выбрано"));
        return;
    }
    try
    {
        addoredit_srez * form = new addoredit_srez(this,true,ui->tableView->model()->index(ui->tableView->currentIndex().row(),0).data().toInt());
        form->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void Srezy::on_tableView_doubleClicked(const QModelIndex &index)
{
    on_pushButton_edit_clicked();
}

void Srezy::refreshData()
{
    if (!dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableView->setModel(dal_studentsControl->getSrezy(this->spec, this->gruppa, ui->dateEdit_start->date(), ui->dateEdit_end->date()));
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

void Srezy::on_comboBox_gruppa_activated(int index)
{
    this->gruppa = ui->comboBox_gruppa->model()->index(index,0).data().toInt();
}

void Srezy::on_pushButton_addBalls_clicked()
{
    if (!ui->tableView->model()->index(ui->tableView->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка удаления"), tr("Ни одной записи не выбрано"));
        return;
    }
    try
    {
        addoredit_srez_bally * form = new addoredit_srez_bally(this, ui->tableView->model()->index(ui->tableView->currentIndex().row(),0).data().toInt());
        form->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void Srezy::on_actionSetBally_triggered()
{
    on_pushButton_addBalls_clicked();
}

void Srezy::on_actionEdit_triggered()
{
    on_pushButton_edit_clicked();
}

void Srezy::on_actionDelete_triggered()
{
    on_pushButton_del_clicked();
}

void Srezy::on_tableView_clicked(const QModelIndex &index)
{
    if(ui->tableView->model()->rowCount()<=0)
        return;
    QSqlQueryModel* queryStud = new QSqlQueryModel();
    queryStud->setQuery("select * from srez_bally_view where srez_id = " + ui->tableView->model()->index(ui->tableView->currentIndex().row(),0).data().toString());
    ui->tableView_bally->setModel(queryStud);
    ui->tableView_bally->setColumnHidden(0, true);
    ui->tableView_bally->setColumnHidden(1, true);
    ui->tableView_bally->setColumnHidden(8, true);
    ui->tableView_bally->setColumnHidden(9, true);
    ui->tableView_bally->model()->setHeaderData(2, Qt::Horizontal,tr("ФИО студента"));
    ui->tableView_bally->model()->setHeaderData(3, Qt::Horizontal, ui->tableView->model()->index(ui->tableView->currentIndex().row(),3).data().toString());
    QSqlQuery *querySred = dal_studentsControl->getSrezySrednie(ui->tableView->model()->index(ui->tableView->currentIndex().row(),0).data().toInt());
    ui->label_p1->setText(ui->tableView->model()->index(ui->tableView->currentIndex().row(),3).data().toString());
    ui->label_b1->setText(querySred->value(1).toString());
    if(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 4).data().toString() != "")
    {
        ui->tableView_bally->setColumnHidden(4, false);
        ui->tableView_bally->model()->setHeaderData(4, Qt::Horizontal, ui->tableView->model()->index(ui->tableView->currentIndex().row(),4).data().toString());
        ui->label_p2->setText(ui->tableView->model()->index(ui->tableView->currentIndex().row(),4).data().toString());
        ui->label_b2->setText(querySred->value(2).toString());
    }
    else
    {
        ui->label_p2->text().clear();
        ui->label_b2->text().clear();
        ui->tableView_bally->setColumnHidden(4, true);
    }

    if(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 5).data().toString() != "")
    {
        ui->tableView_bally->setColumnHidden(5, false);
        ui->tableView_bally->model()->setHeaderData(5, Qt::Horizontal, ui->tableView->model()->index(ui->tableView->currentIndex().row(), 5).data().toString());
        ui->label_p3->setText(ui->tableView->model()->index(ui->tableView->currentIndex().row(),5).data().toString());
        ui->label_b3->setText(querySred->value(3).toString());
    }
    else
    {
        ui->label_p3->text().clear();
        ui->label_b3->text().clear();
        ui->tableView_bally->setColumnHidden(5, true);
    }

    if(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 6).data().toString() != "")
    {
        ui->tableView_bally->setColumnHidden(6, false);
        ui->tableView_bally->model()->setHeaderData(6, Qt::Horizontal, ui->tableView->model()->index(ui->tableView->currentIndex().row(), 6).data().toString());
        ui->label_p4->setText(ui->tableView->model()->index(ui->tableView->currentIndex().row(),6).data().toString());
        ui->label_b4->setText(querySred->value(4).toString());
    }
    else
    {
        ui->label_p4->text().clear();
        ui->label_b4->text().clear();
        ui->tableView_bally->setColumnHidden(6, true);
    }

    if(ui->tableView->model()->index(ui->tableView->currentIndex().row(), 7).data().toString() != "")
    {
        ui->tableView_bally->setColumnHidden(7, false);
        ui->tableView_bally->model()->setHeaderData(7, Qt::Horizontal, ui->tableView->model()->index(ui->tableView->currentIndex().row(), 7).data().toString());
        ui->label_p5->setText(ui->tableView->model()->index(ui->tableView->currentIndex().row(),7).data().toString());
        ui->label_b5->setText(querySred->value(5).toString());
    }
    else
    {
        ui->label_p5->text().clear();
        ui->label_b5->text().clear();
        ui->tableView_bally->setColumnHidden(7, true);
    }
    if(ui->tableView_bally->model()->rowCount()<=0)
    {
        ui->label_b1->text().clear();
        ui->label_b2->text().clear();
        ui->label_b3->text().clear();
        ui->label_b4->text().clear();
        ui->label_b5->text().clear();
    }
}

void Srezy::on_comboBox_spec_activated(const QString &arg1)
{
    this->spec = ui->comboBox_spec->model()->index(ui->comboBox_spec->currentIndex(), 0).data().toInt();
    ui->comboBox_gruppa->setModel(dal_studentsControl->getComboGroup(this->spec));
    ui->comboBox_gruppa->setModelColumn(1);
    ui->comboBox_gruppa->setCurrentIndex(-1);
    ui->comboBox_gruppa->setEnabled(true);
}
