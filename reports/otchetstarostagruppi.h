#ifndef OTCHETSTAROSTAGRUPPI_H
#define OTCHETSTAROSTAGRUPPI_H

#include <QDialog>
#include <dals/dal_main.h>
#include <QString>
#include "myprint.h"
#include <QPrintPreviewDialog>

extern DAL_main  *dal_main;

namespace Ui {
class OtchetStarostaGruppi;
}

class OtchetStarostaGruppi : public QDialog
{
    Q_OBJECT

public:
    explicit OtchetStarostaGruppi(QWidget *parent = 0);
    ~OtchetStarostaGruppi();
     bool static isOpen;

private slots:
     void printDocument(QPrinter *printer);

     void on_pushButton_showReport_clicked();

private:
    Ui::OtchetStarostaGruppi *ui;
    QSqlTableModel *ReportStarostGroupModel;
    QTextDocument* body;
};

#endif // OTCHETSTAROSTAGRUPPI_H
