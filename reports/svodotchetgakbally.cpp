#include "svodotchetgakbally.h"
#include "ui_svodotchetgakbally.h"
#include <QMessageBox>
#include <QList>
bool SvodOtchetGakBally::isOpen = false;
SvodOtchetGakBally::SvodOtchetGakBally(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SvodOtchetGakBally)
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
    ui->tabWidget->setCurrentIndex(0);
    ui->comboBox_spec->setModel(dal_studentsControl->getSpec());
    ui->comboBox_spec->setModelColumn(1);
    ui->comboBox_spec->setCurrentIndex(-1);
    ui->comboBox_gak->setModel(dal_studentsControl->getGAK(0, 0));
    ui->comboBox_gak->setModelColumn(13);
    ui->comboBox_gak->setCurrentIndex(-1);
    ui->comboBox_group->setEditable(false);
}

SvodOtchetGakBally::~SvodOtchetGakBally()
{
    this->isOpen = false;
    delete ui;
}

void SvodOtchetGakBally::printDocument(QPrinter *printer)
{
    toPrint->print(printer);
}

void SvodOtchetGakBally::on_pushButton_showReport_clicked()
{
    if(ui->comboBox_gak->currentIndex() ==-1 || ui->comboBox_spec->currentIndex() ==-1)
    {
        QMessageBox::warning(this, tr("Ошибка"), tr("Выберите год проведения ГАК и специальность"));
        return;
    }
    MyPrint myPrint;
    RowList list;
    list[4] = tr("Группа");
    list[5] = tr("Всего студентов");
    list[6] = tr("Допущено к ГЭ");
    list[7] = tr("Сдали ГЭ");
    list[8] = tr("Неуд-но");
    list[9] = tr("Удов-но");
    list[10] = tr("Хорошо");
    list[11] = tr("Отлично");
    QSqlQueryModel* gakmodel = dal_studentsControl->getGAK(0, ui->comboBox_gak->model()->index(ui->comboBox_gak->currentIndex(), 0).data().toInt());
    QSqlQueryModel* SostKomGAKModel = dal_studentsControl->getSostavKom(gakmodel->record(0).value(1).toInt());
    QString header = this->windowTitle() + "<br><br>" \
            "<H3>На период с " + ui->comboBox_gak->model()->index(ui->comboBox_gak->currentIndex(), 13).data().toString() + "</H3>";
    QString body;
    body.append("<html><head>"
                "<link rel='stylesheet' type='text/css' href='format.css'>"
                "</head><body>"
                "<center><H2>" + header + "</H2></center><br>"
                "<p>ГАК по специальности <b>\"" + ui->comboBox_spec->model()->index(ui->comboBox_spec->currentIndex(),1).data().toString() + "\"</b> утвержден приказом по КГТУ № "
                + gakmodel->record(0).value(12).toString() + " в следующем составе:<br>"
                "Председатель ГАК: " + SostKomGAKModel->record(0).value(1).toString() + " - " + SostKomGAKModel->record(0).value(2).toString() + "<br>"
                "Зам председателя: " + SostKomGAKModel->record(0).value(3).toString() + " - " + SostKomGAKModel->record(0).value(4).toString() + "<br>"
                "Члены комиссии:<br>"
                "3. " + SostKomGAKModel->record(0).value(5).toString() + " - " + SostKomGAKModel->record(0).value(6).toString() + "<br>"
                "4. " + SostKomGAKModel->record(0).value(7).toString() + " - " + SostKomGAKModel->record(0).value(8).toString() + "<br>"
                "5. " + SostKomGAKModel->record(0).value(9).toString() + " - " + SostKomGAKModel->record(0).value(10).toString() + "<br>"
                "6. " + SostKomGAKModel->record(0).value(11).toString() + " - " + SostKomGAKModel->record(0).value(12).toString() + "<br>"
                "7. " + SostKomGAKModel->record(0).value(13).toString() + " - " + SostKomGAKModel->record(0).value(14).toString() + "<br>"
                " Результаты сдачи ГЭ проведенного " + gakmodel->record(0).value(10).toDate().toString("dd-MM-yyyy") + " приведены ниже в таблице 1 <br>"
                "</p>");
    body.append(myPrint.printOnlyTable(dal_studentsControl->getGakSvodReport(ui->comboBox_spec->model()->index(ui->comboBox_spec->currentIndex(),0).data().toInt(),
                                                                             ui->comboBox_group->model()->index(ui->comboBox_group->currentIndex(),0).data().toInt(),
                                                                             ui->comboBox_gak->model()->index(ui->comboBox_gak->currentIndex(),0).data().toInt()),
                                       list, 0));

    body.append("<br><br><p><b>Список предметов, вынесенных на Государственный экзамен:</b><br>"
                "1. " + gakmodel->record(0).value(6).toString() + "<br>"
                "2. " + gakmodel->record(0).value(7).toString() + "<br>"
                "3. " + gakmodel->record(0).value(8).toString() + "<br>"
                "4. " + gakmodel->record(0).value(9).toString() + "<br></p>");
    RowList list2;
    list2[5] = tr("Группа");
    list2[6] = tr("Студент");
    list2[7] = gakmodel->record(0).value(6).toString();
    list2[8] = gakmodel->record(0).value(7).toString();
    list2[9] = gakmodel->record(0).value(8).toString();
    list2[10] = gakmodel->record(0).value(9).toString();
    list2[12] = tr("Средняя оценка");

    body.append(myPrint.printOnlyTable(dal_studentsControl->getGakBally(ui->comboBox_spec->model()->index(ui->comboBox_spec->currentIndex(),0).data().toInt(),
                                                                        ui->comboBox_group->model()->index(ui->comboBox_group->currentIndex(),0).data().toInt(),
                                                                        ui->comboBox_gak->model()->index(ui->comboBox_gak->currentIndex(),0).data().toInt()),
                                       list2, 1));
    body.append("</body></html>");
    QTextDocument *toPrint2 = new QTextDocument;
    toPrint2->addResource(QTextDocument::StyleSheetResource, QUrl( "format.css" ), css->reportCss);
    toPrint2->setHtml(body);
    toPrint = toPrint2;

    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("file.pdf");
    QPrintPreviewDialog *preview = new QPrintPreviewDialog (&printer);
    preview->setAttribute(Qt::WA_DeleteOnClose);
    preview->setWindowFlags(Qt::Widget);
    QList<QPrintPreviewWidget*> list3 = preview->findChildren<QPrintPreviewWidget*>();
    if(!list3.isEmpty())
        list3.first()->setZoomMode(QPrintPreviewWidget::FitToWidth);
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

void SvodOtchetGakBally::on_pushButton_graph_clicked()
{
    /*QLayoutItem* item;
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
*/
}


void SvodOtchetGakBally::on_comboBox_spec_activated(const QString &arg1)
{
    ui->comboBox_group->setEnabled(true);
    ui->comboBox_group->setModel(dal_studentsControl->getComboGroup(ui->comboBox_spec->model()->index(ui->comboBox_spec->currentIndex(),0).data().toInt()));
    ui->comboBox_group->setModelColumn(1);
    ui->comboBox_group->setCurrentIndex(-1);
}
