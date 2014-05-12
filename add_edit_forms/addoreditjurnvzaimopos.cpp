#include "addoreditjurnvzaimopos.h"
#include "ui_addoreditjurnvzaimopos.h"
#include <QMessageBox>
AddOrEditJurnVzaimopos::AddOrEditJurnVzaimopos(QWidget *parent, int select, int id) :
    QDialog(parent),
    ui(new Ui::AddOrEditJurnVzaimopos)
{
    ui->setupUi(this);
    ui->lineEdit_Ocenka->setValidator( new QIntValidator(1, 1000000, this));
    ui->lineEdit_Ocenka->setPlaceholderText("Вводятся только цифры");
    this->sotr_ids=0;
    this->group_ids=0;
    this->discipl_ids=0;
    this->prover_ids = 0;
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    this->isEdit = false;

    ui->label_error->setVisible(false);

    dal_studentsControl = new Dal_studentsControl(this);

    QSqlQueryModel *comboPrepod = new QSqlQueryModel(this);
    comboPrepod = dal_studentsControl->getSotrKaf(this->sotr_ids);
    ui->comboBoxPrepod->setModel(comboPrepod);
    ui->comboBoxPrepod->setModelColumn(1);
    ui->comboBoxPrepod->setCurrentIndex(-1);

    QSqlQueryModel *comboDisciplina = new QSqlQueryModel(this);
    comboDisciplina = dal_studentsControl->getDiscipl();
    ui->comboBox_discipl->setModel(comboDisciplina);
    ui->comboBox_discipl->setModelColumn(1);
    ui->comboBox_discipl->setCurrentIndex(-1);

    QSqlQueryModel *comboGroup = new QSqlQueryModel(this);
    comboGroup = dal_studentsControl->getComboGroup(0);
    ui->comboBox_gruppa->setModel(comboGroup);
    ui->comboBox_gruppa->setModelColumn(1);
    ui->comboBox_gruppa->setCurrentIndex(-1);


    QSqlQueryModel *comboProver = new QSqlQueryModel(this);
    comboProver = dal_studentsControl->getSotrKaf(this->sotr_ids);
    ui->comboBox_prover->setModel(comboProver);
    ui->comboBox_prover->setModelColumn(1);
    ui->comboBox_prover->setCurrentIndex(-1);




    if (select == 1)
    {
        this->setWindowTitle(tr("Добавить новую запись"));
        ui->dateEdit->setDateTime(QDateTime::currentDateTime());
    }

    else
    {
        this->isEdit = true;
        this->setWindowTitle(tr("Редактирование записи"));
        this->rec_id = id;
        int ids_prepods;
        int ids_discipl;
        int ids_groupss;
        int ids_proveryai;


        QSqlQuery *query = new QSqlQuery;
        query = dal_studentsControl->getCurrentJurnalVzaimoPos(this->rec_id);
        ids_prepods = query->value(1).toInt();
        ids_discipl = query->value(2).toInt();
        ids_groupss = query->value(3).toInt();
        ids_proveryai = query->value(6).toInt();

        ui->lineEdit_Ocenka->setText(query->value(8).toString());
        ui->comboBox->setCurrentText(query->value(7).toString());
        this->sotr_ids = ids_prepods;
        this->discipl_ids = ids_discipl;
        this->group_ids = ids_groupss;
        this->prover_ids = ids_proveryai;
        this->tip_discipl = ui->comboBox->currentText();

        ui->plainTextEdit->setPlainText(query->value(5).toString());
        ui->dateEdit->setDateTime(query->value(4).toDateTime());

        for (int j = 0; j < ui->comboBoxPrepod->count(); j ++)
        {
            if (comboPrepod->index(j, 0).data().toInt() == ids_prepods)
            {
                ui->comboBoxPrepod->setCurrentIndex(j);
                ui->comboBoxPrepod->view()->setCurrentIndex(comboPrepod->index(j, 0));
                break;
            }
        }
        for (int i = 0; i < ui->comboBox_discipl->count(); i ++)
        {
            if (comboDisciplina->index(i, 0).data().toInt() == ids_discipl)
            {
                ui->comboBox_discipl->setCurrentIndex(i);
                ui->comboBox_discipl->view()->setCurrentIndex(comboDisciplina->index(i, 0));
                break;
            }
        }

        for (int i = 0; i < ui->comboBox_gruppa->count(); i ++)
        {
            if (comboGroup->index(i, 0).data().toInt() == ids_groupss)
            {
                ui->comboBox_gruppa->setCurrentIndex(i);
                ui->comboBox_gruppa->view()->setCurrentIndex(comboGroup->index(i, 0));
                break;
            }
        }

        for (int i = 0; i < ui->comboBox_prover->count(); i ++)
        {
            if (comboProver->index(i, 0).data().toInt() == ids_proveryai)
            {
                ui->comboBox_prover->setCurrentIndex(i);
                ui->comboBox_prover->view()->setCurrentIndex(comboProver->index(i, 0));
                break;
            }
        }
    }
}

AddOrEditJurnVzaimopos::~AddOrEditJurnVzaimopos()
{
    delete ui;
}

void AddOrEditJurnVzaimopos::on_comboBoxPrepod_activated(int index)
{
   this->sotr_ids = ui->comboBoxPrepod->model()->index(ui->comboBoxPrepod->currentIndex(), 0).data().toInt();
    QSqlQueryModel *comboProver = new QSqlQueryModel(this);
    comboProver = dal_studentsControl->getSotrKaf(this->sotr_ids);
    ui->comboBox_prover->setModel(comboProver);
    ui->comboBox_prover->setModelColumn(1);
    ui->comboBox_prover->setCurrentIndex(-1);
}

void AddOrEditJurnVzaimopos::on_pushButton_OK_clicked()
{
    bool error = false;
    if (ui->comboBoxPrepod->currentIndex()==-1)
    {
        error = true;
    }

    if (ui->comboBox_discipl->currentIndex()==-1)
    {
        error = true;
    }

    if (ui->comboBox_gruppa->currentIndex()==-1)
    {
        error = true;
    }

    if (ui->comboBox_prover->currentIndex()==-1)
    {
        error = true;
    }

    if (ui->comboBox->currentIndex()==-1)
    {
        error = true;
    }


    if (ui->lineEdit_Ocenka->text().isEmpty())
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
            if (dal_studentsControl->getJurnVzaimoposAdd(this->sotr_ids,\
                                                         this->discipl_ids,\
                                                         this->group_ids,
                                                         this->prover_ids,\
                                                         ui->dateEdit->dateTime(),\
                                                         ui->plainTextEdit->toPlainText(),
                                                         this->tip_discipl,
                                                         ui->lineEdit_Ocenka->text()))
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
            if (dal_studentsControl->JurnVzaimoposEdit(  this->rec_id,\
                                                         this->sotr_ids,\
                                                         this->discipl_ids,\
                                                         this->group_ids,
                                                         this->prover_ids,\
                                                         ui->dateEdit->dateTime(),\
                                                         ui->plainTextEdit->toPlainText(),
                                                         this->tip_discipl,\
                                                         ui->lineEdit_Ocenka->text()))
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

void AddOrEditJurnVzaimopos::on_comboBox_discipl_activated(int index)
{
    this->discipl_ids = ui->comboBox_discipl->model()->index(ui->comboBox_discipl->currentIndex(), 0).data().toInt();
}

void AddOrEditJurnVzaimopos::on_comboBox_gruppa_activated(int index)
{
    this->group_ids = ui->comboBox_gruppa->model()->index(ui->comboBox_gruppa->currentIndex(), 0).data().toInt();
}

void AddOrEditJurnVzaimopos::on_comboBox_prover_activated(int index)
{
    this->prover_ids = ui->comboBox_prover->model()->index(ui->comboBox_prover->currentIndex(), 0).data().toInt();
}

void AddOrEditJurnVzaimopos::on_pushButton_cancel_clicked()
{
    this->close();
}

void AddOrEditJurnVzaimopos::on_comboBox_activated(const QString &arg1)
{
    this->tip_discipl = ui->comboBox->currentText();
    qDebug()<<this->tip_discipl;
}

void AddOrEditJurnVzaimopos::on_lineEdit_Ocenka_textChanged(const QString &arg1)
{
    if(ui->lineEdit_Ocenka->text().toInt()>100)
    {
        QMessageBox::information(this, tr("Информация"), tr("Оценка должна быть меньше 100 баллов"));
        ui->lineEdit_Ocenka->setText("100");
    }

    if(ui->lineEdit_Ocenka->text().toInt()<=0)
    {
        QMessageBox::information(this, tr("Информация"), tr("Оценка должна быть больше 0 баллов"));
        ui->lineEdit_Ocenka->setText("1");
    }

}
