#include "prosmotrgrafvzaimopos.h"
#include "ui_prosmotrgrafvzaimopos.h"
#include "delegates/transactiondatedelegatewithousec.h"
#include <QMessageBox>

ProsmotrGrafVzaimopos::ProsmotrGrafVzaimopos(QWidget *parent, QString GodSS, QString GodPo) :
    QDialog(parent),
    ui(new Ui::ProsmotrGrafVzaimopos)
{
    ui->setupUi(this);
    this->GodStart = GodSS;
    this->GodEnd = GodPo;
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    this->setWindowTitle("Просмотр графика взаимопосещения на " + GodSS + " - " + GodPo + " учебный год");
    ProsmotrGrafModel = new QSqlTableModel(this);
    ProsmotrGrafModel->setTable("ProsmotrGrafikaView");
    ProsmotrGrafModel->setHeaderData(3,Qt::Horizontal,tr("Преподаватель"));
    ProsmotrGrafModel->setHeaderData(4,Qt::Horizontal,tr("Дата проверки"));
    ProsmotrGrafModel->setHeaderData(5,Qt::Horizontal,tr("Проверяющий"));

    ProsmotrGrafModel->setFilter("NachaloGoda = " + GodSS + " and KoneccGoda = " + GodPo);
    ProsmotrGrafModel->select();

    ui->tableView_ProsmotrGraf->setModel(ProsmotrGrafModel);
    ui->tableView_ProsmotrGraf->setColumnHidden(0,true);
    ui->tableView_ProsmotrGraf->setColumnHidden(1,true);
    ui->tableView_ProsmotrGraf->setColumnHidden(2,true);
    ui->tableView_ProsmotrGraf->setColumnHidden(6,true);
    ui->tableView_ProsmotrGraf->setColumnHidden(7,true);
    ui->tableView_ProsmotrGraf->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_ProsmotrGraf->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_ProsmotrGraf->setItemDelegateForColumn(4, new transactiondatedelegatewithousec(ui->tableView_ProsmotrGraf));
}

ProsmotrGrafVzaimopos::~ProsmotrGrafVzaimopos()
{

    delete ui;
}


void ProsmotrGrafVzaimopos::on_pushButtonClose_clicked()
{
    this->close();
}

void ProsmotrGrafVzaimopos::on_pushButtonAdd_clicked()
{
    try
    {
        AddGrafVzaimoposForm = new AddGrafVzaimopos(this, INSERT, -1, this->GodStart, this->GodEnd);
        AddGrafVzaimoposForm->exec();
    }
    catch (...)
    {
        return;
    }
    ProsmotrGrafModel->setFilter("NachaloGoda = " + this->GodStart + " and KoneccGoda = " + this->GodEnd);
    ProsmotrGrafModel->select();
}

void ProsmotrGrafVzaimopos::on_pushButtonEdit_clicked()
{
    if(ui->tableView_ProsmotrGraf->model()->index(ui->tableView_ProsmotrGraf->currentIndex().row(),1).data().toInt()==dal_main->getCurrentEmployee())
    {
        ui->pushButtonEdit->setEnabled(true);
    if (! ui->tableView_ProsmotrGraf->model()->index(ui->tableView_ProsmotrGraf->currentIndex().row(), 0).data().toInt())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите запись из таблицы"));
        return;
    }
    int id = ui->tableView_ProsmotrGraf->model()->index(ui->tableView_ProsmotrGraf->currentIndex().row(), 0).data().toInt();
    try
    {
        AddGrafVzaimoposForm = new AddGrafVzaimopos(this, EDIT, id);
        AddGrafVzaimoposForm->exec();
    }
    catch (...)
    {
        return;
    }
    ProsmotrGrafModel->setFilter("NachaloGoda = " + this->GodStart + " and KoneccGoda = " + this->GodEnd);
    ProsmotrGrafModel->select();
    }

    else
    {
        ui->pushButtonEdit->setEnabled(false);
        QMessageBox::information(this,tr("Внимание"),tr("Вы не можете редактировать не свою запись"));
    }
}

void ProsmotrGrafVzaimopos::printDocument(QPrinter *printer)
{
   body->print(printer);
}

void ProsmotrGrafVzaimopos::on_pushButtonPrint_clicked()
{
    MyPrint myPrint;
    RowList list;
    QString title = this->windowTitle();

    list[3] = tr("Преподаватель");
    list[4] = tr("Дата проверки");
    list[5] = tr("Проверяющий");
    ProsmotrGrafModel->select();
    body = myPrint.print((QSqlQueryModel*)ProsmotrGrafModel, list, title , "", 0);

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

void ProsmotrGrafVzaimopos::on_tableView_ProsmotrGraf_clicked(const QModelIndex &index)
{
    if(ui->tableView_ProsmotrGraf->model()->index(ui->tableView_ProsmotrGraf->currentIndex().row(),1).data().toInt()==dal_main->getCurrentEmployee())
    {
        ui->pushButtonEdit->setEnabled(true);
    }
    else
    {
       ui->pushButtonEdit->setEnabled(false);
    }
}
