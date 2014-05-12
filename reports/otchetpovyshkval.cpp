#include "otchetpovyshkval.h"
#include "ui_otchetpovyshkval.h"

bool OtchetPovyshKval::isOpen = false;
OtchetPovyshKval::OtchetPovyshKval(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OtchetPovyshKval)
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
    ui->dateEdit_endReport->setDate(QDate::currentDate());
    ui->dateEdit_startReport->setDisplayFormat("MMMM yyyy");
    ui->dateEdit_endReport->setDisplayFormat("MMMM yyyy");
    ui->comboBox_doljnost->setModel(dal_prepodcontrol->getDoljnost());
    ui->comboBox_doljnost->setModelColumn(1);
    ui->comboBox_doljnost->setCurrentIndex(-1);
    ui->groupBox_columns->setVisible(false);

}

OtchetPovyshKval::~OtchetPovyshKval()
{
    this->isOpen = false;
    delete ui;
}

void OtchetPovyshKval::printDocument(QPrinter *printer)
{
    toPrint->print(printer);
}

void OtchetPovyshKval::on_pushButton_showReport_clicked()
{
    MyPrint myPrint;
    RowList list;
    list[3] = tr("Должность");
    list[4] = tr("Сотрудник");
    if(ui->checkBox1->isChecked())
        list[5] = tr("Дата начала");
    if(ui->checkBox2->isChecked())
        list[6] = tr("Тема");
    if(ui->checkBox3->isChecked())
        list[7] = tr("Курс");
    if(ui->checkBox4->isChecked())
        list[8] = tr("Место");
    if(ui->checkBox5->isChecked())
        list[9] = tr("Оценка");
    QString header = this->windowTitle() + "<br><br>" \
            "<H3>На период с " + ui->dateEdit_startReport->date().toString("dd-MM-yyyy") + " по " + ui->dateEdit_endReport->date().toString("dd-MM-yyyy") + "</H3>";
    toPrint = myPrint.print(dal_prepodcontrol->getPovyshKvalReport(ui->comboBox_doljnost->model()->index(ui->comboBox_doljnost->currentIndex(),0).data().toInt(),
                                                                   ui->dateEdit_startReport->date(),
                                                                   ui->dateEdit_endReport->date()),
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

void OtchetPovyshKval::on_pushButton_set_clicked()
{
    ui->groupBox_columns->setVisible(!ui->groupBox_columns->isVisible());
}
