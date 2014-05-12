
#include "dals/dal_main.h"
#include <QMessageBox>
#include <QDebug>
#include <QDialog>
#include "plots.h"
Plots::Plots(QObject *parent) :
    QObject(parent)
{
    

}
Plots::~Plots()
{
}

QCustomPlot* Plots::createChart(QVector<double> x, QVector<double> y, QString chartName)
{
    QCustomPlot* customPlot = new QCustomPlot();
    // create and configure plottables:

    QCPBars *bars1 = new QCPBars(customPlot->xAxis, customPlot->yAxis);
    customPlot->addPlottable(bars1);
    bars1->setWidth(0.3);
    bars1->setData(x, y);
    bars1->setPen(QPen(Qt::red));
    bars1->setBrush(QColor(10, 140, 70, 160));
    // set title of plot:
    customPlot->plotLayout()->insertRow(0);
    customPlot->plotLayout()->addElement(0, 0, new QCPPlotTitle(customPlot, chartName));
    // set a fixed tick-step to one tick per year value:
    customPlot->xAxis->setAutoTickStep(false);
    customPlot->xAxis->setTickStep(1);
    customPlot->xAxis->setSubTickCount(3);
    // set a fixed tick-step to one tick per колво value:
    customPlot->yAxis->setAutoTickStep(false);
    customPlot->yAxis->setTickStep(10);
    customPlot->yAxis->setSubTickCount(3);
    // labels
    customPlot->xAxis->setLabel("Год");
    customPlot->yAxis->setLabel("% человек");
    customPlot->yAxis->setLabelColor(Qt::white);
    customPlot->xAxis->setLabelColor(Qt::white);
    // move bars above graphs and grid below bars:
    customPlot->addLayer("abovemain", customPlot->layer("main"), QCustomPlot::limAbove);
    customPlot->addLayer("belowmain", customPlot->layer("main"), QCustomPlot::limBelow);
    bars1->setLayer("abovemain");
    customPlot->xAxis->grid()->setLayer("belowmain");
    customPlot->yAxis->grid()->setLayer("belowmain");
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
    customPlot->setInteractions(QCP::iRangeDrag | QCP::iRangeZoom | QCP::iSelectPlottables);
    return customPlot;
}


