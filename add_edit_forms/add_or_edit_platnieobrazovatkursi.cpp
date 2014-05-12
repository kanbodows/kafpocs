#include "add_or_edit_platnieobrazovatkursi.h"
#include "ui_add_or_edit_platnieobrazovatkursi.h"
#include <QMessageBox>

add_or_edit_PlatnieObrazovatKursi::add_or_edit_PlatnieObrazovatKursi(QWidget *parent, int select, int id) :
    QDialog(parent),
    ui(new Ui::add_or_edit_PlatnieObrazovatKursi)
{
    ui->setupUi(this);
    ui->lineEdit_KolvoChasov->setValidator( new QIntValidator(1, 1000000, this));
    ui->lineEdit_KolvoChasov->setPlaceholderText("Вводятся только цифры");

    ui->lineEdit_Oplata->setValidator( new QIntValidator(1, 1000000, this));
    ui->lineEdit_Oplata->setPlaceholderText("Вводятся только цифры");

    this->sotr_ids=0;

    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    this->isEdit = false;

    ui->label_error->setVisible(false);

    dal_prepodcontrol = new Dal_prepodcontrol(this);

    QSqlQueryModel *comboPrepod = new QSqlQueryModel(this);
    comboPrepod = dal_prepodcontrol->getSotrModels("",0,"");
    ui->comboBoxPrepod->setModel(comboPrepod);
    ui->comboBoxPrepod->setModelColumn(1);
    ui->comboBoxPrepod->setCurrentIndex(-1);

    if (select == 1)
    {
        this->setWindowTitle(tr("Добавить новую запись"));
    }

    else
    {
        this->isEdit = true;
        this->setWindowTitle(tr("Редактирование записи"));
        this->rec_id = id;
        int ids_prepods;
        this->sotr_ids = ids_prepods;

        QSqlQuery *query = new QSqlQuery;
        query = dal_prepodcontrol->getCurrentPlatnuyObrazovatUslugu(this->rec_id);
        ids_prepods = query->value(1).toInt();
        ui->lineEditNazvanieKursa->setText(query->value(2).toString());
        ui->lineEdit_KolvoChasov->setText(query->value(4).toString());
        ui->comboBox_tipZanyatia->setCurrentText(query->value(5).toString());
        ui->comboBox_tipKursa->setCurrentText(query->value(8).toString());
        ui->lineEdit_periodObuch->setText(query->value(6).toString());
        ui->lineEdit_Oplata->setText(query->value(7).toString());
        this->sotr_ids = ids_prepods;


        for (int j = 0; j < ui->comboBoxPrepod->count(); j ++)
        {
            if (comboPrepod->index(j, 0).data().toInt() == ids_prepods)
            {
                ui->comboBoxPrepod->setCurrentIndex(j);
                ui->comboBoxPrepod->view()->setCurrentIndex(comboPrepod->index(j, 0));
                break;
            }
        }
    }
}

add_or_edit_PlatnieObrazovatKursi::~add_or_edit_PlatnieObrazovatKursi()
{
    delete ui;
}

void add_or_edit_PlatnieObrazovatKursi::on_comboBoxPrepod_activated(int index)
{
    this->sotr_ids = ui->comboBoxPrepod->model()->index(ui->comboBoxPrepod->currentIndex(), 0).data().toInt();
}

void add_or_edit_PlatnieObrazovatKursi::on_pushButton_Cancel_clicked()
{
    this->close();
}

void add_or_edit_PlatnieObrazovatKursi::on_pushButtonOk_clicked()
{
    bool error = false;
    if (ui->comboBoxPrepod->currentIndex()==-1)
    {
        error = true;
    }

    if (ui->comboBox_tipKursa->currentIndex()==-1)
    {
        error = true;
    }

    if (ui->comboBox_tipZanyatia->currentIndex()==-1)
    {
        error = true;
    }

    if (ui->lineEditNazvanieKursa->text().isEmpty())
    {
        error = true;
    }

    if (ui->lineEdit_KolvoChasov->text().isEmpty())
    {
        error = true;
    }

    if (ui->lineEdit_Oplata->text().isEmpty())
    {
        error = true;
    }

    if (ui->lineEdit_periodObuch->text().isEmpty())
    {
        error = true;
    }

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
            if (dal_prepodcontrol->PlatnieObrazovatslugiAdd(this->sotr_ids,\
                                                         ui->lineEditNazvanieKursa->text(),\
                                                         ui->lineEdit_KolvoChasov->text(),
                                                         ui->comboBox_tipZanyatia->currentText(),\
                                                         ui->lineEdit_periodObuch->text(),\
                                                         ui->lineEdit_Oplata->text(),
                                                         ui->comboBox_tipKursa->currentText()))
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
            if (dal_prepodcontrol->PlatnieObrazovatslugiEdit(this->rec_id,\
                                                             this->sotr_ids,\
                                                               ui->lineEditNazvanieKursa->text(),\
                                                               ui->lineEdit_KolvoChasov->text(),
                                                               ui->comboBox_tipZanyatia->currentText(),\
                                                               ui->lineEdit_periodObuch->text(),\
                                                               ui->lineEdit_Oplata->text(),\
                                                               ui->comboBox_tipKursa->currentText()))
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
