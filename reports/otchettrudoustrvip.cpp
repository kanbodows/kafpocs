#include "otchettrudoustrvip.h"
#include "ui_otchettrudoustrvip.h"
#include <QMessageBox>
#include <QList>
bool OtchetTrudoUstrVip::isOpen = false;
OtchetTrudoUstrVip::OtchetTrudoUstrVip(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OtchetTrudoUstrVip)
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
    plots = new Plots(this);
    ui->tabWidget->setCurrentIndex(0);
}

OtchetTrudoUstrVip::~OtchetTrudoUstrVip()
{
    isOpen = false;
    delete ui;
}

void OtchetTrudoUstrVip::printDocument(QPrinter *printer)
{
    body->print(printer);
}

void OtchetTrudoUstrVip::on_pushButton_showReport_clicked()
{
    MyPrint myPrint;
    RowList list;
    list[0] = tr("Год выпуска");
    list[1] = tr("Всего окончили");
    list[2] = tr("Трудоустроено чел.");
    list[3] = tr("Трудоустроено в % чел.");
    list[4] = tr("Выехали за пределы республики");
    body = myPrint.print(dal_studentsControl->getRepTrudoVipusk(), list, this->windowTitle(), "", 0);
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("file.pdf");
    QPrintPreviewDialog *preview = new QPrintPreviewDialog (&printer);
    preview->setAttribute(Qt::WA_DeleteOnClose);
    preview->setWindowFlags(Qt::Widget);
    QList<QPrintPreviewWidget*> list2 = preview->findChildren<QPrintPreviewWidget*>();
    if(!list2.isEmpty())
      list2.first()->setZoomMode(QPrintPreviewWidget::FitToWidth);
    connect(preview, SIGNAL(paintRequested(QPrinter *)), SLOT(printDocument(QPrinter *)));
    QLayoutItem* item2;
    while ((item2 = ui->verticalLayout->takeAt(0) ) != NULL )
    {
        delete item2->widget();
        delete item2;
    }
    ui->verticalLayout->addWidget(preview);
    on_pushButton_graph_clicked();
    preview->exec();

}

void OtchetTrudoUstrVip::on_pushButton_graph_clicked()
{
    QLayoutItem* item;
    while ((item = ui->gridLayout_4->takeAt(0) ) != NULL )
    {
        delete item->widget();
        delete item;
    }
    QSqlQuery *query = new QSqlQuery();
    query->prepare("select * from ReportUstrVipusk");
    query->exec();
    query->first();
    QVector<double> year(query->size()), y1(query->size()), y2(query->size()), y3(query->size()), y4(query->size());
    for (int i=0; i < query->size(); ++i)
    {
        year[i] = query->value(0).toDouble();
        y1[i] = query->value(1).toDouble();
        y2[i] = query->value(2).toDouble();
        y3[i] = query->value(3).toDouble();
        y4[i] = query->value(4).toDouble();
        query->next();
    }
    QCustomPlot* customPlot = new QCustomPlot();
    // create and configure plottables:
    QCPGraph *graph1 = customPlot->addGraph();
    graph1->setData(year, y1);
    graph1->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 10));
    graph1->setPen(QPen(QColor(120, 120, 120), 2));
    graph1->setName("Всего окончили");
    graph1->setSelectable(true);

    QCPGraph *graph2 = customPlot->addGraph();
    graph2->setData(year, y2);
    graph2->setPen(QPen(QColor(150, 20, 120), 5));
    graph2->setBrush(QColor(200, 200, 200, 20));
    graph2->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 10));
    graph2->setChannelFillGraph(graph1);
    graph2->setName("Трудоустроено чел.");

    QCPGraph *graph3 = customPlot->addGraph();
    graph3->setData(year, y4);
    graph3->setPen(QPen(Qt::red));
    graph3->setBrush(QColor(20, 180, 200, 20));
    graph3->setChannelFillGraph(graph2);
    graph3->setScatterStyle(QCPScatterStyle(QCPScatterStyle::ssCircle, QPen(Qt::black, 1.5), QBrush(Qt::white), 10));
    graph3->setName("Выехали за пределы республики");

    // set title of plot:
    customPlot->plotLayout()->insertRow(0);
    customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(customPlot, "Графики"));
    // set a fixed tick-step to one tick per year value:
    customPlot->xAxis->setAutoTickStep(false);
    customPlot->xAxis->setTickStep(1);
    customPlot->xAxis->setSubTickCount(3);
    // set a fixed tick-step to one tick per колво value:
    customPlot->yAxis->setAutoTickStep(false);
    customPlot->yAxis->setTickStep(1);
    customPlot->yAxis->setSubTickCount(3);
    // labels
    customPlot->xAxis->setLabel("Год");
    customPlot->yAxis->setLabel("Кол-во");
    customPlot->yAxis->setLabelColor(Qt::white);
    customPlot->xAxis->setLabelColor(Qt::white);
    // move bars above graphs and grid below bars:
    customPlot->addLayer("abovemain", customPlot->layer("main"), QCustomPlot::limAbove);
    customPlot->addLayer("belowmain", customPlot->layer("main"), QCustomPlot::limBelow);
    graph1->setLayer("abovemain");
    customPlot->xAxis->grid()->setLayer("belowmain");
    customPlot->yAxis->grid()->setLayer("belowmain");
    customPlot->legend->setVisible(true);
    customPlot->legend->setFont(QFont("Helvetica", 9));
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    // set some pens, brushes and backgrounds:
    customPlot->xAxis->setBasePen(QPen(Qt::white, 1));
    customPlot->yAxis->setBasePen(QPen(Qt::white, 1));
    customPlot->xAxis->setTickPen(QPen(Qt::white, 1));
    customPlot->yAxis->setTickPen(QPen(Qt::white, 1));
    customPlot->xAxis->setSubTickPen(QPen(Qt::white, 1));
    customPlot->yAxis->setSubTickPen(QPen(Qt::white, 1));
    customPlot->xAxis->setTickLabelColor(Qt::white);
    customPlot->yAxis->setTickLabelColor(Qt::white);
    customPlot->xAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    customPlot->yAxis->grid()->setPen(QPen(QColor(140, 140, 140), 1, Qt::DotLine));
    customPlot->xAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    customPlot->yAxis->grid()->setSubGridPen(QPen(QColor(80, 80, 80), 1, Qt::DotLine));
    customPlot->xAxis->grid()->setSubGridVisible(true);
    customPlot->yAxis->grid()->setSubGridVisible(true);
    customPlot->xAxis->grid()->setZeroLinePen(Qt::NoPen);
    customPlot->yAxis->grid()->setZeroLinePen(Qt::NoPen);
    customPlot->xAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    customPlot->yAxis->setUpperEnding(QCPLineEnding::esSpikeArrow);
    QLinearGradient plotGradient;
    plotGradient.setStart(0, 0);
    plotGradient.setFinalStop(0, 350);
    plotGradient.setColorAt(0, QColor(80, 80, 80));
    plotGradient.setColorAt(1, QColor(50, 50, 50));
    customPlot->setBackground(plotGradient);
    QLinearGradient axisRectGradient;
    axisRectGradient.setStart(0, 0);
    axisRectGradient.setFinalStop(0, 350);
    axisRectGradient.setColorAt(0, QColor(80, 80, 80));
    axisRectGradient.setColorAt(1, QColor(30, 30, 30));
    customPlot->axisRect()->setBackground(axisRectGradient);
    customPlot->rescaleAxes();
    ui->gridLayout_4->addWidget(customPlot, 0, 0);
    ui->gridLayout_4->addWidget(plots->createChart(year, y3, "Трудоустроено в % чел."), 0, 1);

}
