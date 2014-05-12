#ifndef SVODOTCHETPRAKTIKI_H
#define SVODOTCHETPRAKTIKI_H

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
class SvodOtchetPraktiki;
}

class SvodOtchetPraktiki : public QDialog
{
    Q_OBJECT

public:
    explicit SvodOtchetPraktiki(QWidget *parent = 0);
    ~SvodOtchetPraktiki();
    bool static isOpen;

private slots:

    void printDocument(QPrinter *printer);

    void on_pushButton_showReport_clicked();

    void on_pushButton_graph_clicked();

private:
    Ui::SvodOtchetPraktiki *ui;
    Dal_studentsControl *dal_studentsControl;
    Plots *plots;
    QTextDocument* body;
};

#endif // SvodOtchetPraktiki_H
