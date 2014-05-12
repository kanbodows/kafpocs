#include "jurnalvzaimoposesheniya.h"
#include "ui_jurnalvzaimoposesheniya.h"
#include "delegates/transactiondatedelegate.h"
#include "delegates/word_wrap_delegate.h"
#include <QMessageBox>
bool JurnalVzaimoposesheniya::isOpen = false;
JurnalVzaimoposesheniya::JurnalVzaimoposesheniya(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::JurnalVzaimoposesheniya)
{
    ui->setupUi(this);
    this->count=0;
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
    this->vidim=false;

    this->dateBeginning = ui->dateEdit->date();
    this->dateEnd = QDate::currentDate();

    ui->dateEdit_end->setDate(QDate::currentDate());
    this->isDate = false;
    this->fioProver = "";

    dal_studentsControl = new Dal_studentsControl(this);

    JurnalVzaimoposModels = dal_studentsControl->getJurnVzaimopos(this->fioProver, this->dateBeginning, this->dateEnd);
    ui->tableView_jurnalVzaimopos->setModel(JurnalVzaimoposModels);
    ui->tableView_jurnalVzaimopos->setColumnHidden(0,true);
    ui->tableView_jurnalVzaimopos->setColumnHidden(1,true);
    ui->tableView_jurnalVzaimopos->setColumnHidden(2,true);
    ui->tableView_jurnalVzaimopos->setColumnHidden(3,true);
    ui->tableView_jurnalVzaimopos->setColumnHidden(4,true);

    ui->tableView_jurnalVzaimopos->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_jurnalVzaimopos->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_jurnalVzaimopos->verticalHeader()->hide();
    ui->tableView_jurnalVzaimopos->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    //   ui->tableView_jurnalVzaimopos->setItemDelegateForColumn(9, new transactionDateDelegate(ui->tableView_jurnalVzaimopos));
    ui->tableView_jurnalVzaimopos->setItemDelegateForColumn(10,new TextEditDelegate(ui->tableView_jurnalVzaimopos));
    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ui->tableView_jurnalVzaimopos->addAction(ui->actionEdit);
    ui->tableView_jurnalVzaimopos->addAction(ui->actionDelete);
    ui->pushButtonPrint->setEnabled(false);
}

JurnalVzaimoposesheniya::~JurnalVzaimoposesheniya()
{
    isOpen = false;
    delete ui;
}

void JurnalVzaimoposesheniya::refreshData()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    ui->tableView_jurnalVzaimopos->setModel(dal_studentsControl->getJurnVzaimopos(this->fioProver, this->dateBeginning, this->dateEnd));

    if(this->vidim==true)
    {
        if(ui->tableView_jurnalVzaimopos->model()->rowCount()>0)
        {
            this->naideno.append("                                   Найдено: " + QString::number(ui->tableView_jurnalVzaimopos->model()->rowCount()));
            ui->label_result->setVisible(false);
            ui->label_naideno->setVisible(true);
            ui->label_naideno->setText(this->naideno);
            this->naideno.clear();
        }
        else
        {
            ui->label_naideno->setVisible(true);
            this->naideno.append("                                   Найдено: 0");
            ui->label_result->setVisible(true);
            ui->label_naideno->setText(this->naideno);
            this->naideno.clear();
        }
    }
}

void JurnalVzaimoposesheniya::on_pushButton_del_clicked()
{
    if(ui->tableView_jurnalVzaimopos->model()->index(ui->tableView_jurnalVzaimopos->currentIndex().row(),1).data().toInt()==dal_main->getCurrentEmployee())
    {
        ui->pushButton_del->setEnabled(true);
        ui->pushButton_edit->setEnabled(true);
        if (! ui->tableView_jurnalVzaimopos->model()->index(ui->tableView_jurnalVzaimopos->currentIndex().row(), 0).data().toInt())
        {
            QMessageBox::information(this, tr("Информация"), tr("Ни одной записи не выбрано"));
            return;
        }
        if (QMessageBox::information(this, tr("Информация"), tr("Вы уверены, что хотите удалить запись? \n Восстановить запись невозможно"),
                                     QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            if (! dal_main->checkConnection())
            {
                QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
                return;
            }
            if (dal_studentsControl->deleteJurnVzaimPos(ui->tableView_jurnalVzaimopos->model()->index(ui->tableView_jurnalVzaimopos->currentIndex().row(), 0).data().toInt()))
            {
                this->refreshData();
                QMessageBox::information(this, tr("Удаление"), tr("Данные успешно удалены"));
            }
            else
            {
                QMessageBox::information(this, tr("Удаление"), tr("Не удалось удалить данные, попробуйте еще раз"));
                this->refreshData();
            }
        }
        else
        {
            this->refreshData();
            return;
        }
    }
    else
    {
        ui->pushButton_del->setEnabled(false);
        ui->pushButton_edit->setEnabled(false);
        QMessageBox::information(this,tr("Внимание"),tr("Вы не можете удалить не свою запись"));
    }

}

void JurnalVzaimoposesheniya::on_pushButton_update_clicked()
{
    this->refreshData();
}

void JurnalVzaimoposesheniya::on_pushButton_add_clicked()
{
    try
    {
        addOrEditJurnVzaimoposform = new AddOrEditJurnVzaimopos(this, INSERT);
        addOrEditJurnVzaimoposform->exec();
    }
    catch (...)
    {
        return;
    }
    this->refreshData();
}

void JurnalVzaimoposesheniya::on_pushButton_edit_clicked()
{
    if(ui->tableView_jurnalVzaimopos->model()->index(ui->tableView_jurnalVzaimopos->currentIndex().row(),1).data().toInt()==dal_main->getCurrentEmployee())
    {
        ui->pushButton_del->setEnabled(true);
        ui->pushButton_edit->setEnabled(true);
        if (! ui->tableView_jurnalVzaimopos->model()->index(ui->tableView_jurnalVzaimopos->currentIndex().row(), 0).data().toInt())
        {
            QMessageBox::information(this, tr("Информация"), tr("Выберите запись из таблицы"));
            return;
        }
        int id = ui->tableView_jurnalVzaimopos->model()->index(ui->tableView_jurnalVzaimopos->currentIndex().row(), 0).data().toInt();
        try
        {
            addOrEditJurnVzaimoposform = new AddOrEditJurnVzaimopos(this, EDIT, id);
            addOrEditJurnVzaimoposform->exec();
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

void JurnalVzaimoposesheniya::on_actionEdit_triggered()
{
    on_pushButton_edit_clicked();
}

void JurnalVzaimoposesheniya::on_actionDelete_triggered()
{
    on_pushButton_del_clicked();
}

void JurnalVzaimoposesheniya::on_tableView_jurnalVzaimopos_doubleClicked(const QModelIndex &index)
{
    on_pushButton_edit_clicked();
}

void JurnalVzaimoposesheniya::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {
        ui->groupBox_search->setVisible(false);
        this->fioProver="";
        this->dateBeginning = QDate();
        this->dateEnd = ui->dateEdit_end->date();
        this->isDate = false;

        ui->lineEdit_prepod->clear();
        ui->pushButtonPrint->setEnabled(false);
        this->refreshData();
    }
    else
    {
        ui->groupBox_search->setVisible(true);
        ui->dateEdit_end->setDate(QDate::currentDate());
    }

    on_pushButton_clear_clicked();
}

void JurnalVzaimoposesheniya::on_pushButtonPrint_clicked()
{
    MyPrint myPrint;
    RowList list;
    QString title = this->windowTitle();

    list[5] = tr("ФИО преподавателя");
    list[6] = tr("Вид дисциплины");
    list[7] = tr("Дисциплина");
    list[8] = tr("Группа");
    list[9] = tr("Дата посещения");
    list[10] = tr("Отзыв");
    list[11] = tr("Проверяющий");
    list[12] = tr("Оценка");

    QString prover = ui->tableView_jurnalVzaimopos->model()->index(0,11).data().toString();
    JurnalVzaimoposModels = dal_studentsControl->getJurnVzaimopos(this->fioProver, this->dateBeginning, this->dateEnd );
    ui->tableView_jurnalVzaimopos->setModel(JurnalVzaimoposModels);
    body = myPrint.print((QSqlQueryModel*)JurnalVzaimoposModels, list, "Журнал взаимопосещения на " +  ui->dateEdit->date().toString("yyyy") + " - " + ui->dateEdit_end->date().toString("yyyy") + " учебный год", "Подпись проверяющего ( " + prover +  " ):____________________________________", 0);

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("file.pdf");
    QPrintPreviewDialog *preview = new QPrintPreviewDialog (&printer);
    preview->setAttribute(Qt::WA_DeleteOnClose);
    preview->setWindowFlags(Qt::Widget);
    QList<QPrintPreviewWidget*> list2 = preview->findChildren<QPrintPreviewWidget*>();
    if(!list2.isEmpty()) // paranoiac safety check
        list2.first()->setZoomMode(QPrintPreviewWidget::FitToWidth);
    connect(preview, SIGNAL(paintRequested(QPrinter *)), SLOT(printDocument(QPrinter *)));
    QLayoutItem* item;
    //    while ((item = ui->verticalLayout->takeAt(0) ) != NULL )
    //    {
    //        delete item->widget();
    //        delete item;
    //    }
    //    ui->verticalLayout->addWidget(preview);

    preview->exec();
}

void JurnalVzaimoposesheniya::printDocument(QPrinter *printer)
{
    body->print(printer);
}

void JurnalVzaimoposesheniya::on_pushButton_find_clicked()
{
    this->vidim = true;
    if(!ui->lineEdit_prepod->text().isEmpty() && ui->tableView_jurnalVzaimopos->model()->rowCount()>0)
        ui->pushButtonPrint->setEnabled(true);
    else
        ui->pushButtonPrint->setEnabled(false);
    this->fioProver = ui->lineEdit_prepod->text();
    if (this->isDate==true)
    {
        this->dateBeginning = ui->dateEdit->date();
        this->dateEnd = ui->dateEdit_end->date();
    }
    this->refreshData();
}

void JurnalVzaimoposesheniya::on_dateEdit_editingFinished()
{
    this->isDate = true;
}

void JurnalVzaimoposesheniya::on_dateEdit_end_editingFinished()
{
    this->isDate = true;
}

void JurnalVzaimoposesheniya::on_pushButton_clear_clicked()
{

    ui->pushButtonPrint->setEnabled(false);
    this->isDate = false;
    this->fioProver="";
    this->dateBeginning = QDate();
    this->dateEnd = ui->dateEdit_end->date();

    ui->lineEdit_prepod->clear();
    ui->label_naideno->setVisible(false);
    ui->label_result->setVisible(false);
    this->vidim=false;
    this->refreshData();
}

void JurnalVzaimoposesheniya::on_lineEdit_prepod_editingFinished()
{
    if(ui->lineEdit_prepod->text().isEmpty())
        ui->pushButtonPrint->setEnabled(false);
}

void JurnalVzaimoposesheniya::on_tableView_jurnalVzaimopos_clicked(const QModelIndex &index)
{
    if(ui->tableView_jurnalVzaimopos->model()->index(ui->tableView_jurnalVzaimopos->currentIndex().row(),1).data().toInt()==dal_main->getCurrentEmployee())
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
