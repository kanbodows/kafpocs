#ifndef OTCHETTRUDOUSTRVIP_H
#define OTCHETTRUDOUSTRVIP_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_studentscontrol.h"
#include <QString>
#include "myprint.h"
#include "delegates/qcustomplot.h"
#include <QSqlQuery>
#include "plots.h"

extern DAL_main  *dal_main;
namespace Ui {
class OtchetTrudoUstrVip;
}

class OtchetTrudoUstrVip : public QDialog
{
    Q_OBJECT

public:
    explicit OtchetTrudoUstrVip(QWidget *parent = 0);
    ~OtchetTrudoUstrVip();
    bool static isOpen;

private slots:

    void printDocument(QPrinter *printer);

    void on_pushButton_showReport_clicked();

    void on_pushButton_graph_clicked();

private:
    Ui::OtchetTrudoUstrVip *ui;
    Dal_studentsControl *dal_studentsControl;
    Plots *plots;
    QTextDocument* body;
};

#endif // OTCHETTRUDOUSTRVIP_H
