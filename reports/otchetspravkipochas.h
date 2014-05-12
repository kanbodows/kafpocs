#ifndef OTCHETSPRAVKIPOCHAS_H
#define OTCHETSPRAVKIPOCHAS_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_prepodcontrol.h"
#include <QMessageBox>
#include "myprint.h"
#include <QPrintPreviewDialog>
#include "styles.h"
namespace Ui {
class OtchetSpravkiPochas;
}

class OtchetSpravkiPochas : public QDialog
{
    Q_OBJECT
public:
    explicit OtchetSpravkiPochas(QWidget *parent = 0);
    ~OtchetSpravkiPochas();    
    bool static isOpen;
private slots:

    void printDocument(QPrinter *printer);

    void on_pushButton_showReport_clicked();

    void on_pushButton_set_clicked();

private:
    Ui::OtchetSpravkiPochas *ui;
    Dal_prepodcontrol *dal_prepodcontrol;
    QTextDocument* toPrint;
    Styles *css;
};

#endif // OTCHETPOVYSHKVAL_H
