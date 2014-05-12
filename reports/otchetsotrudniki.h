#ifndef OTCHETSOTRUDNIKI_H
#define OTCHETSOTRUDNIKI_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_prepodcontrol.h"
#include <QMessageBox>
#include "myprint.h"
#include <QPrintPreviewDialog>
namespace Ui {
class OtchetSotrudniki;
}

class OtchetSotrudniki : public QDialog
{
    Q_OBJECT
public:
    explicit OtchetSotrudniki(QWidget *parent = 0);
    ~OtchetSotrudniki();    
    bool static isOpen;
private slots:

    void printDocument(QPrinter *printer);

    void on_pushButton_showReport_clicked();

    void on_pushButton_set_clicked();

private:
    Ui::OtchetSotrudniki *ui;
    Dal_prepodcontrol *dal_prepodcontrol;
    QTextDocument* toPrint;
};

#endif // OTCHETSOTRUDNIKI_H
