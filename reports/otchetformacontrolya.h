#ifndef OTCHETFORMACONTROLYA_H
#define OTCHETFORMACONTROLYA_H

#include <QDialog>
#include <dals/dal_main.h>
#include <QString>
#include "myprint.h"
#include <QPrintPreviewDialog>
#include "dals/dal_studentscontrol.h"
extern DAL_main  *dal_main;

namespace Ui {
class OtchetFormaControlya;
}

class OtchetFormaControlya : public QDialog
{
    Q_OBJECT

public:
    explicit OtchetFormaControlya(QWidget *parent = 0);
    ~OtchetFormaControlya();
     bool static isOpen;

private slots:
     void printDocument(QPrinter *printer);

     void on_pushButton_showReport_clicked();

private:
    Ui::OtchetFormaControlya *ui;
    QSqlTableModel *formaControlyaModel;
    Dal_studentsControl* dal_studentsControl;
    QTextDocument *body;
};

#endif // OTCHETFORMACONTROLYA_H
