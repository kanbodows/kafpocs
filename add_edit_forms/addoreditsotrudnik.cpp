#include "addoreditsotrudnik.h"
#include "ui_addoreditsotrudnik.h"
#include<QMessageBox>

AddOrEditSotrudnik::AddOrEditSotrudnik(QWidget *parent, int select, int id, int viewsv) :
    QDialog(parent),
    ui(new Ui::AddOrEditSotrudnik)
{
    ui->setupUi(this);

    ui->lineEdit_StajVKGTU->setValidator( new QIntValidator(1, 1000000, this));
    ui->lineEdit_StajVKGTU->setPlaceholderText("Вводятся только цифры");

    ui->lineEdit_ObshiiStaj->setValidator( new QIntValidator(1, 1000000, this));
    ui->lineEdit_ObshiiStaj->setPlaceholderText("Вводятся только цифры");

    ui->lineEdit_pedStaj->setValidator( new QIntValidator(1, 1000000, this));
    ui->lineEdit_pedStaj->setPlaceholderText("Вводятся только цифры");

    ui->lineEdit_Email->setPlaceholderText("Пример: serega_90.18@mail.ru");
    this->doljnost=0;
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    this->isEdit = false;
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    QSqlQueryModel *comboDoljnost = new QSqlQueryModel(this);
    comboDoljnost = dal_prepodcontrol->getDoljnost();
    ui->dateEdit_dateReg->setDate(QDate::currentDate());
    ui->comboBox_dolj->setModel(comboDoljnost);
    ui->comboBox_dolj->setModelColumn(1);
    ui->comboBox_dolj->setCurrentIndex(-1);
    ui->label_error->setVisible(false);

    if (select == 1)
    {
        this->setWindowTitle(tr("Добавить новую запись"));
        ui->pushButton_changePassword->setVisible(false);
    }
    else
    {
        this->isEdit = true;
        if(viewsv==3)
        {
            this->setWindowTitle(tr("Подробная информация о сотруднике"));
            ui->pushButton_changePassword->setVisible(false);
            ui->lineEdit_Login->setVisible(false);
            ui->lineEdit_parol->setVisible(false);
            ui->label_login->setVisible(false);
            ui->label_parol->setVisible(false);

            ui->checkBox_OnlyGak->setEnabled(false);

            ui->checkBox_status->setEnabled(false);

            ui->label_DomAddress->setText("Домашний адрес");
            ui->lineEdit_DomAdress->setReadOnly(true);

            ui->label_mail->setText("E-mail");
            ui->lineEdit_Email->setReadOnly(true);

            ui->labelFIO->setText("ФИО");
            ui->lineEdit_FIO->setReadOnly(true);

            ui->label_mestoRab->setText("Место работы");
            ui->lineEdit_MestoRaboti->setReadOnly(true);

            ui->label_mestorojd->setText("Место рождение");
            ui->lineEdit_MestoRojdenia->setReadOnly(true);

            ui->label_nacion->setText("Национальность");
            ui->lineEdit_Nacionalnost->setReadOnly(true);

            ui->label_obrazov->setText("Образование");
            ui->lineEdit_Obrazovanie->setReadOnly(true);

            ui->label_ObshiiStaj->setText("Общий стаж");
            ui->lineEdit_ObshiiStaj->setReadOnly(true);

            ui->label_PedStaj->setText("Педагогический стаж");
            ui->lineEdit_pedStaj->setReadOnly(true);

            ui->label_seriaPasporta->setText("Серия паспорта");
            ui->lineEdit_SeriaPasporta->setReadOnly(true);

            ui->label_SocZash->setText("Соц. Защита");
            ui->lineEdit_SocZashita->setReadOnly(true);

            ui->label_StajKGTU->setText("Стаж в КГТУ");
            ui->lineEdit_StajVKGTU->setReadOnly(true);

            ui->label_Stavka->setText("Ставка");
            ui->lineEdit_Stavka->setReadOnly(true);

            ui->label_tel->setText("Телефон");
            ui->lineEdit_telephone->setReadOnly(true);

            ui->label_UchZvan->setText("Ученое звание");
            ui->lineEdit_UchebnoZvanie->setReadOnly(true);

            ui->label_sostav->setText("Состав");
            ui->comboBox_sostav->setEnabled(false);

        }

        else
        {
            this->setWindowTitle(tr("Редактирование записи"));
            ui->pushButton_changePassword->setVisible(true);
            ui->lineEdit_Login->setVisible(false);
            ui->lineEdit_parol->setVisible(false);
            ui->label_login->setVisible(false);
            ui->label_parol->setVisible(false);
        }
        this->rec_id = id;
        int ids_doljnost;

        QSqlQuery *query = new QSqlQuery;
        query = dal_prepodcontrol->getCurrentSotrudnik(this->rec_id);
        ids_doljnost = query->value(27).toInt();
        comboDoljnost = dal_prepodcontrol->getDoljnost();
        ui->comboBox_dolj->setModel(comboDoljnost);
        ui->comboBox_dolj->setModelColumn(1);
        ui->comboBox_dolj->setCurrentIndex(-1);

        ui->lineEdit_FIO->setText(query->value(1).toString());
        ui->lineEdit_telephone->setText(query->value(2).toString());
        ui->lineEdit_Email->setText(query->value(3).toString());
        ui->lineEdit_Obrazovanie->setText(query->value(4).toString());
        ui->lineEdit_ObshiiStaj->setText(query->value(5).toString());
        ui->lineEdit_StajVKGTU->setText(query->value(6).toString());
        ui->lineEdit_pedStaj->setText(query->value(7).toString());
        ui->lineEdit_UchebnoZvanie->setText(query->value(8).toString());
        ui->lineEdit_Stavka->setValue(query->value(11).toDouble());
        ui->lineEdit_Login->setText(query->value(12).toString());
        ui->lineEdit_parol->setText(query->value(13).toString());
        ui->dateEdit_dateReg->setDate(query->value(14).toDate());
        ui->dateEdit_dateRojdenie->setDate(query->value(15).toDate());
        if(query->value(17).toInt()==0)
            ui->checkBox_status->setChecked(false);
        else
            ui->checkBox_status->setChecked(true);
        ui->lineEdit_MestoRojdenia->setText(query->value(18).toString());
        ui->lineEdit_MestoRaboti->setText(query->value(19).toString());
        ui->lineEdit_Nacionalnost->setText(query->value(20).toString());
        ui->lineEdit_SeriaPasporta->setText(query->value(21).toString());
        ui->lineEdit_DomAdress->setText(query->value(22).toString());
        ui->lineEdit_SocZashita->setText(query->value(25).toString());
        if(query->value(23).toString()=="холост")
            ui->comboBox_SemeqnoePoloj->setCurrentIndex(0);
        else
            ui->comboBox_SemeqnoePoloj->setCurrentIndex(1);

        if(query->value(26).toInt()==0)
            ui->checkBox_OnlyGak->setChecked(false);
        else
            ui->checkBox_OnlyGak->setChecked(true);
        for (int j = 0; j < ui->comboBox_dolj->count(); j ++)
        {
            if (comboDoljnost->index(j, 0).data().toInt() == ids_doljnost)
            {
                ui->comboBox_dolj->setCurrentIndex(j);
                ui->comboBox_dolj->view()->setCurrentIndex(comboDoljnost->index(j, 0));
                break;
            }
        }
        ui->comboBox_sostav->setCurrentText(query->value(28).toString());
    }

}

AddOrEditSotrudnik::~AddOrEditSotrudnik()
{
    delete ui;
}

void AddOrEditSotrudnik::on_pushButton_OK_clicked()
{
    bool error = false;
    if (ui->lineEdit_FIO->text()=="")
    {
        error = true;
    }

    if (ui->lineEdit_Obrazovanie->text()=="")
    {
        error = true;
    }


    if(ui->lineEdit_Stavka->text().isEmpty())
    {
        error = true;
    }

    if(ui->lineEdit_DomAdress->text().isEmpty())
    {
        error = true;
    }

    if(ui->lineEdit_DomAdress->text().isEmpty())
    {
        error = true;
    }

    if(ui->lineEdit_MestoRaboti->text().isEmpty())
    {
        error = true;
    }

    if(ui->lineEdit_Nacionalnost->text().isEmpty())
    {
        error = true;
    }

    if(ui->lineEdit_SeriaPasporta->text().isEmpty())
    {
        error = true;
    }

    if(ui->lineEdit_SocZashita->text().isEmpty())
    {
        error = true;
    }

    if(ui->comboBox_dolj->currentIndex()==-1)
    {
        error = true;
    }

    if(ui->lineEdit_Login->text().isEmpty())
    {
        error = true;
    }

    if(ui->lineEdit_parol->text().isEmpty())
    {
        error = true;
    }

    int prov=0;
    prov = ui->lineEdit_Email->text().count("@");
    if(prov==0)
    {
        QMessageBox::information(this,tr("Информация"), tr("Поле ' адрес почты' введено не верно"));
        return;
    }
    else
    {
        if (error==true)
        {
            ui->label_error->setVisible(true);
            return;
        }
        else
        {
            if (! this->isEdit)
            {
                if (! dal_main->checkConnection())
                {
                    QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
                    return;
                }
                if (dal_prepodcontrol->SotrudnikAdd(ui->lineEdit_FIO->text(),\
                                                    ui->lineEdit_telephone->text(),\
                                                    ui->lineEdit_Email->text(),\
                                                    ui->lineEdit_Obrazovanie->text(),\
                                                    ui->lineEdit_ObshiiStaj->text().toInt(),\
                                                    ui->lineEdit_StajVKGTU->text().toInt(),\
                                                    ui->lineEdit_pedStaj->text().toInt(),\
                                                    ui->lineEdit_UchebnoZvanie->text(),\
                                                    ui->comboBox_dolj->model()->index(ui->comboBox_dolj->currentIndex(), 0).data().toInt(),\
                                                    ui->lineEdit_Stavka->value(),\
                                                    ui->lineEdit_Login->text(),\
                                                    ui->lineEdit_parol->text(),\
                                                    ui->dateEdit_dateRojdenie->date(),\
                                                    ui->dateEdit_dateReg->date(),\
                                                    ui->checkBox_status->isChecked(),\
                                                    ui->lineEdit_MestoRojdenia->text(),\
                                                    ui->lineEdit_MestoRaboti->text(),\
                                                    ui->lineEdit_Nacionalnost->text(),\
                                                    ui->lineEdit_SeriaPasporta->text(),\
                                                    ui->lineEdit_DomAdress->text(),\
                                                    ui->comboBox_SemeqnoePoloj->currentText(),\
                                                    ui->lineEdit_SocZashita->text(),\
                                                    ui->checkBox_OnlyGak->isChecked(),
                                                    ui->comboBox_sostav->currentText()))
                {
                    this->close();
                    QMessageBox::information(this, tr("Информация"), tr("Запись успешно добавлена"));
                }
                else
                {
                    this->close();
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
                if (dal_prepodcontrol->editSotrudnik(this->rec_id,\
                                                     ui->lineEdit_FIO->text(),\
                                                     ui->lineEdit_telephone->text(),\
                                                     ui->lineEdit_Email->text(),\
                                                     ui->lineEdit_Obrazovanie->text(),\
                                                     ui->lineEdit_ObshiiStaj->text().toInt(),\
                                                     ui->lineEdit_StajVKGTU->text().toInt(),\
                                                     ui->lineEdit_pedStaj->text().toInt(),\
                                                     ui->lineEdit_UchebnoZvanie->text(),\
                                                     ui->comboBox_dolj->model()->index(ui->comboBox_dolj->currentIndex(), 0).data().toInt(),\
                                                     ui->lineEdit_Stavka->value(),\
                                                     ui->lineEdit_Login->text(),\
                                                     ui->lineEdit_parol->text(),\
                                                     ui->dateEdit_dateRojdenie->date(),\
                                                     ui->dateEdit_dateReg->date(),\
                                                     ui->checkBox_status->isChecked(),\
                                                     ui->lineEdit_MestoRojdenia->text(),\
                                                     ui->lineEdit_MestoRaboti->text(),\
                                                     ui->lineEdit_Nacionalnost->text(),\
                                                     ui->lineEdit_SeriaPasporta->text(),\
                                                     ui->lineEdit_DomAdress->text(),\
                                                     ui->comboBox_SemeqnoePoloj->currentText(),\
                                                     ui->lineEdit_SocZashita->text(),\
                                                     ui->checkBox_OnlyGak->isChecked(),
                                                     ui->comboBox_sostav->currentText()))
                {
                    this->close();
                    QMessageBox::information(this, tr("Информация"), tr("Данные успешно отредактированы"));
                }
                else
                {
                    this->close();
                    QMessageBox::information(this, tr("Информация"), tr("Данные не были изменены"));
                }
            }
        }
    }
}

void AddOrEditSotrudnik::on_pushButton_Cancel_clicked()
{
    this->close();
}

void AddOrEditSotrudnik::on_pushButton_changePassword_clicked()
{
    try
    {
        smenaForm = new SmenaParrolya(this, this->rec_id);
        smenaForm->exec();
    }
    catch (...)
    {
        return;
    }
}
