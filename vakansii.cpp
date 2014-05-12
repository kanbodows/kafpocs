#include "vakansii.h"
#include "ui_vakansii.h"
#include "QDebug"
#include <QMessageBox>
#include "delegates/word_wrap_delegate.h"
bool Vakansii::isOpen = false;
Vakansii::Vakansii(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Vakansii)
{
    ui->setupUi(this);
    ui->doubleSpinBox_zarplataDo->setVisible(false);
    ui->doubleSpinBox_ZarplataOt->setVisible(false);
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
    VakansiiModel = new QSqlRelationalTableModel(this);
    VakansiiModel->setTable("is_vakansii");

    VakansiiModel->setHeaderData(1,Qt::Horizontal,tr("ФИО"));
    VakansiiModel->setHeaderData(2,Qt::Horizontal,tr("Название вакансии"));
    VakansiiModel->setHeaderData(3,Qt::Horizontal,tr("Описание"));
    VakansiiModel->setHeaderData(4,Qt::Horizontal,tr("Требования"));
    VakansiiModel->setHeaderData(5,Qt::Horizontal,tr("Условия"));
    VakansiiModel->setHeaderData(6,Qt::Horizontal,tr("Зарплата"));
    VakansiiModel->setRelation(1, QSqlRelation("is_partners", "id_partners", "pt_name"));
    VakansiiModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    VakansiiModel->select();
    ui->tableView_vakansii->setModel(VakansiiModel);
    ui->tableView_vakansii->setColumnHidden(0,true);
    ui->tableView_vakansii->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_vakansii->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_vakansii->verticalHeader()->hide();
    ui->tableView_vakansii->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableView_vakansii->setItemDelegate(new QSqlRelationalDelegate(ui->tableView_vakansii));
    ui->tableView_vakansii->setItemDelegateForColumn(3, new TextEditDelegate (ui->tableView_vakansii));
//    ui->tableView_vakansii->setItemDelegateForColumn(, new TextEditDelegate (ui->tableView_vakansii));
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    this->count=0;
    this->ZarplataOt = 0;
    this->ZarplataDo = 0;
    this->naz_vakan ="";


}

Vakansii::~Vakansii()
{
    isOpen = false;
    delete ui;
}

void Vakansii::on_comboBox_znak_activated(const QString &arg1)
{
    this->Znak = arg1;
    if(this->Znak == "Больше")
    {
        ui->doubleSpinBox_ZarplataOt->setVisible(true);
        ui->label_zarplataOt->setText("Введите зарплату");
        ui->label_ZarplataDo->setVisible(false);
        ui->doubleSpinBox_zarplataDo->setVisible(false);
    }

    else if(this->Znak == "Меньше")
    {
        ui->doubleSpinBox_ZarplataOt->setVisible(true);
        ui->label_zarplataOt->setText("Введите зарплату");
        ui->label_ZarplataDo->setVisible(false);
        ui->doubleSpinBox_zarplataDo->setVisible(false);
    }

    else if(this->Znak == "Между")
    {
        ui->doubleSpinBox_ZarplataOt->setVisible(true);
        ui->label_zarplataOt->setVisible(true);
        ui->label_zarplataOt->setText("Зарплата от");

        ui->doubleSpinBox_zarplataDo->setVisible(true);
        ui->label_ZarplataDo->setVisible(true);
        ui->label_ZarplataDo->setText("Зарплата до");

    }
    else
    {
        ui->doubleSpinBox_ZarplataOt->setVisible(false);
        ui->label_zarplataOt->setVisible(false);

        ui->doubleSpinBox_zarplataDo->setVisible(false);
        ui->label_ZarplataDo->setVisible(false);
    }
}

void Vakansii::formingSearch()
{
    qDebug()<<this->naz_vakan;
    QString condition = "";

    if (this->naz_vakan!="" )
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " is_vakansii.`vk_name` like '%" + this->naz_vakan + "%' ";
    }

    if (this->Znak =="Больше")
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " is_vakansii.`vk_zp` > " + QString::number(this->ZarplataOt);
    }

    else if (this->Znak =="Меньше")
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " is_vakansii.`vk_zp` < " + QString::number(this->ZarplataOt);
    }

    else if (this->Znak =="Между")
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " is_vakansii.`vk_zp` between '" + QString::number(this->ZarplataOt) + "' and '" + QString::number(this->ZarplataDo) + "'";
    }




    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    VakansiiModel->setFilter(condition);
    qDebug()<<condition;
    if(ui->tableView_vakansii->model()->rowCount()>0)
    {
        this->naideno.append("    Найдено: " + QString::number(ui->tableView_vakansii->model()->rowCount()));
        ui->label_result->setVisible(false);
        ui->label_naideno->setText(this->naideno);
        this->naideno.clear();
    }
    else
    {
        this->naideno.append("    Найдено: 0");
        ui->label_result->setVisible(true);
        ui->label_naideno->setText(this->naideno);
        this->naideno.clear();
    }
}

void Vakansii::on_pushButton_add_clicked()
{
    VakansiiModel->insertRow(VakansiiModel->rowCount());
    ui->tableView_vakansii->edit(VakansiiModel->index(VakansiiModel->rowCount() - 1, 1));
    ui->tableView_vakansii->setCurrentIndex(VakansiiModel->index(VakansiiModel->rowCount()-1,1));
}

void Vakansii::on_pushButton_update_clicked()
{
    this->vidim=false;
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }

    VakansiiModel->select();
    ui->comboBox_znak->setCurrentText("Выберите знак");
}

void Vakansii::on_pushButton_del_clicked()
{
    if (!ui->tableView_vakansii->model()->index(ui->tableView_vakansii->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::information(this, tr("Информация"), tr("Ни одной записи не выбрано"));
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
        VakansiiModel->removeRow(ui->tableView_vakansii->currentIndex().row());
        VakansiiModel->submitAll();
        VakansiiModel->select();
    }
    else
    {
        return;
    }

}



void Vakansii::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        this->Znak = "Выберите знак";
        ui->comboBox_znak->setCurrentText("Выберите знак");
        ui->doubleSpinBox_zarplataDo->setVisible(false);
        ui->doubleSpinBox_ZarplataOt->setVisible(false);
        ui->label_zarplataOt->setVisible(false);
        ui->label_ZarplataDo->setVisible(false);
        this->ZarplataOt = 0;
        this->ZarplataDo = 0;
        ui->doubleSpinBox_zarplataDo->setValue(0);
        ui->doubleSpinBox_ZarplataOt->setValue(0);
        ui->label_naideno->setVisible(false);
        this->naz_vakan = "";
        ui->lineEdit_vakansia->setText("");
        on_pushButton_update_clicked();
        this->formingSearch();
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

void Vakansii::on_pushButton_find_clicked()
{
    if(this->Znak=="Между")
    {
        if(ui->doubleSpinBox_zarplataDo->value() < ui->doubleSpinBox_ZarplataOt->value())
        {
            QMessageBox::information(this, tr("Информация"), tr("'Зарплата от' должна быть меньше чем 'Зарплата до'"));
            return;
        }
    }
    this->vidim=true;
    this->naideno.clear();
    ui->label_naideno->setVisible(true);
    this->formingSearch();
}

void Vakansii::on_lineEdit_vakansia_editingFinished()
{
    this->naz_vakan = ui->lineEdit_vakansia->text();
}

void Vakansii::on_doubleSpinBox_ZarplataOt_editingFinished()
{
    this->ZarplataOt = ui->doubleSpinBox_ZarplataOt->value();
}

void Vakansii::on_doubleSpinBox_zarplataDo_editingFinished()
{

    this->ZarplataDo = ui->doubleSpinBox_zarplataDo->value();

}

void Vakansii::on_pushButtonSave_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    for(int i=0;i<ui->tableView_vakansii->model()->rowCount(); i++)
    {
        if(ui->tableView_vakansii->model()->index(i,1).data().isNull() || ui->tableView_vakansii->model()->index(i,2).data().isNull() || ui->tableView_vakansii->model()->index(i,3).data().isNull() || ui->tableView_vakansii->model()->index(i,4).data().isNull() || ui->tableView_vakansii->model()->index(i,5).data().isNull())
        {
            QMessageBox::information(this, tr("Информация"),tr("Поля 'ФИО' ,'Название Вакансии', 'Подробное описание'\n'Требования' и 'Условия' должны быть заполнены"));
            return;
        }
    }
    VakansiiModel->submitAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}

void Vakansii::on_pushButton_Cancel_clicked()
{
    VakansiiModel->revertAll();
}

void Vakansii::on_pushButton_clear_clicked()
{

    this->naz_vakan="";
    this->Znak="Выберите знак";
    ui->lineEdit_vakansia->clear();
    ui->doubleSpinBox_ZarplataOt->setValue(0);
    ui->doubleSpinBox_zarplataDo->setValue(0);
    ui->label_naideno->setVisible(false);
    ui->label_result->setVisible(false);
    this->vidim=false;
    this->formingSearch();
}

void Vakansii::on_pushButton_edit_clicked()
{
    if (!ui->tableView_vakansii->model()->index(ui->tableView_vakansii->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
        return;
    }
    ui->tableView_vakansii->edit(ui->tableView_vakansii->currentIndex());
}
