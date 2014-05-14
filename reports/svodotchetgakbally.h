#ifndef SVODOTCHETGAKBALLY_H
#define SVODOTCHETGAKBALLY_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_studentscontrol.h"
#include <QString>
#include "myprint.h"
#include "delegates/qcustomplot.h"
#include <QSqlQuery>
#include "plots.h"
#include "styles.h"

extern DAL_main  *dal_main;
namespace Ui {
class SvodOtchetGakBally;
}

class SvodOtchetGakBally : public QDialog
{
    Q_OBJECT

public:
    explicit SvodOtchetGakBally(QWidget *parent = 0);
    ~SvodOtchetGakBally();
    bool static isOpen;

private slots:

    void printDocument(QPrinter *printer);

    void on_pushButton_showReport_clicked();

    void on_pushButton_graph_clicked();

    void on_comboBox_spec_activated(const QString &arg1);

private:
    Ui::SvodOtchetGakBally *ui;
    Dal_studentsControl *dal_studentsControl;
    Plots *plots;
    QTextDocument* toPrint;
    Styles *css;
};

#endif // SvodOtchetGakBally_H
