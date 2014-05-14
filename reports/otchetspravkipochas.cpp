#include "otchetspravkipochas.h"
#include "ui_otchetspravkipochas.h"

bool OtchetSpravkiPochas::isOpen = false;
OtchetSpravkiPochas::OtchetSpravkiPochas(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OtchetSpravkiPochas)
{
    ui->setupUi(this);
    if (! this->isOpen)
    {
        this->isOpen = true;
    }
    else
    {
        QMessageBox::warning(this, tr("Ошибка"), tr("Окно уже открыто"));
        throw std::exception();

    }
    if (!dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        this->isOpen = false;
        throw std::exception();
    }
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    css = new Styles;
    ui->dateEdit_startReport->setDate(QDate::currentDate());
    ui->dateEdit_startReport->setDisplayFormat("MMMM yyyy");
    ui->comboBox_sotr->setModel(dal_prepodcontrol->getcomboPrepod());
    ui->comboBox_sotr->setModelColumn(1);
    ui->comboBox_sotr->setCurrentIndex(-1);
    ui->groupBox_columns->setVisible(false);

}

OtchetSpravkiPochas::~OtchetSpravkiPochas()
{
    this->isOpen = false;
    delete ui;
}

void OtchetSpravkiPochas::printDocument(QPrinter *printer)
{
    toPrint->print(printer);
}

void OtchetSpravkiPochas::on_pushButton_showReport_clicked()
{
    MyPrint myPrint;
    RowList list;
    list[1] = tr("Сотрудник");
    if(ui->checkBox1->isChecked())
        list[2] = tr("Номер протокола");
    if(ui->checkBox2->isChecked())
        list[3] = tr("Лекций");
    if(ui->checkBox3->isChecked())
        list[4] = tr("Лабор.");
    if(ui->checkBox4->isChecked())
        list[5] = tr("Практич.");
    if(ui->checkBox5->isChecked())
        list[6] = tr("On-line");
    if(ui->checkBox6->isChecked())
        list[7] = tr("Off-line");
    if(ui->checkBox7->isChecked())
        list[8] = tr("Контр. раб.");
    if(ui->checkBox8->isChecked())
        list[9] = tr("Экзам.");
    if(ui->checkBox9->isChecked())
        list[10] = tr("Курс. раб.");
    if(ui->checkBox10->isChecked())
        list[11] = tr("Язык");
    if(ui->checkBox11->isChecked())
        list[12] = tr("Номер справки");
    if(ui->checkBox12->isChecked())
        list[13] = tr("Всего");
    QString header = this->windowTitle() + "<br><br>" \
            "<H3>На " + ui->dateEdit_startReport->date().toString("MMMM yyyy") + " г. </H3>";
    if(!ui->checkBox_detalno->isChecked())
    {
        toPrint = myPrint.print(dal_prepodcontrol->getSpravkiPochasReport(ui->comboBox_sotr->model()->index(ui->comboBox_sotr->currentIndex(),0).data().toInt(),
                                                                          ui->dateEdit_startReport->date()),
                                list, header, "", 0);
    }
    else
    {
        QSqlQueryModel* modelMain = dal_prepodcontrol->getSpravkiPochasReport(ui->comboBox_sotr->model()->index(ui->comboBox_sotr->currentIndex(),0).data().toInt(),
                                                                              ui->dateEdit_startReport->date());
        QString body;
        body.append("<html><head>"
                    "<link rel='stylesheet' type='text/css' href='format.css'>"
                    "</head><body>");
        body.append("<center><H2>" + header + "</H2></center><br>");
        RowList list2;
        list2[0] = tr("Дата");
        list2[1] = tr("Группа");
        list2[2] = tr("Дисциплина");
        if(ui->checkBox2->isChecked())
            list2[3] = tr("Лекций");
        if(ui->checkBox3->isChecked())
            list2[4] = tr("Лабор.");
        if(ui->checkBox4->isChecked())
            list2[5] = tr("Практич.");
        if(ui->checkBox5->isChecked())
            list2[6] = tr("On-line");
        if(ui->checkBox6->isChecked())
            list2[7] = tr("Off-line");
        if(ui->checkBox7->isChecked())
            list2[8] = tr("Контр. раб.");
        if(ui->checkBox8->isChecked())
            list2[9] = tr("Экзам.");
        if(ui->checkBox9->isChecked())
            list2[10] = tr("Курс. раб.");

        for (int i = 0; i < modelMain->rowCount(); ++i)
        {
            body.append("<p><b>" + modelMain->record(i).value(1).toString() + "</b></p><hr>");
            body.append(myPrint.printOnlyTable(dal_prepodcontrol->getSpravkaPochasDetails(modelMain->record(i).value(14).toInt()), list2, 0));
            body.append("<p><H3>Итого: всего: " +  modelMain->record(i).value(13).toString() + "</H3>");
            if(ui->checkBox2->isChecked())
                body.append(" лекций: " + modelMain->record(i).value(3).toString());
            if(ui->checkBox3->isChecked())
                body.append(" лабор.: " + modelMain->record(i).value(4).toString());
            if(ui->checkBox4->isChecked())
                body.append(" прак.: " + modelMain->record(i).value(5).toString());
            if(ui->checkBox5->isChecked())
                body.append(" on-line: " + modelMain->record(i).value(6).toString());
            if(ui->checkBox6->isChecked())
                body.append(" off-line: " + modelMain->record(i).value(7).toString());
            if(ui->checkBox7->isChecked())
                body.append(" контр. раб.: " + modelMain->record(i).value(8).toString());
            if(ui->checkBox8->isChecked())
                body.append(" экзамен.: " + modelMain->record(i).value(9).toString());
            if(ui->checkBox9->isChecked())
                body.append(" курс. раб.: " + modelMain->record(i).value(10).toString());
            body.append("</p>");
            body.append("<br>");
        }
        body.append("</body></html>");
        QTextDocument *toPrint2 = new QTextDocument;
        toPrint2->addResource(QTextDocument::StyleSheetResource, QUrl( "format.css" ), css->reportCss);
        toPrint2->setHtml(body);
        toPrint = toPrint2;
    }
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
    while ((item = ui->verticalLayout->takeAt(0) ) != NULL )
    {
        delete item->widget();
        delete item;
    }
    ui->verticalLayout->addWidget(preview);
    preview->exec();
}

void OtchetSpravkiPochas::on_pushButton_set_clicked()
{
    ui->groupBox_columns->setVisible(!ui->groupBox_columns->isVisible());
}
