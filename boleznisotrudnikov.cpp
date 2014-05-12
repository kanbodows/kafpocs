#include "boleznisotrudnikov.h"
#include "ui_boleznisotrudnikov.h"
#include <QMessageBox>
#include "delegates/datedelegate.h"
bool BolezniSotrudnikov::isOpen = false;
BolezniSotrudnikov::BolezniSotrudnikov(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::BolezniSotrudnikov)
{
    ui->setupUi(this);
    this->count=0;

    this->dateBeginning = ui->dateEdit_start->date();
    this->dateEnd = QDate::currentDate();
    ui->dateEdit_end->setDate(QDate::currentDate());
    this->isDate = false;
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
    BoleznSotrModel = new QSqlRelationalTableModel(this);
    BoleznSotrModel->setTable("is_sotr_bolezni");
    BoleznSotrModel->setHeaderData(1,Qt::Horizontal,tr("Преподаватель"));
    BoleznSotrModel->setHeaderData(2,Qt::Horizontal,tr("Болезнь"));
    BoleznSotrModel->setHeaderData(3,Qt::Horizontal,tr("Дата заболевания"));
    BoleznSotrModel->setHeaderData(4,Qt::Horizontal,tr("Дата выздоравления"));
    BoleznSotrModel->setRelation(1, QSqlRelation("sotr_kafedri", "id_sotr", "st_fio"));
    BoleznSotrModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    BoleznSotrModel->select();
    ComboSotrudnikiModel = new QSqlTableModel(this);
    ComboSotrudnikiModel->setTable("sotr_kafedri");

    ui->tableView_sotrBolez->setModel(BoleznSotrModel);
    ui->tableView_sotrBolez->setColumnHidden(0,true);
    ui->tableView_sotrBolez->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_sotrBolez->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_sotrBolez->setItemDelegate(new QSqlRelationalDelegate(ui->tableView_sotrBolez));
    ui->tableView_sotrBolez->setItemDelegateForColumn(3, new dateDelegate(ui->tableView_sotrBolez));
    ui->tableView_sotrBolez->setItemDelegateForColumn(4, new dateDelegate(ui->tableView_sotrBolez));
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ui->tableView_sotrBolez->addAction(ui->actionEdit);
    ui->tableView_sotrBolez->addAction(ui->actionDelete);
}

BolezniSotrudnikov::~BolezniSotrudnikov()
{
    isOpen = false;
    delete ui;
}

void BolezniSotrudnikov::formingSearch()
{
    QString condition = "";

    if (! ui->lineEdit_bolezn->text().isEmpty())
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " bolezn like '%" + ui->lineEdit_bolezn->text() + "%'";
    }

    if (this->sotrudnik_id != 0)
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " sotrudnik = " + QString::number(this->sotrudnik_id);
    }

    if(!this->dateBeginning.isNull())
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " date_in > '" + dateBeginning.toString("yyyy-MM-dd") + "' and date_out < '" + dateEnd.toString("yyyy-MM-dd 23:59:59") + "'";
    }


    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }

    BoleznSotrModel->setFilter(condition);
    if(ui->tableView_sotrBolez->model()->rowCount()>0)
    {
    this->naideno.append("    Найдено: " + QString::number(ui->tableView_sotrBolez->model()->rowCount()));
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

void BolezniSotrudnikov::on_pushButton_add_clicked()
{
    BoleznSotrModel->insertRow(BoleznSotrModel->rowCount());
    ui->tableView_sotrBolez->edit(BoleznSotrModel->index(BoleznSotrModel->rowCount() - 1, 1));
    ui->tableView_sotrBolez->setCurrentIndex(BoleznSotrModel->index(BoleznSotrModel->rowCount()-1,1));
}

void BolezniSotrudnikov::on_pushButton_update_clicked()
{

    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    BoleznSotrModel = new QSqlRelationalTableModel(this);
    BoleznSotrModel->setTable("is_sotr_bolezni");
    BoleznSotrModel->setHeaderData(1,Qt::Horizontal,tr("Преподаватель"));
    BoleznSotrModel->setHeaderData(2,Qt::Horizontal,tr("Болезнь"));
    BoleznSotrModel->setHeaderData(3,Qt::Horizontal,tr("Дата заболевания"));
    BoleznSotrModel->setHeaderData(4,Qt::Horizontal,tr("Дата выздоравления"));
    BoleznSotrModel->setRelation(1, QSqlRelation("sotr_kafedri", "id_sotr", "st_fio"));
    BoleznSotrModel->setEditStrategy(QSqlRelationalTableModel::OnManualSubmit);
    BoleznSotrModel->select();
    ComboSotrudnikiModel = new QSqlTableModel(this);
    ComboSotrudnikiModel->setTable("sotr_kafedri");

    ui->tableView_sotrBolez->setModel(BoleznSotrModel);
    ui->tableView_sotrBolez->setColumnHidden(0,true);
    ui->tableView_sotrBolez->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_sotrBolez->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_sotrBolez->setItemDelegate(new QSqlRelationalDelegate(ui->tableView_sotrBolez));
    ui->tableView_sotrBolez->setItemDelegateForColumn(3, new dateDelegate(ui->tableView_sotrBolez));
    ui->tableView_sotrBolez->setItemDelegateForColumn(4, new dateDelegate(ui->tableView_sotrBolez));
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);

   if (this->sotrudnik_id != 0)
    {
        for (int i = 0; i < ui->comboBox_sotrudnik->count(); i++)
        {
            if (ComboSotrudnikiModel->index(i, 0).data().toInt() == this->sotrudnik_id)
            {
                ui->comboBox_sotrudnik->setCurrentIndex(i);
                ui->comboBox_sotrudnik->view()->setCurrentIndex(ComboSotrudnikiModel->index(i, 0));
                break;
            }
        }
    }
}

void BolezniSotrudnikov::on_pushButton_del_clicked()
{
    if (!ui->tableView_sotrBolez->model()->index(ui->tableView_sotrBolez->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка удаления"), tr("Ни одной записи не выбрано"));
        return;
    }

    if (QMessageBox::information(this, tr("Информация"), tr("        Вы уверены, что хотите удалить запись? \n Все данные связанные с записью также будут удалены"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            if (! dal_main->checkConnection())
            {
                QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
                return;
            }
            BoleznSotrModel->removeRow(ui->tableView_sotrBolez->currentIndex().row());
            BoleznSotrModel->submitAll();
            BoleznSotrModel->select();
        }
        else
        {
            return;
        }

}

void BolezniSotrudnikov::on_pushButton_search_clicked()
{
    on_pushButton_clear_clicked();
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        this->sotrudnik_id = 0;
        ui->lineEdit_bolezn->clear();
        this->dateBeginning = QDate();
        this->isDate = false;
        BoleznSotrModel->setFilter("");
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
        ComboSotrudnikiModel->select();
        ui->comboBox_sotrudnik->setModel(ComboSotrudnikiModel);
        ui->comboBox_sotrudnik->setModelColumn(1);
        ui->comboBox_sotrudnik->setCurrentIndex(-1);
        this->sotrudnik_id = 0;
        ui->dateEdit_end->setDate(QDate::currentDate());
        connect(ui->lineEdit_bolezn,SIGNAL(editingFinished()),this,SLOT(formingSearch()));
    }
}

void BolezniSotrudnikov::on_pushButton_find_clicked()
{
    this->naideno.clear();
    ui->label_naideno->setVisible(true);
    if (this->isDate)
    {
        this->dateBeginning = ui->dateEdit_start->date();
        this->dateEnd = ui->dateEdit_end->date();
    }
    this->formingSearch();
}

void BolezniSotrudnikov::on_pushButton_clear_clicked()
{
    this->sotrudnik_id = 0;
    this->naideno.clear();
    ui->label_naideno->clear();
    ui->lineEdit_bolezn->clear();
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    BoleznSotrModel->setFilter("");
    BoleznSotrModel->select();
    ui->comboBox_sotrudnik->setModel(ComboSotrudnikiModel);
    ui->comboBox_sotrudnik->setModelColumn(1);
    ui->comboBox_sotrudnik->setCurrentIndex(-1);
    this->dateBeginning = QDate();
    this->isDate = false;
}

void BolezniSotrudnikov::on_comboBox_sotrudnik_activated(int index)
{
     this->sotrudnik_id = ui->comboBox_sotrudnik->model()->index(index,0).data().toInt();
}

void BolezniSotrudnikov::on_pushButton_save_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    for(int i=0;i<ui->tableView_sotrBolez->model()->rowCount(); i++)
    {
        if(ui->tableView_sotrBolez->model()->index(i,1).data().isNull()||ui->tableView_sotrBolez->model()->index(i,2).data().isNull()||ui->tableView_sotrBolez->model()->index(i,3).data().isNull() ||ui->tableView_sotrBolez->model()->index(i,4).data().isNull())
        {
            QMessageBox::information(this, tr("Информация"),tr("Поля 'Преподаватель', 'Болезнь' , 'Дата заболевания' и 'Дата выздоравления' должно быть заполнено"));
            return;
        }
    }
    BoleznSotrModel->submitAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}

void BolezniSotrudnikov::on_pushButton_cancel_clicked()
{
    BoleznSotrModel->revertAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные не сохранены"));
}

void BolezniSotrudnikov::on_dateEdit_start_dateChanged(const QDate &date)
{
    this->isDate=true;
}

void BolezniSotrudnikov::on_dateEdit_end_dateChanged(const QDate &date)
{
    this->isDate=true;
}

void BolezniSotrudnikov::on_pushButton_edit_clicked()
{
    if (!ui->tableView_sotrBolez->model()->index(ui->tableView_sotrBolez->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
        return;
    }
    ui->tableView_sotrBolez->edit(ui->tableView_sotrBolez->currentIndex());
}

void BolezniSotrudnikov::on_actionEdit_triggered()
{
    on_pushButton_edit_clicked();
}

void BolezniSotrudnikov::on_actionDelete_triggered()
{
    on_pushButton_del_clicked();
}

void BolezniSotrudnikov::on_tableView_sotrBolez_doubleClicked(const QModelIndex &index)
{
    on_pushButton_edit_clicked();
}
