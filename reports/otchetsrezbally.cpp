#include "otchetsrezbally.h"
#include "ui_otchetsrezbally.h"


bool Otchetsrezbally::isOpen = false;
Otchetsrezbally::Otchetsrezbally(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Otchetsrezbally)
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
    plots = new Plots(this);
    css = new Styles;
    ui->comboBox_gruppa->setModel(dal_studentsControl->getComboGroup(0));
    ui->comboBox_gruppa->setModelColumn(1);
    ui->comboBox_gruppa->setCurrentIndex(-1);
    ui->tabWidget->setCurrentIndex(0);
}

Otchetsrezbally::~Otchetsrezbally()
{
    this->isOpen = false;
    delete ui;
}

void Otchetsrezbally::printDocument(QPrinter *printer)
{
    bodyPrint->print(printer);
}

void Otchetsrezbally::on_pushButton_showReport_clicked()
{
    QString body = "";

    int count; // кол-во дисциплин по которым сдавали срез
    body.append("<html><head>"
                "<link rel='stylesheet' type='text/css' href='format.css'>"
                "</head><body>");
    body.append("<br><center><H3>Срез знаний</H3></center>");
    querySrez = dal_studentsControl->getSrezyQuery(ui->comboBox_gruppa->model()->index(ui->comboBox_gruppa->currentIndex(),0).data().toInt(),  ui->dateEdit_startReport->date(), ui->dateEdit_endReport->date());
    for (int r = 0; r < querySrez->size(); r++)
    {
        QSqlQuery *queryStud = new QSqlQuery(); // баллы по текущему срезу
        queryStud->prepare("select * from srez_bally_view where srez_id = " + querySrez->value(0).toString());
        queryStud->exec();
        queryStud->first();
        if (queryStud->size()<=0)
        {
            querySrez->next();
            continue;
        }
        body.append("<p><b>" + querySrez->value(2).toString() + "</b></p>"); // название группы
        body.append("<table border=\"1\">"
                    "<tr><th>№</th><th>ФИО</th>"
                    "<th>" + querySrez->value(3).toString() + "</th>");
        count = 5;
        if(!querySrez->value(4).isNull())
            body.append("<th> " + querySrez->value(4).toString() + " </th>"); // дисциплины по которым сдавали
        else
            count--;
        if(!querySrez->value(5).isNull())
            body.append("<th> " + querySrez->value(5).toString() + " </th>");
        else
            count--;
        if(!querySrez->value(6).isNull())
            body.append("<th> " + querySrez->value(6).toString() + " </th>");
        else
            count--;
        if(!querySrez->value(7).isNull())
            body.append("<th> " + querySrez->value(7).toString() + " </th>");
        else
            count--;
        body.append("</tr>");

        for (int i = 0; i < queryStud->size(); i++)
        {
            body.append("<tr><td>" + QString::number(i+1) + "</td>" // номер студента
                        "<td>" + queryStud->value(2).toString() + "</td>"); // фио студента
            body.append("<td>" + queryStud->value(3).toString() + "</td>"); // баллы по перой дисциплине
            if(count>=2)
                body.append("<td>" + queryStud->value(4).toString() + "</td>"); // баллы по второй дисциплине если не пусто и т.д.
            if(count>=3)
                body.append("<td>" + queryStud->value(5).toString() + "</td>");
            if(count>=4)
                body.append("<td>" + queryStud->value(6).toString() + "</td>");
            if(count>=5)
                body.append("<td>" + queryStud->value(7).toString() + "</td>");
            body.append("</tr>");
            queryStud->next();
        }
        QSqlQuery *querySred = dal_studentsControl->getSrezySrednie(querySrez->value(0).toInt()); // получение средних баллов
        body.append("<tr><td class = 'sum' colspan=\"2\">Средние баллы:</td>"
                    "<td class = 'sum'>" + querySred->value(1).toString() + "</td>"); // средние первой дисциплины
        if(count>=2)
            body.append("<td class = 'sum'>" + querySred->value(2).toString() + "</td>"); // средние второй дисциплины и т.д.
        if(count>=3)
            body.append("<td class = 'sum'>" + querySred->value(3).toString() + "</td>");
        if(count>=4)
            body.append("<td class = 'sum'>" + querySred->value(4).toString() + "</td>");
        if(count>=5)
            body.append("<td class = 'sum'>" + querySred->value(5).toString() + "</td>");
        body.append("</tr>");
        body.append("</table>");
        querySrez->next();
    }
    body.append("<br><p align=\"right\">"+QDate::currentDate().toString("dd-MM-yyyy") +"</p>");
    body.append("</body></html>");
    bodyPrint = new QTextDocument;
    bodyPrint->addResource(QTextDocument::StyleSheetResource, QUrl( "format.css" ), css->reportCss);
    bodyPrint->setHtml(body);
    //    QPrinter printer(QPrinter::HighResolution);
    //    printer.setOrientation(QPrinter::Portrait);
    //    printer.setPaperSize(QPrinter::A4);
    //    printer.setFullPage(true);
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("file.pdf");
    QPrintPreviewDialog *preview = new QPrintPreviewDialog (&printer);
    preview->setAttribute(Qt::WA_DeleteOnClose);
    preview->setWindowTitle("Печать");
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
    on_pushButton_graph_clicked();
    preview->exec();
}

void Otchetsrezbally:: on_pushButton_graph_clicked()
{
    /* QLayoutItem* item;
    while ((item = ui->gridLayout_4->takeAt(0) ) != NULL )
    {
        delete item->widget();
        delete item;
    }
    QSqlQuery *query = new QSqlQuery();
    query->prepare("select * from ReportUstrVipusk");
    query->exec();
    query->first();

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
    ui->gridLayout_4->addWidget(plots->createChart(year, y3, "Трудоустроено в % чел."), 0, 1);*/
}
