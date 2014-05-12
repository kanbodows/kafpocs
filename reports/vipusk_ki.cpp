#include "vipusk_ki.h"
#include "ui_vipusk_ki.h"
//#include "delegates/datedelegate.h"
#include <QMessageBox>
bool Vipusk_ki::isOpen = false;

Vipusk_ki::Vipusk_ki(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Vipusk_ki)
{
    ui->setupUi(this);
    if (!this->isOpen)
    {
        this->isOpen = true;
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
    dal_studentsControl = new Dal_studentsControl(this);
}

Vipusk_ki::~Vipusk_ki()
{
    this->isOpen = false;
    delete ui;
}

void Vipusk_ki::printDocument(QPrinter *printer)
{
    body->print(printer);
}

void Vipusk_ki::on_pushButton_showReport_clicked()
{
    MyPrint myPrint;
    RowList list;
    list[4] = tr("Группа");
    list[5] = tr("ФИО");
    list[6] = tr("Номер телефона");
    list[7] = tr("Адрес почты");
    //body = myPrint.printBody(dal_studentsControl->getVipusk_ki(), list, this->windowTitle(),"");
    body = myPrint.print(dal_studentsControl->getVipusk_ki(), list, this->windowTitle(),"", 1);
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
