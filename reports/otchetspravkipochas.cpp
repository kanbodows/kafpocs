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
    toPrint = myPrint.print(dal_prepodcontrol->getSpravkiPochasReport(ui->comboBox_sotr->model()->index(ui->comboBox_sotr->currentIndex(),0).data().toInt(),
                                                                      ui->dateEdit_startReport->date()),
                            list, header, "", 0);
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
