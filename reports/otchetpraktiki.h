#ifndef OTCHETPRAKTIKI_H
#define OTCHETPRAKTIKI_H

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
class OtchetPraktiki;
}

class OtchetPraktiki : public QDialog
{
    Q_OBJECT

public:
    explicit OtchetPraktiki(QWidget *parent = 0);
    ~OtchetPraktiki();
    bool static isOpen;

private slots:

    void printDocument(QPrinter *printer);

    void on_pushButton_showReport_clicked();

    void on_pushButton_graph_clicked();

    void on_pushButton_set_clicked();

private:
    Ui::OtchetPraktiki *ui;
    Dal_studentsControl *dal_studentsControl;
    Plots *plots;
    QTextDocument* body;
};

#endif // OTCHETPRAKTIKI_H
