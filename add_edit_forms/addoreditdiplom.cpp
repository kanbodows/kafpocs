#include "addoreditdiplom.h"
#include "ui_addoreditdiplom.h"
#include <QMessageBox>
#include"delegates/readonlydelegate.h"
AddOrEditDiplom::AddOrEditDiplom(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::AddOrEditDiplom)
{
    ui->setupUi(this);
    this->group=0;
    this->formirKom=0;
    this->temaRusKg=0;
    this->rukovod=0;
    ui->comboBox_temaKg->setEnabled(false);
    ui->tableWidget->setColumnHidden(5, true);
    ui->tableWidget->setColumnHidden(6, true);
    ui->tableWidget->setColumnHidden(7, true);
    ui->tableWidget->setColumnHidden(8, true);
    ui->tableWidget->addAction(ui->actionDelete);
    ui->tableWidget->setItemDelegateForColumn(0, new readOnlyDelegate(ui->tableWidget));
    ui->tableWidget->setItemDelegateForColumn(1, new readOnlyDelegate(ui->tableWidget));
    ui->tableWidget->setItemDelegateForColumn(2, new readOnlyDelegate(ui->tableWidget));
    ui->tableWidget->setItemDelegateForColumn(3, new readOnlyDelegate(ui->tableWidget));
    ui->tableWidget->setItemDelegateForColumn(4, new readOnlyDelegate(ui->tableWidget));


    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }

    ui->label_error->setVisible(false);
    dal_studentsControl = new Dal_studentsControl(this);
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }

    QSqlQueryModel *comboGroup = new QSqlQueryModel(this);
    comboGroup = dal_studentsControl->getComboGroup(0);
    ui->comboBox_group->setModel(comboGroup);
    ui->comboBox_group->setModelColumn(1);
    ui->comboBox_group->setCurrentIndex(-1);
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    QSqlQueryModel *ComboformirKomissii = new QSqlQueryModel(this);
    ComboformirKomissii = dal_studentsControl->getFormKom();
    ui->comboBox_formirKom->setModel(ComboformirKomissii);
    ui->comboBox_formirKom->setModelColumn(1);
    ui->comboBox_formirKom->setCurrentIndex(-1);
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    QSqlQueryModel *comboTemaKg = new QSqlQueryModel(this);
    comboTemaKg = dal_studentsControl->getTemaRusKG(0);
    ui->comboBox_temaKg->setModel(comboTemaKg);
    ui->comboBox_temaKg->setModelColumn(2);
    ui->comboBox_temaKg->setCurrentIndex(-1);
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    QSqlQueryModel *comboTemaRus = new QSqlQueryModel(this);
    comboTemaRus = dal_studentsControl->getTemaRusKG(0);
    ui->comboBox_temarus->setModel(comboTemaRus);
    ui->comboBox_temarus->setModelColumn(1);
    ui->comboBox_temarus->setCurrentIndex(-1);
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    QSqlQueryModel *comboRuk = new QSqlQueryModel(this);
    comboRuk = dal_studentsControl->getComboRuk();
    ui->comboBox_ruk->setModel(comboRuk);
    ui->comboBox_ruk->setModelColumn(1);
    ui->comboBox_ruk->setCurrentIndex(-1);

    css = new Styles;
}

AddOrEditDiplom::~AddOrEditDiplom()
{
    delete ui;
}

void AddOrEditDiplom::on_comboBox_group_activated(int index)
{
    this->group = ui->comboBox_group->model()->index(ui->comboBox_group->currentIndex(),0).data().toInt();
    dal_studentsControl = new Dal_studentsControl(this);
    rukModel = dal_studentsControl->getStudent(this->group);
    ui->tableView_stud->setModel(rukModel);
    ui->tableView_stud->setColumnHidden(0,true);
    ui->tableView_stud->setColumnHidden(2,true);
    ui->tableView_stud->setColumnHidden(3,true);
    ui->tableView_stud->setColumnHidden(4,true);
    ui->tableView_stud->setColumnHidden(5,true);
    ui->tableView_stud->setColumnHidden(6,true);
    ui->tableView_stud->setColumnHidden(7,true);
    ui->tableView_stud->setColumnHidden(8,true);
    ui->tableView_stud->setColumnHidden(9,true);
    ui->tableView_stud->setColumnHidden(10,true);
    ui->tableView_stud->setColumnHidden(11,true);
    ui->tableView_stud->setColumnHidden(12,true);
    ui->tableView_stud->setColumnHidden(13,true);
    ui->tableView_stud->setColumnHidden(14,true);
    ui->tableView_stud->setColumnHidden(15,true);
    ui->tableView_stud->setColumnHidden(16,true);
    ui->tableView_stud->setColumnHidden(17,true);
    ui->tableView_stud->setColumnHidden(18,true);
    ui->tableView_stud->setColumnHidden(19,true);
    ui->tableView_stud->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_stud->horizontalHeader()->setStretchLastSection(true);
}

void AddOrEditDiplom::on_comboBox_temarus_activated(int index)
{
    this->temaRusKg = ui->comboBox_temarus->model()->index(ui->comboBox_temarus->currentIndex(),0).data().toInt();
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    QSqlQueryModel *comboTemaKg = new QSqlQueryModel(this);
    comboTemaKg = dal_studentsControl->getTemaRusKG(this->temaRusKg);
    ui->comboBox_temaKg->setModel(comboTemaKg);
    ui->comboBox_temaKg->setModelColumn(2);

    ui->comboBox_temarus->setStyleSheet(css->pressedStyleSheet);
}

void AddOrEditDiplom::on_pushButton_addWidjet_clicked()
{
    bool error = false;

    if(!ui->comboBox_group->model()->index(ui->comboBox_group->currentIndex(),0).data().isValid())
    {
        QMessageBox::information(this,tr("Информация"),tr("Выберите группу"));
        return;
    }

    if(!ui->tableView_stud->model()->index(ui->tableView_stud->currentIndex().row(), 1).data().isValid())
    {
        error =true;
        ui->tableView_stud->setStyleSheet(css->wrongStyleSheet2);
    }

    if(!ui->comboBox_formirKom->model()->index(ui->comboBox_formirKom->currentIndex(),0).data().isValid())
    {
        error = true;
        ui->comboBox_formirKom->setStyleSheet(css->wrongStyleSheet2);
    }

    if(!ui->comboBox_ruk->model()->index(ui->comboBox_ruk->currentIndex(),0).data().isValid())
    {
        error = true;
        ui->comboBox_ruk->setStyleSheet(css->wrongStyleSheet2);

    }

    if(!ui->comboBox_temarus->model()->index(ui->comboBox_temarus->currentIndex(),0).data().isValid())
    {
        error = true;
        ui->comboBox_temarus->setStyleSheet(css->wrongStyleSheet2);
    }

    if(error == true)
    {
        ui->label_error->setVisible(true);
        return;
    }
    else
    {
        ui->label_error->setVisible(false);
        addRMToTableWidget(ui->tableView_stud->model()->index(ui->tableView_stud->currentIndex().row(),1).data().toString(),
                           ui->comboBox_temarus->model()->index(ui->comboBox_temarus->currentIndex(),1).data().toString(),
                           ui->comboBox_temaKg->model()->index(ui->comboBox_temaKg->currentIndex(),2).data().toString(),
                           ui->comboBox_formirKom->model()->index(ui->comboBox_formirKom->currentIndex(),1).data().toString(),
                           ui->comboBox_ruk->model()->index(ui->comboBox_ruk->currentIndex(),1).data().toString(),
                           ui->tableView_stud->model()->index(ui->tableView_stud->currentIndex().row(),0).data().toInt(),
                           ui->comboBox_temarus->model()->index(ui->comboBox_temarus->currentIndex(),0).data().toInt(),
                           ui->comboBox_formirKom->model()->index(ui->comboBox_formirKom->currentIndex(),0).data().toInt(),
                           ui->comboBox_ruk->model()->index(ui->comboBox_ruk->currentIndex(),0).data().toInt());
    }

}

void AddOrEditDiplom::addRMToTableWidget(QString FIOStud, QString temaRus, QString temaKg, QString GodFormKom, QString ruk, int FIOStud_id, int tema_id,int GodFormKom_id, int ruk_id)
{
    for(int j=0; j < ui->tableWidget->rowCount(); j++)// проход по записям в table
    {
        if(FIOStud_id == ui->tableWidget->item(j,5)->text().toInt())//если студент уже есть
        {
            QMessageBox::warning(this,tr("Ошибка"),tr("Данный студент уже есть в списке"));
            return;//выход
        }
    }
    int row = ui->tableWidget->rowCount();//кол-во записей в table
    ui->tableWidget->insertRow(row);//добавим новую запись
    //добавление новых элементов
    ui->tableWidget->setItem(row, 0, new QTableWidgetItem(FIOStud));
    ui->tableWidget->setItem(row, 1, new QTableWidgetItem(temaRus));
    ui->tableWidget->setItem(row, 2, new QTableWidgetItem((temaKg)));
    ui->tableWidget->setItem(row, 3, new QTableWidgetItem(GodFormKom));
    ui->tableWidget->setItem(row, 4, new QTableWidgetItem(ruk));
    ui->tableWidget->setItem(row, 5, new QTableWidgetItem(QString::number(FIOStud_id)));
    ui->tableWidget->setItem(row, 6, new QTableWidgetItem(QString::number(tema_id)));
    ui->tableWidget->setItem(row, 7, new QTableWidgetItem(QString::number(GodFormKom_id)));
    ui->tableWidget->setItem(row, 8, new QTableWidgetItem(QString::number(ruk_id)));
}

void AddOrEditDiplom::on_pushButton_addBase_clicked()
{
    if(ui->tableWidget->rowCount()==0)
    {
        QMessageBox::information(this,tr("Информация"),tr("Добавьте данные"));
        return;
    }
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    int rowcount = ui->tableWidget->rowCount();
    for(int i=0; i < rowcount; i++)
        dal_studentsControl->addDiplom(ui->tableWidget->item(i,5)->text().toInt(),
                                       ui->tableWidget->item(i,6)->text().toInt(),
                                       ui->tableWidget->item(i,7)->text().toInt(),
                                       ui->tableWidget->item(i,8)->text().toInt());
    this->close();
    QMessageBox::information(this, tr("Запись данных"), tr("Данные успешно добавлены"));
}

void AddOrEditDiplom::on_tableView_stud_clicked(const QModelIndex &index)
{
    ui->tableView_stud->setStyleSheet(css->pressedStyleSheet);
}

void AddOrEditDiplom::on_comboBox_formirKom_activated(int index)
{
    ui->comboBox_formirKom->setStyleSheet(css->pressedStyleSheet);
}

void AddOrEditDiplom::on_comboBox_ruk_activated(int index)
{
    ui->comboBox_ruk->setStyleSheet(css->pressedStyleSheet);
}

void AddOrEditDiplom::on_comboBox_temaKg_activated(int index)
{
    ui->comboBox_temaKg->setStyleSheet(css->pressedStyleSheet);
}

void AddOrEditDiplom::on_actionDelete_triggered()
{
     ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}
