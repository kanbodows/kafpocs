#include "addoredit_prepoddiscipline.h"
#include "ui_addoredit_prepoddiscipline.h"

addoredit_prepodDiscipline::addoredit_prepodDiscipline(QWidget *parent, int discipline_id) :
    QDialog(parent),
    discipline_id(discipline_id),
    ui(new Ui::addoredit_prepodDiscipline)
{
    ui->setupUi(this);
    if (!dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    ui->listView_gruppa->setModel(dal_prepodcontrol->getGroups(this->discipline_id));
    ui->listView_gruppa->setModelColumn(2);
    ui->listView_sotrudnik->setModel(dal_prepodcontrol->getcomboPrepod());
    ui->listView_sotrudnik->setModelColumn(1);
    if(dal_prepodcontrol->existLektor(this->discipline_id))
    {
        ui->checkBox_lk->setEnabled(false);
    }
    else
    {
        ui->checkBox_pr->setEnabled(false);
        ui->checkBox_lb->setEnabled(false);
    }
    ui->label_error->setVisible(false);
}

addoredit_prepodDiscipline::~addoredit_prepodDiscipline()
{
    delete ui;
}

void addoredit_prepodDiscipline::on_pushButton_ok_clicked()
{
   /*if (ui->listView_discipline->model()->index(ui->listView_discipline->currentIndex().row(), == -1 || ui->listView_gruppa->currentIndex() == -1 || ui->listView_sotrudnik->currentIndex() == -1)
    {
        ui->label_error->setVisible(true);
        return;
    }*/
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    if (dal_prepodcontrol->addPrepodDiscipline(ui->listView_sotrudnik->model()->index(ui->listView_sotrudnik->currentIndex().row(),0).data().toInt(),
                                               ui->listView_gruppa->model()->index(ui->listView_gruppa->currentIndex().row(),0).data().toInt(),
                                               this->discipline_id,
                                               ui->checkBox_lk->isChecked(),
                                               ui->checkBox_pr->isChecked(),
                                               ui->checkBox_lb->isChecked()))
    {
        QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
    }
    else
    {
        QMessageBox::warning(this, tr("Ошибка"), tr("Данные не были добавлены в базу данных"));
    }
    this->close();
}

void addoredit_prepodDiscipline::on_pushButton_cancel_clicked()
{
    this->close();
}

void addoredit_prepodDiscipline::on_listView_gruppa_clicked(const QModelIndex &index)
{
    if(dal_prepodcontrol->existPraktik(ui->listView_gruppa->model()->index(ui->listView_gruppa->currentIndex().row(),0).data().toInt(),
                                       this->discipline_id))
    {
        ui->checkBox_pr->setEnabled(false);
        ui->checkBox_pr->setChecked(false);
    }
    else
    {
        ui->checkBox_pr->setEnabled(true);
    }
    if(dal_prepodcontrol->existLabor(ui->listView_gruppa->model()->index(ui->listView_gruppa->currentIndex().row(),0).data().toInt(),
                                     this->discipline_id))
    {
        ui->checkBox_lb->setEnabled(false);
        ui->checkBox_lb->setChecked(false);
    }
    else
    {
        ui->checkBox_lb->setEnabled(true);
    }
    if(!ui->checkBox_pr->isEnabled() && !ui->checkBox_lk->isEnabled() && !ui->checkBox_lb->isEnabled())
    {
        ui->pushButton_ok->setEnabled(false);
        ui->label_error->setText("На текущую группу уже назначены преподаватели");
        ui->label_error->setVisible(true);
    }
    else
    {
        ui->pushButton_ok->setEnabled(true);
        ui->label_error->setVisible(false);
    }

}
