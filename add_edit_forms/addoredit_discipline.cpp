#include "addoredit_discipline.h"
#include "ui_addoredit_discipline.h"
#include <QFileDialog>
#include <QLineEdit>
addoredit_discipline::addoredit_discipline(QWidget *parent, bool isEdit, int id_discip) :
    QDialog(parent),
    id_discip(id_discip),
    isEdit(isEdit),
    ui(new Ui::addoredit_discipline)
{
    ui->setupUi(this);
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    dal_studentsControl = new Dal_studentsControl(this);
    ui->comboBox_semestr->setModel(dal_prepodcontrol->getcomboSemmestr());
    ui->comboBox_semestr->setModelColumn(2);
    ui->comboBox_semestr->setCurrentIndex(-1);
    ui->comboBox_avtor->setModel(dal_prepodcontrol->getcomboPrepod());
    ui->comboBox_avtor->setModelColumn(1);
    ui->comboBox_avtor->setCurrentIndex(-1);
    ui->comboBox_spec->setModel(dal_studentsControl->getSpec());
    ui->comboBox_spec->setModelColumn(1);
    ui->comboBox_spec->setCurrentIndex(-1);
    ui->comboBox_kafedra->setModel(dal_prepodcontrol->getcomboKafedry());
    ui->comboBox_kafedra->setModelColumn(1);
    ui->comboBox_kafedra->setCurrentIndex(-1);
    ui->comboBox_tip->setCurrentIndex(-1);
    ui->label_error->setVisible(false);
//    ui->comboBox_spec->setVisible(false);
    if(this->isEdit)
    {
        this->setWindowTitle("Редактирование дисциплины");
        QSqlQuery *query = new QSqlQuery;
        query = dal_prepodcontrol->getCurrentDiscipline(this->id_discip);

        for (int j = 0; j < ui->comboBox_semestr->count(); j++)
        {
            if (ui->comboBox_semestr->model()->index(j, 0).data().toInt() == query->value(2).toInt())
            {
                ui->comboBox_semestr->setCurrentIndex(j);
                ui->comboBox_semestr->view()->setCurrentIndex(ui->comboBox_semestr->model()->index(j, 0));
                break;
            }
        }
        for (int j = 0; j < ui->comboBox_kafedra->count(); j++)
        {
            if (ui->comboBox_kafedra->model()->index(j, 0).data().toInt() == query->value(10).toInt())
            {
                ui->comboBox_kafedra->setCurrentIndex(j);
                ui->comboBox_kafedra->view()->setCurrentIndex(ui->comboBox_kafedra->model()->index(j, 0));
                break;
            }
        }
        for (int j = 0; j < ui->comboBox_spec->count(); j++)
        {
            if (ui->comboBox_spec->model()->index(j, 0).data().toInt() == query->value(11).toInt())
            {
                ui->comboBox_spec->setCurrentIndex(j);
                ui->comboBox_spec->view()->setCurrentIndex(ui->comboBox_spec->model()->index(j, 0));
                break;
            }
        }
        ui->comboBox_tip->setCurrentText(query->value(12).toString());
        ui->discipline_lineEdit->setText(query->value(1).toString());
        ui->spinBox_tochki->setValue(query->value(3).toInt());
        ui->lineEdit_varianty->setText(query->value(4).toString());
        ui->checkBox_mashin->setChecked(query->value(5).toBool());
        ui->checkBox_blanoch->setChecked(query->value(6).toBool());
        ui->checkBox_ust->setChecked(query->value(7).toBool());
        ui->checkBox_pism->setChecked(query->value(8).toBool());
        for (int j = 0; j < ui->comboBox_avtor->count(); j++)
        {
            if (ui->comboBox_avtor->model()->index(j, 0).data().toInt() == query->value(9).toInt())
            {
                ui->comboBox_avtor->setCurrentIndex(j);
                ui->comboBox_avtor->view()->setCurrentIndex(ui->comboBox_avtor->model()->index(j, 0));
                break;
            }
        }
        ui->tableView_metodichki->setModel(dal_prepodcontrol->getMetodichki(this->id_discip));
        ui->tableView_knigi->setModel(dal_prepodcontrol->getKnigi(this->id_discip));
        ui->pushButton_ok->setText("Сохранить");
    }
    else
    {
        ui->groupBox_met->setVisible(false);
        ui->groupBox_knigi->setVisible(false);
        this->adjustSize();
    }
    ui->tableView_metodichki->setColumnHidden(0,true);
    ui->tableView_metodichki->setColumnHidden(6,true);
    ui->tableView_knigi->setColumnHidden(0,true);
    ui->tableView_knigi->setColumnHidden(6,true);
    ui->tableView_metodichki->addAction(ui->actionEditMet);
    ui->tableView_metodichki->addAction(ui->actionDelMet);
    ui->tableView_knigi->addAction(ui->actionEditKniga);
    ui->tableView_knigi->addAction(ui->actionDelKniga);
}

addoredit_discipline::~addoredit_discipline()
{
    delete ui;
}

void addoredit_discipline::on_pushButton_ok_clicked()
{
    if (ui->comboBox_semestr->currentIndex()==-1 || ui->comboBox_kafedra->currentIndex() ==-1 || ui->comboBox_tip->currentText().isEmpty())
    {
        ui->label_error->setVisible(true);
        return;
    }
    else
    {
        if (!this->isEdit)
        {
            if (! dal_main->checkConnection())
            {
                QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
                return;
            }
            this->id_discip = dal_prepodcontrol->addDiscipline(ui->discipline_lineEdit->text(),
                                                               ui->comboBox_semestr->model()->index(ui->comboBox_semestr->currentIndex(),0).data().toInt(),
                                                               ui->spinBox_tochki->value(),
                                                               ui->lineEdit_varianty->text(),
                                                               ui->checkBox_mashin->isChecked(),
                                                               ui->checkBox_blanoch->isChecked(),
                                                               ui->checkBox_ust->isChecked(),
                                                               ui->checkBox_pism->isChecked(),
                                                               ui->comboBox_avtor->model()->index(ui->comboBox_avtor->currentIndex(),0).data().toInt(),
                                                               ui->comboBox_kafedra->model()->index(ui->comboBox_kafedra->currentIndex(),0).data().toInt(),
                                                               ui->comboBox_spec->model()->index(ui->comboBox_spec->currentIndex(),0).data().toInt(),
                                                               ui->comboBox_tip->currentText());
            if (this->id_discip != -1)
            {
                QMessageBox::information(this, tr("Информация"), tr("Дисциплина успешно добавлена"));
                ui->pushButton_ok->setText("Сохранить");
                this->isEdit = true;
                ui->groupBox_met->setVisible(true);
                ui->groupBox_knigi->setVisible(true);
                this->adjustSize();
            }
            else
            {
                QMessageBox::warning(this, tr("Ошибка"), tr("Данные не были добавлены в базу данных"));
            }
        }
        else
        {
            if (! dal_main->checkConnection())
            {
                QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
                return;
            }
            if (dal_prepodcontrol->editDiscipline(this->id_discip,
                                                  ui->discipline_lineEdit->text(),
                                                  ui->comboBox_semestr->model()->index(ui->comboBox_semestr->currentIndex(),0).data().toInt(),
                                                  ui->spinBox_tochki->value(),
                                                  ui->lineEdit_varianty->text(),
                                                  ui->checkBox_mashin->isChecked(),
                                                  ui->checkBox_blanoch->isChecked(),
                                                  ui->checkBox_ust->isChecked(),
                                                  ui->checkBox_pism->isChecked(),
                                                  ui->comboBox_avtor->model()->index(ui->comboBox_avtor->currentIndex(),0).data().toInt(),
                                                  ui->comboBox_kafedra->model()->index(ui->comboBox_kafedra->currentIndex(),0).data().toInt(),
                                                  ui->comboBox_spec->model()->index(ui->comboBox_spec->currentIndex(),0).data().toInt(),
                                                  ui->comboBox_tip->currentText()))
            {
                QMessageBox::information(this, tr("Информация"), tr("Данные успешно отредактированы"));
            }
            else
            {
                QMessageBox::warning(this, tr("Ошибка"), tr("Данные не были изменены"));
            }
        }
        ui->label_error->setVisible(false);
    }
}

void addoredit_discipline::on_pushButton_choosePhoto_clicked()
{
    this->foto = "";
    QString filename = QFileDialog::getOpenFileName(this,tr("open foto"),"/home","Image files(*.png *.jpg *.bmp)");
    /*ui->label_foto_diploma->setPixmap(filename);
    ui->label_foto_diploma->setScaledContents(true);
    this->foto = filename;
    if(this->foto!="")
        ui->label_foto_diploma->setText("");*/
}

void addoredit_discipline::on_pushButton_addMet_clicked()
{
    try
    {
        addoredit_metodichki * form = new addoredit_metodichki(this,false,-1,this->id_discip);
        form->exec();
        ui->tableView_metodichki->setModel(dal_prepodcontrol->getMetodichki(this->id_discip));
        ui->tableView_metodichki->setColumnHidden(0,true);
        ui->tableView_metodichki->setColumnHidden(6,true);
    }
    catch (...)
    {
        return;
    }
}

void addoredit_discipline::on_pushButton_editMet_clicked()
{
    if (!ui->tableView_metodichki->model()->index(ui->tableView_metodichki->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка удаления"), tr("Ни одной записи не выбрано"));
        return;
    }
    try
    {
        addoredit_metodichki * form = new addoredit_metodichki(this,true,ui->tableView_metodichki->model()->index(ui->tableView_metodichki->currentIndex().row(),6).data().toInt(),this->id_discip);
        form->exec();
        ui->tableView_metodichki->setModel(dal_prepodcontrol->getMetodichki(this->id_discip));
    }
    catch (...)
    {
        return;
    }
}

void addoredit_discipline::on_pushButton_delMet_clicked()
{
    if (QMessageBox::warning(this, tr("Проверка удаления"), "Вы действительно хотате удалить выбранную методичку", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        if(dal_prepodcontrol->deleteMetodichka(ui->tableView_metodichki->model()->index(ui->tableView_metodichki->currentIndex().row(),0).data().toInt()))
        {
            QMessageBox::information(this, tr("Информация"), tr("Методичка успешно удалена"));
            ui->tableView_metodichki->setModel(dal_prepodcontrol->getMetodichki(this->id_discip));
        }
    }
}

void addoredit_discipline::on_pushButton_addKnigi_clicked()
{
    try
    {
        addoredit_knigi * form = new addoredit_knigi(this,false,-1,this->id_discip);
        form->exec();
        ui->tableView_knigi->setModel(dal_prepodcontrol->getKnigi(this->id_discip));
        ui->tableView_knigi->setColumnHidden(0,true);
        ui->tableView_knigi->setColumnHidden(6,true);
    }
    catch (...)
    {
        return;
    }
}

void addoredit_discipline::on_pushButton_editKnigi_clicked()
{
    if (!ui->tableView_knigi->model()->index(ui->tableView_knigi->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка удаления"), tr("Ни одной записи не выбрано"));
        return;
    }
    try
    {
        addoredit_knigi * form = new addoredit_knigi(this,true,ui->tableView_knigi->model()->index(ui->tableView_knigi->currentIndex().row(),0).data().toInt(),this->id_discip);
        form->exec();
        ui->tableView_knigi->setModel(dal_prepodcontrol->getKnigi(this->id_discip));
    }
    catch (...)
    {
        return;
    }
}

void addoredit_discipline::on_pushButton_delKnigi_clicked()
{
    if (QMessageBox::warning(this, tr("Проверка удаления"), "Вы действительно хотате удалить выбранную книгу", QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
    {
        if(dal_prepodcontrol->deleteKniga(ui->tableView_knigi->model()->index(ui->tableView_knigi->currentIndex().row(),0).data().toInt()))
        {
            QMessageBox::information(this, tr("Информация"), tr("Книга успешно удалена"));
            ui->tableView_knigi->setModel(dal_prepodcontrol->getKnigi(this->id_discip));
        }
    }
}

void addoredit_discipline::on_pushButton_cancel_clicked()
{
    this->close();
}

void addoredit_discipline::on_tableView_metodichki_doubleClicked(const QModelIndex &index)
{
    on_pushButton_editMet_clicked();
}

void addoredit_discipline::on_tableView_knigi_doubleClicked(const QModelIndex &index)
{
    on_pushButton_editKnigi_clicked();
}

void addoredit_discipline::on_actionEditMet_triggered()
{
    on_pushButton_editMet_clicked();
}

void addoredit_discipline::on_actionEditKniga_triggered()
{
    on_pushButton_editKnigi_clicked();
}

void addoredit_discipline::on_actionDelMet_triggered()
{
    on_pushButton_delMet_clicked();
}

void addoredit_discipline::on_actionDelKniga_triggered()
{
    on_pushButton_delKnigi_clicked();
}
