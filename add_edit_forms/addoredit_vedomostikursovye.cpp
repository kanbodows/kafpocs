#include "addoredit_vedomostikursovye.h"
#include "ui_addoredit_vedomostikursovye.h"
#include "dals/dal_studentscontrol.h"
#include "delegates/checkboxdelegate.h"
#include <QMessageBox>
#include "delegates/readonlydelegate.h"

AddOrEdit_VedomostiKursovye::AddOrEdit_VedomostiKursovye(QWidget *parent, bool isEdit, int id_vedomostiKurs) :
    QDialog(parent),
    isEdit(isEdit),
    id_vedomostiKurs(id_vedomostiKurs),
    ui(new Ui::AddOrEdit_VedomostiKursovye)
{
    ui->setupUi(this);
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    dal_studentsControl = new Dal_studentsControl(this);
    css = new Styles;
    toPrint = new QTextDocument;
    ui->comboBox_discipline->setModel(dal_studentsControl->getDiscipl());
    ui->comboBox_discipline->setModelColumn(1);
    ui->comboBox_discipline->setCurrentIndex(-1);
    ui->comboBox_spec->setModel(dal_studentsControl->getSpec());
    ui->comboBox_spec->setModelColumn(1);
    ui->comboBox_spec->setCurrentIndex(-1);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->setColumnHidden(0, true);
    ui->tableWidget->setColumnHidden(1, true);
    ui->tableWidget->setItemDelegateForColumn(1, new checkboxdelegate(ui->tableWidget));
    ui->tableWidget->setItemDelegateForColumn(2, new readOnlyDelegate(ui->tableWidget));
    ui->tableWidget->setItemDelegateForColumn(3, new readOnlyDelegate(ui->tableWidget));
    ui->tableWidget->setItemDelegateForColumn(4, new readOnlyDelegate(ui->tableWidget));
    ui->label_error->setVisible(false);
    ui->comboBox_group->setEnabled(false);
    ui->dateEdit->setDate(QDate::currentDate());
    ui->pushButton_print->setEnabled(false);
    ui->label_kurator->setText("");
    if(this->isEdit)
    {
        this->setWindowTitle("Редактирование ведомости на хранение курсовых работ");
        QSqlQuery *query = new QSqlQuery;
        query = dal_studentsControl->getCurrentVedomostiKursovye(this->id_vedomostiKurs);
        ui->comboBox_group->setEnabled(true);
        ui->comboBox_spec->setCurrentText(query->value(4).toString());
        ui->comboBox_group->setCurrentText(query->value(5).toString());
        ui->comboBox_discipline->setCurrentText(query->value(6).toString());
        ui->dateEdit->setDate(query->value(9).toDate());
        ui->label_kolvo_rabot->setText(query->value(7).toString());
        ui->label_kolvo->setText(query->value(8).toString());
        getCourseWorks();
        on_pushButton_print_clicked();
    }
}

AddOrEdit_VedomostiKursovye::~AddOrEdit_VedomostiKursovye()
{
    delete ui;
}

void AddOrEdit_VedomostiKursovye::getCourseWorks()// получение курсовых работ в группе по данной дисциплине
{
    if(ui->comboBox_group->currentIndex() == -1 || ui->comboBox_discipline->currentIndex() == -1)
    {
        return;
    }
    while (ui->tableWidget->rowCount() > 0)
        ui->tableWidget->removeRow(0);
    QSqlQueryModel* model = dal_studentsControl->getCourseWorks(ui->comboBox_group->model()->index(ui->comboBox_group->currentIndex(),0).data().toInt(),
                                                                ui->comboBox_discipline->model()->index(ui->comboBox_discipline->currentIndex(),0).data().toInt(),
                                                                "",
                                                                id_vedomostiKurs);
    int kolvoListov = 0;
    for (int row = 0; row < model->rowCount(); row++)
    {
        ui->tableWidget->insertRow(row);
        ui->tableWidget->setItem(row, 0, new QTableWidgetItem(model->record(row).value(0).toString()));
        ui->tableWidget->setItem(row, 1, new QTableWidgetItem("1"));
        ui->tableWidget->setItem(row, 2, new QTableWidgetItem(model->record(row).value(7).toString()));
        ui->tableWidget->setItem(row, 3, new QTableWidgetItem(model->record(row).value(9).toString()));
        ui->tableWidget->setItem(row, 4, new QTableWidgetItem(model->record(row).value(13).toString()));
        ui->tableWidget->setItem(row, 5, new QTableWidgetItem(model->record(row).value(15).toString()));
        kolvoListov += model->record(row).value(13).toInt();
    }
    ui->label_kolvo_rabot->setText(QString::number(model->rowCount()));
    ui->label_kolvo->setText(QString::number(kolvoListov));
    ui->label_kurator->setText(model->record(0).value(8).toString());
}

void AddOrEdit_VedomostiKursovye::on_pushButton_ok_clicked()
{
    if(ui->tableWidget->rowCount()<=0)
    {
        QMessageBox::warning(this, tr("Ошибка"), tr("Нет курсовых на хранение"));
        return;
    }
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return ;
    }
//    if(!this->isEdit)
    {
        this->id_vedomostiKurs = dal_studentsControl->addVedomostiKursovye(ui->comboBox_group->model()->index(ui->comboBox_group->currentIndex(),0).data().toInt(),
                                                                           ui->comboBox_discipline->model()->index(ui->comboBox_discipline->currentIndex(),0).data().toInt(),
                                                                           ui->dateEdit->date(),
                                                                           ui->label_kolvo_rabot->text().toInt(),
                                                                           ui->label_kolvo->text().toInt());
        for(int i=0; i < ui->tableWidget->rowCount(); i++)
        {
            if(ui->tableWidget->item(i, 1)->text().toInt()==1)
                dal_studentsControl->addKursovyeVedomosti(this->id_vedomostiKurs, ui->tableWidget->item(i, 5)->text(), ui->tableWidget->item(i, 0)->text().toInt());
        }
        ui->pushButton_ok->setEnabled(false);
        QMessageBox::information(this,tr("Информация"),tr("Данные были добавлены"));
    }
    /*else
    {
        for(int i=0; i < ui->tableWidget->rowCount(); i++)
        {
            if (! dal_main->checkConnection())
            {
                QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
                return ;
            }
            // изменение в базу
        }
        QMessageBox::information(this,tr("Информация"),tr("Данные были изменены"));
    }*/
    ui->pushButton_print->setEnabled(true);
}

void AddOrEdit_VedomostiKursovye::on_pushButton_cancel_clicked()
{
    this->close();
}

void AddOrEdit_VedomostiKursovye::on_tableWidget_itemChanged(QTableWidgetItem *item)
{
    if(item->column()==1)
    {
        int kolvoCur  = ui->label_kolvo_rabot->text().toInt();
        if(item->text()=="1")
            kolvoCur++;
        else
            kolvoCur--;
        item->setText(QString::number(kolvoCur));
    }
}

void AddOrEdit_VedomostiKursovye::printDocument(QPrinter *printer)
{
    toPrint->print(printer);
}

void AddOrEdit_VedomostiKursovye::on_pushButton_print_clicked()
{
    MyPrint myPrint;
    RowList list;
    list[7] = tr("ФИО студента");
    list[9] = tr("Тема курсовой");
    list[13] = tr("Кол-во листов");
    list[15] = tr("Примечание");
    QString body;
    body.append("<html><head>"
                "<link rel='stylesheet' type='text/css' href='format.css'>"
                "</head><body>"
                "<center><H3>Ведомость</H3></center><hr><table>"
                "<tr><td>Приема на хранение курсовых работ (проектов), расчётно-графических заданий</p></td></tr>"
                "<tr><td>студентов группы <b>" + ui->comboBox_group->currentText() + "</b>, специальности <b>" + ui->comboBox_spec->currentText() + "</b>, факультета <b>ФИТ</b></td></tr>"
                "<tr><td>дисциплина <b>" + ui->comboBox_discipline->currentText() + "</b>, руководитель <b>" + ui->label_kurator->text() + "</b>, месяц защиты <b>" + ui->dateEdit->date().toString("MMMM-yyyy") + "</b></td></tr></table><hr>");
    body.append(myPrint.printOnlyTable(dal_studentsControl->getCourseWorks(ui->comboBox_group->model()->index(ui->comboBox_group->currentIndex(),0).data().toInt(),
                                                                           ui->comboBox_discipline->model()->index(ui->comboBox_discipline->currentIndex(),0).data().toInt(),
                                                                           "",
                                                                           id_vedomostiKurs),
                                       list, 0));
    body.append("<hr><p>Количество работ: <b>" + ui->label_kolvo_rabot->text() + "</b>, количество страниц: <b>" + ui->label_kolvo->text() + "</b></p><hr>"
                "<p>Роспись лица принявшего на хранение _______________________</p></body></html>");
    toPrint->addResource(QTextDocument::StyleSheetResource, QUrl( "format.css" ), css->reportCss);
    toPrint->setHtml(body);
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("file.pdf");
    QPrintPreviewDialog *preview = new QPrintPreviewDialog (&printer);
    preview->setAttribute(Qt::WA_DeleteOnClose);
    preview->setWindowFlags(Qt::Window);
    QList<QPrintPreviewWidget*> list2 = preview->findChildren<QPrintPreviewWidget*>();
    if(!list2.isEmpty()) // paranoiac safety check
        list2.first()->setZoomMode(QPrintPreviewWidget::FitToWidth);
    connect(preview, SIGNAL(paintRequested(QPrinter *)), SLOT(printDocument(QPrinter *)));
    preview->exec();
}

void AddOrEdit_VedomostiKursovye::on_comboBox_spec_activated(const QString &arg1)
{
    ui->comboBox_group->setEnabled(true);
    ui->comboBox_group->setModel(dal_studentsControl->getComboGroup(ui->comboBox_spec->model()->index(ui->comboBox_spec->currentIndex(),0).data().toInt()));
    ui->comboBox_group->setModelColumn(1);
    ui->comboBox_group->setCurrentIndex(-1);
}

void AddOrEdit_VedomostiKursovye::on_comboBox_group_activated(const QString &arg1)
{
    getCourseWorks();
}

void AddOrEdit_VedomostiKursovye::on_comboBox_discipline_activated(const QString &arg1)
{
    getCourseWorks();
}
