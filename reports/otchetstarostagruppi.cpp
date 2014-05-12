#include "otchetstarostagruppi.h"
#include "ui_otchetstarostagruppi.h"
#include "myprint.h"
#include <QMessageBox>

bool OtchetStarostaGruppi::isOpen = false;
OtchetStarostaGruppi::OtchetStarostaGruppi(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OtchetStarostaGruppi)
{
    ui->setupUi(this);
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
    ReportStarostGroupModel = new QSqlTableModel(this);
    ReportStarostGroupModel->setTable("ReportStarostaView");
    ReportStarostGroupModel->select();
}

OtchetStarostaGruppi::~OtchetStarostaGruppi()
{
    this->isOpen = false;
    delete ui;
}

void OtchetStarostaGruppi::printDocument(QPrinter *printer)
{
    body->print(printer);
}

void OtchetStarostaGruppi::on_pushButton_showReport_clicked()
{
    MyPrint myPrint;
    RowList list;
    list[3] = tr("Группа");
    list[4] = tr("ФИО");
    list[5] = tr("Номер телефона");
    list[6] = tr("Адрес почты");
    list[7] = tr("Адрес проживания");
    ReportStarostGroupModel->select();
    body = myPrint.print((QSqlQueryModel*)ReportStarostGroupModel, list, this->windowTitle(), "", 0);
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
