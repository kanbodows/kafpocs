#include "otchetformacontrolya.h"
#include "ui_otchetformacontrolya.h"
#include "myprint.h"
#include <QMessageBox>

bool OtchetFormaControlya::isOpen = false;
OtchetFormaControlya::OtchetFormaControlya(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OtchetFormaControlya)
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
    dal_studentsControl = new Dal_studentsControl(this);
    formaControlyaModel = new QSqlTableModel(this);
    formaControlyaModel->setTable("forma_controlya_view");
    ui->comboBox_spec->setModel(dal_studentsControl->getSpec());
    ui->comboBox_spec->setModelColumn(1);
    ui->comboBox_spec->setCurrentIndex(-1);
}

OtchetFormaControlya::~OtchetFormaControlya()
{
    this->isOpen = false;
    delete ui;
}

void OtchetFormaControlya::printDocument(QPrinter *printer)
{
   body->print(printer);
}

void OtchetFormaControlya::on_pushButton_showReport_clicked()
{
    MyPrint myPrint;
    RowList list;
    QString title;
    int groupColumns = 0;
    if(ui->comboBox_spec->currentIndex()!=-1)
    {
//        formaControlyaModel->setFilter(" WHERE  1 = 1 ");
        title = this->windowTitle() + " по специальности " + ui->comboBox_spec->currentText();
    }
    else
    {
        title = this->windowTitle();
        list[1] = tr("Специальность");
        groupColumns = 1;
    }
    list[2] = tr("Дисциплины");
    list[3] = tr("Кол-во контр. точек");
    list[4] = tr("Кол-во вариантов (картов, бланков и др. форм)");
    list[5] = tr("машинная");
    list[6] = tr("бланочная");
    list[7] = tr("устная");
    list[8] = tr("письменная");
    list[9] = tr("Автор тестов");
    formaControlyaModel->select();
    body = myPrint.print((QSqlQueryModel*)formaControlyaModel, list, title , "", groupColumns);
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
