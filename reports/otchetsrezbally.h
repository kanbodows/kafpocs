#ifndef OTCHETSREZBALLY_H
#define OTCHETSREZBALLY_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_studentsControl.h"
#include <QMessageBox>
#include "myprint.h"
#include <QPrintPreviewDialog>
#include "delegates/qcustomplot.h"
#include <QSqlQuery>
#include "plots.h"
namespace Ui {
class Otchetsrezbally;
}

class Otchetsrezbally : public QDialog
{
    Q_OBJECT

public:
    explicit Otchetsrezbally(QWidget *parent = 0);
    ~Otchetsrezbally();
    bool static isOpen;
private slots:
    void printDocument(QPrinter *printer);
    void on_pushButton_showReport_clicked();

    void on_pushButton_graph_clicked();

private:
    Ui::Otchetsrezbally *ui;
    Dal_studentsControl *dal_studentsControl;
    QSqlQuery *querySrez;
    QTextDocument *bodyPrint;
    Plots *plots;
//    QVector<double> year(query->size()), y1(query->size()), y2(query->size()), y3(query->size()), y4(query->size());
    QVector<QString> labels;
    Styles *css;
};

#endif // OTCHETSREZBALLY_H
