#include "addoredit_spravkapochas.h"
#include "ui_addoredit_spravkapochas.h"
#include "dals/dal_studentscontrol.h"
#include "delegates/comboboxitemdelegate.h"
#include <QMessageBox>
#include "delegates/readonlydelegate.h"
#include "delegates/doublespinboxdelegate.h"
#include "delegates/textnndelegate.h"
AddOrEdit_SpravkaPochas::AddOrEdit_SpravkaPochas(QWidget *parent, bool isEdit, int id_spravkaPochas) :
    QDialog(parent),
    isEdit(isEdit),
    id_spravkaPochas(id_spravkaPochas),
    ui(new Ui::AddOrEdit_SpravkaPochas)
{
    ui->setupUi(this);
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    dal_studentsControl = new Dal_studentsControl(this);
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    css = new Styles;
    toPrint = new QTextDocument;
    ui->comboBox_sotr->setModel(dal_prepodcontrol->getcomboPrepod());
    ui->comboBox_sotr->setModelColumn(1);
    ui->comboBox_sotr->setCurrentIndex(-1);
    ui->comboBox_kafedra->setModel(dal_studentsControl->getKafedry());
    ui->comboBox_kafedra->setModelColumn(1);
    ui->comboBox_kafedra->setCurrentIndex(0);
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget->verticalHeader()->hide();
    ui->tableWidget->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->tableWidget->setItemDelegateForColumn(0, new textNnDelegate(ui->tableWidget));
    ui->tableWidget->setItemDelegateForColumn(3, new DoubleSpinBoxDelegate(ui->tableWidget));
    ui->tableWidget->setItemDelegateForColumn(4, new DoubleSpinBoxDelegate(ui->tableWidget));
    ui->tableWidget->setItemDelegateForColumn(5, new DoubleSpinBoxDelegate(ui->tableWidget));
    ui->tableWidget->setItemDelegateForColumn(6, new DoubleSpinBoxDelegate(ui->tableWidget));
    ui->tableWidget->setItemDelegateForColumn(7, new DoubleSpinBoxDelegate(ui->tableWidget));
    ui->tableWidget->setItemDelegateForColumn(8, new DoubleSpinBoxDelegate(ui->tableWidget));
    ui->tableWidget->setItemDelegateForColumn(9, new DoubleSpinBoxDelegate(ui->tableWidget));
    ui->tableWidget->setItemDelegateForColumn(10, new DoubleSpinBoxDelegate(ui->tableWidget));
    ui->dateEdit->setDate(QDate::currentDate());
    ui->pushButton_print->setEnabled(false);
    if(this->isEdit)
    {
        this->setWindowTitle("Редактирование записи");
        QSqlQuery *query = new QSqlQuery();
        query = dal_prepodcontrol->getCurrentSpravkaPochas(this->id_spravkaPochas);
        ui->lineEdit_nomer->setText(query->value(17).toString());
        ui->lineEdit_protocol->setText(query->value(6).toString());
        ui->comboBox_facultet->setCurrentText(query->value(3).toString());
        ui->comboBox_kafedra->setCurrentText(query->value(4).toString());
        ui->comboBox_sotr->setCurrentText(query->value(5).toString());
        ui->dateEdit->setDate(query->value(7).toDate());
        ui->tableWidget->item(0, 3)->setText(query->value(8).toString());
        ui->tableWidget->item(0, 4)->setText(query->value(9).toString());
        ui->tableWidget->item(0, 5)->setText(query->value(10).toString());
        ui->tableWidget->item(0, 6)->setText(query->value(11).toString());
        ui->tableWidget->item(0, 7)->setText(query->value(12).toString());
        ui->tableWidget->item(0, 8)->setText(query->value(13).toString());
        ui->tableWidget->item(0, 9)->setText(query->value(14).toString());
        ui->tableWidget->item(0, 10)->setText(query->value(15).toString());
        ui->comboBox_yazyk->setCurrentText(query->value(16).toString());
        ui->label_vsego->setText(query->value(18).toString());
        QSqlQuery* details = new QSqlQuery();
        details->prepare("SELECT * FROM spravki_details_view WHERE sparvki_pochas_id = " + QString::number(id_spravkaPochas));
        details->exec();
        details->first();
        for (int i = 0; i < details->size(); ++i)
        {
            ui->tableWidget->insertRow(ui->tableWidget->rowCount()-1);
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-2, 0, new QTableWidgetItem(details->value(3).toString()));
            QComboBox* com = new QComboBox();
            com->setModel(dal_studentsControl->getComboGroup(0));
            com->setModelColumn(1);
            com->setCurrentText(details->value(4).toString());
            ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-2, 1, com);
            QComboBox* com2 = new QComboBox();
            com2->setModel(dal_studentsControl->getDiscipl());
            com2->setModelColumn(1);
            com2->setCurrentText(details->value(5).toString());
            ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-2, 2, com2);
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-2, 3, new QTableWidgetItem(details->value(6).toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-2, 4, new QTableWidgetItem(details->value(7).toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-2, 5, new QTableWidgetItem(details->value(8).toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-2, 6, new QTableWidgetItem(details->value(9).toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-2, 7, new QTableWidgetItem(details->value(10).toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-2, 8, new QTableWidgetItem(details->value(11).toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-2, 9, new QTableWidgetItem(details->value(12).toString()));
            ui->tableWidget->setItem(ui->tableWidget->rowCount()-2, 10,new QTableWidgetItem(details->value(13).toString()));
            details->next();
        }
        ui->pushButton_print->setEnabled(true);

    }
    this->showMaximized();
}

AddOrEdit_SpravkaPochas::~AddOrEdit_SpravkaPochas()
{
    delete ui;
}

void AddOrEdit_SpravkaPochas::on_pushButton_ok_clicked()
{
    if(ui->tableWidget->rowCount()<=1)
    {
        QMessageBox::warning(this, tr("Ошибка"), tr("Должна быть хотя бы одна запись в таблице часов"));
        return;
    }
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return ;
    }
    if(!this->isEdit)
    {
        this->id_spravkaPochas = dal_prepodcontrol->addSpravkaPochas(ui->lineEdit_nomer->text().toInt(),
                                                                     ui->comboBox_facultet->currentText(),
                                                                     ui->comboBox_kafedra->model()->index(ui->comboBox_kafedra->currentIndex(),0).data().toInt(),
                                                                     ui->comboBox_sotr->model()->index(ui->comboBox_sotr->currentIndex(),0).data().toInt(),
                                                                     ui->lineEdit_protocol->text().toInt(),
                                                                     ui->dateEdit->date(),
                                                                     ui->tableWidget->item(ui->tableWidget->rowCount()-1, 3)->text().toDouble(),
                                                                     ui->tableWidget->item(ui->tableWidget->rowCount()-1, 4)->text().toDouble(),
                                                                     ui->tableWidget->item(ui->tableWidget->rowCount()-1, 5)->text().toDouble(),
                                                                     ui->tableWidget->item(ui->tableWidget->rowCount()-1, 6)->text().toDouble(),
                                                                     ui->tableWidget->item(ui->tableWidget->rowCount()-1, 7)->text().toDouble(),
                                                                     ui->tableWidget->item(ui->tableWidget->rowCount()-1, 8)->text().toDouble(),
                                                                     ui->tableWidget->item(ui->tableWidget->rowCount()-1, 9)->text().toDouble(),
                                                                     ui->tableWidget->item(ui->tableWidget->rowCount()-1, 10)->text().toDouble(),
                                                                     ui->comboBox_yazyk->currentText(),
                                                                     ui->label_vsego->text().toDouble());
        for(int i=0; i < ui->tableWidget->rowCount()-1; i++)
        {
            dal_prepodcontrol->addSpravkiDetails(ui->tableWidget->item(i, 0)->text(),
                                                ((QComboBox*)ui->tableWidget->cellWidget(i, 1))->model()->index(((QComboBox*)ui->tableWidget->cellWidget(i, 1))->currentIndex(),0).data().toInt(),
                                                ((QComboBox*)ui->tableWidget->cellWidget(i, 2))->model()->index(((QComboBox*)ui->tableWidget->cellWidget(i, 2))->currentIndex(),0).data().toInt(),
                                                 ui->tableWidget->item(i, 3)->text().toDouble(),
                                                 ui->tableWidget->item(i, 4)->text().toDouble(),
                                                 ui->tableWidget->item(i, 5)->text().toDouble(),
                                                 ui->tableWidget->item(i, 6)->text().toDouble(),
                                                 ui->tableWidget->item(i, 7)->text().toDouble(),
                                                 ui->tableWidget->item(i, 8)->text().toDouble(),
                                                 ui->tableWidget->item(i, 9)->text().toDouble(),
                                                 ui->tableWidget->item(i, 10)->text().toDouble(),
                                                 this->id_spravkaPochas);
        }
        ui->pushButton_print->setEnabled(true);
        QMessageBox::information(this,tr("Информация"),tr("Данные были добавлены"));
    }
    else
    {
        if (dal_prepodcontrol->editSpravkaPochas(this->id_spravkaPochas,
                                                 ui->lineEdit_nomer->text().toInt(),
                                                 ui->comboBox_facultet->currentText(),
                                                 ui->comboBox_kafedra->model()->index(ui->comboBox_kafedra->currentIndex(),0).data().toInt(),
                                                 ui->comboBox_sotr->model()->index(ui->comboBox_sotr->currentIndex(),0).data().toInt(),
                                                 ui->lineEdit_protocol->text().toInt(),
                                                 ui->dateEdit->date(),
                                                 ui->tableWidget->item(ui->tableWidget->rowCount()-1, 3)->text().toDouble(),
                                                 ui->tableWidget->item(ui->tableWidget->rowCount()-1, 4)->text().toDouble(),
                                                 ui->tableWidget->item(ui->tableWidget->rowCount()-1, 5)->text().toDouble(),
                                                 ui->tableWidget->item(ui->tableWidget->rowCount()-1, 6)->text().toDouble(),
                                                 ui->tableWidget->item(ui->tableWidget->rowCount()-1, 7)->text().toDouble(),
                                                 ui->tableWidget->item(ui->tableWidget->rowCount()-1, 8)->text().toDouble(),
                                                 ui->tableWidget->item(ui->tableWidget->rowCount()-1, 9)->text().toDouble(),
                                                 ui->tableWidget->item(ui->tableWidget->rowCount()-1, 10)->text().toDouble(),
                                                 ui->comboBox_yazyk->currentText(),
                                                 ui->label_vsego->text().toDouble()))
        {
            dal_prepodcontrol->deleteSpravkiDetails(this->id_spravkaPochas);
            for(int i=0; i < ui->tableWidget->rowCount()-1; i++)
            {
                dal_prepodcontrol->addSpravkiDetails(ui->tableWidget->item(i, 0)->text(),
                                                     ((QComboBox*)ui->tableWidget->cellWidget(i, 1))->model()->index(((QComboBox*)ui->tableWidget->cellWidget(i, 1))->currentIndex(),0).data().toInt(),
                                                     ((QComboBox*)ui->tableWidget->cellWidget(i, 2))->model()->index(((QComboBox*)ui->tableWidget->cellWidget(i, 2))->currentIndex(),0).data().toInt(),
                                                     ui->tableWidget->item(i, 3)->text().toDouble(),
                                                     ui->tableWidget->item(i, 4)->text().toDouble(),
                                                     ui->tableWidget->item(i, 5)->text().toDouble(),
                                                     ui->tableWidget->item(i, 6)->text().toDouble(),
                                                     ui->tableWidget->item(i, 7)->text().toDouble(),
                                                     ui->tableWidget->item(i, 8)->text().toDouble(),
                                                     ui->tableWidget->item(i, 9)->text().toDouble(),
                                                     ui->tableWidget->item(i, 10)->text().toDouble(),
                                                     this->id_spravkaPochas);
            }
            QMessageBox::information(this,tr("Информация"),tr("Данные были изменены"));
        }
    }
}

void AddOrEdit_SpravkaPochas::on_pushButton_cancel_clicked()
{
    this->close();
}


void AddOrEdit_SpravkaPochas::printDocument(QPrinter *printer)
{
    toPrint->print(printer);
}

void AddOrEdit_SpravkaPochas::on_pushButton_print_clicked()
{
    QString body;
    body.append("<html><head>"
                "<link rel='stylesheet' type='text/css' href='format.css'>"
                "</head><body>"
                "<center><H3>Справка № " + ui->lineEdit_nomer->text() + "</H3></center><hr>"
                "<table width='100%'>"
                "<tr><td><center>о выполнении учебной нагрузки  на условиях почасовой оплаты штатным преподавателям кафедры</p></center></td></tr>"
                "<tr><td><center><b>" + ui->comboBox_kafedra->currentText() + "</b></center></td></tr>"
                "<tr><td><center><b>" + ui->comboBox_sotr->currentText() + "</b></center></td></tr>"
                "<tr><td><center>на <b>" + ui->comboBox_facultet->currentText() + "</b> факультете по решению кафедры, протокол № <b>" + ui->lineEdit_protocol->text() + "</b></center></td></tr>"
                "<tr><td><center>за <b>" + ui->dateEdit->date().toString("MMMM yyyy") + "</b></center></td></tr>"
                "</table><hr>"
                "<table border='1' class = 'mine'><tr>"
                "<th rowspan='2'>Дата выполнения нагрузки</th>"
                "<th rowspan='2'>В группе </th>"
                "<th rowspan='2'>По дисциплине</th>"
                "<th colspan='8'>Кол-во часов выполн. нагрузки</th></tr><tr>"
                "<th>Лекции</th>"
                "<th>Лаборат. з.</th>"
                "<th>Практич. з.</th>"
                "<th>On-line</th>"
                "<th>Off-line</th>"
                "<th>Кон. раб.</th>"
                "<th>Экзамен</th>"
                "<th>Курс. раб.</th></tr>");
    for (int i = 0; i < ui->tableWidget->rowCount(); ++i) {
        body.append("<tr>");
        body.append("<td>" + ui->tableWidget->item(i, 0)->text() + "</td>");
        if(i != ui->tableWidget->rowCount()-1)
            body.append("<td>" + ((QComboBox*)ui->tableWidget->cellWidget(i, 1))->currentText() + "</td>"
                        "<td>" + ((QComboBox*)ui->tableWidget->cellWidget(i, 2))->currentText() + "</td>");
        else
            body.append("<td colspan='2'><b>Всего: " + ui->label_vsego->text() + "</b></td>");
        for (int j = 3; j < ui->tableWidget->columnCount(); ++j)
            body.append("<td>" + ui->tableWidget->item(i, j)->text() + "</td>");

        body.append("</tr>");
    }
    body.append("<tr><td colspan = '10'>Выполнения подтверждается записями в учебном журнале, протоколе ГАК, актом, табелем"
                "<br><b>Декан факультета _____________________________</b>"
                "<br><b>Зав. кафедрой ________________________________</b>"
                "<br>\"___\"__________ 20__г. <b>Сек. факультета ________________________________</b></td></tr>"
                "<tr><td colspan = '3'><b>В бухгалтерию:</b></td></tr>"
                "<tr><td colspan = '3'>Выполнен. нагрузку подтверждаю в кол-ве часов:</td></tr>"
                "<tr><td colspan = '10'>Нач. уч. части ________________________________   Нагрузка учтена в журнале учета педагог. работ</td></tr>"
                "<tr><td colspan = '10'>\"___\"__________ 20__г. <b>Инспетор уч. части ________________________________</b></td></tr>"
                "<tr><td colspan = '3'><b>Расчет для оплаты:</b></td></tr>"
                "<tr><td colspan = '3'>Подлежит оплате по ставкам сом. т. .........</td></tr>"
                "<tr><td colspan = '3'>В сумме сом. т. .........</td></tr>"
                "<tr><td colspan = '10'><b>Всего сом. ________________________________ Бухгалтер</b></td></tr>"
                "<tr><td colspan = '10'>\"___\"__________ 20__г. </td></tr>"
                "</table></body></html>");
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

void AddOrEdit_SpravkaPochas::on_toolButton_add_clicked()
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount()-1);
    ui->tableWidget->setItem(ui->tableWidget->rowCount()-2, 0, new QTableWidgetItem(QDate::currentDate().toString("dd.MM.yyyy")));
    QComboBox* com = new QComboBox();
    com->setModel(dal_studentsControl->getComboGroup(0));
    com->setModelColumn(1);
    com->setCurrentIndex(-1);
    ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-2, 1, com);
    QComboBox* com2 = new QComboBox();
    com2->setModel(dal_studentsControl->getDiscipl());
    com2->setModelColumn(1);
    com2->setCurrentIndex(-1);
    ui->tableWidget->setCellWidget(ui->tableWidget->rowCount()-2, 2, com2);
    for (int i = 3; i < ui->tableWidget->columnCount(); ++i)
        ui->tableWidget->setItem(ui->tableWidget->rowCount()-2, i, new QTableWidgetItem(""));
}

void AddOrEdit_SpravkaPochas::on_toolButton_delete_clicked()
{
    if(ui->tableWidget->currentRow() != ui->tableWidget->rowCount())
        ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}

void AddOrEdit_SpravkaPochas::on_toolButton_edit_clicked()
{
    //    ui->tableWidget->edit();
}

void AddOrEdit_SpravkaPochas::on_tableWidget_cellChanged(int row, int column)
{
    if(column < 3)
        return;
    double sum = 0;
    for (int i = 0; i < ui->tableWidget->rowCount()-1; ++i)
        sum += ui->tableWidget->item(i, column)->text().toDouble();
    ui->tableWidget->item(ui->tableWidget->rowCount()-1, column)->setText(QString::number(sum));
    sum = 0;
    for (int i = 3; i < ui->tableWidget->columnCount(); ++i)
        sum += ui->tableWidget->item(ui->tableWidget->rowCount()-1, i)->text().toDouble();
    ui->label_vsego->setText(QString::number(sum));
}
