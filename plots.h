#ifndef PLOTS_H
#define PLOTS_H
#include <QObject>
#include <QSqlDatabase>
#include <QSqlQueryModel>
#include <QString>
#include "myprint.h"
#include "delegates/qcustomplot.h"
#include <QSqlQuery>
#include <QtSql>
//
class Plots : public QObject
{
    Q_OBJECT
public:
   
    explicit Plots(QObject *parent = 0);
    ~Plots();   
    QSqlDatabase db;
    QCustomPlot* createChart(QVector<double> x, QVector<double> y, QString chartName);
signals:
    
public slots:
private:


    
};

#endif // PLOTS_H
