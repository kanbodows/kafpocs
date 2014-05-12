#include "otchetsotrudniki.h"
#include "ui_otchetsotrudniki.h"

bool OtchetSotrudniki::isOpen = false;
OtchetSotrudniki::OtchetSotrudniki(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OtchetSotrudniki)
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
    ui->comboBox_doljnost->setModel(dal_prepodcontrol->getDoljnost());
    ui->comboBox_doljnost->setModelColumn(1);
    ui->comboBox_doljnost->setCurrentIndex(-1);
    ui->groupBox_columns->setVisible(false);
    toPrint = new QTextDocument;
}

OtchetSotrudniki::~OtchetSotrudniki()
{
    this->isOpen = false;
    delete ui;
}

void OtchetSotrudniki::printDocument(QPrinter *printer)
{
    toPrint->print(printer);
}

void OtchetSotrudniki::on_pushButton_showReport_clicked()
{
    MyPrint myPrint;
    RowList list;
    list[1] = tr("Состав");
    list[2] = tr("ФИО");
    if(ui->checkBox1->isChecked())
        list[3] = tr("Телефон");
    if(ui->checkBox2->isChecked())
        list[4] = tr("E-mail");
    if(ui->checkBox3->isChecked())
        list[5] = tr("Образование");
    if(ui->checkBox4->isChecked())
        list[6] = tr("Ученое звание");
    if(ui->checkBox5->isChecked())
        list[7] = tr("Должность");
    if(ui->checkBox6->isChecked())
        list[8] = tr("Ставка");
    if(ui->checkBox7->isChecked())
        list[9] = tr("Дата рождения");
    if(ui->checkBox8->isChecked())
        list[10] = tr("Место рождения");
    if(ui->checkBox9->isChecked())
        list[11] = tr("Национальность");
    if(ui->checkBox10->isChecked())
        list[12] = tr("Серия паспорта");
    if(ui->checkBox11->isChecked())
        list[13] = tr("Адрес проживания");
    if(ui->checkBox12->isChecked())
        list[14] = tr("Семейное положение");
    if(ui->checkBox13->isChecked())
        list[15] = tr("№ Соц. защиты");
    QString header = this->windowTitle() + "<br>" \
            "<H3>На " + QDate::currentDate().toString("dd-MM-yyyy") + "</H3>";
    toPrint = myPrint.print(dal_prepodcontrol->getSotrReportModels(ui->comboBox_doljnost->model()->index(ui->comboBox_doljnost->currentIndex(),0).data().toInt(),
                                                                   ui->comboBox_sostav->currentText()),
                            list, header, "", 1);
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



void OtchetSotrudniki::on_pushButton_set_clicked()
{
    ui->groupBox_columns->setVisible(!ui->groupBox_columns->isVisible());
}
