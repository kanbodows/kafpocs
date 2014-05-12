#ifndef OTCHETPOVYSHKVAL_H
#define OTCHETPOVYSHKVAL_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_prepodcontrol.h"
#include <QMessageBox>
#include "myprint.h"
#include <QPrintPreviewDialog>
#include "styles.h"
namespace Ui {
class OtchetPovyshKval;
}

class OtchetPovyshKval : public QDialog
{
    Q_OBJECT
public:
    explicit OtchetPovyshKval(QWidget *parent = 0);
    ~OtchetPovyshKval();    
    bool static isOpen;
private slots:

    void printDocument(QPrinter *printer);

    void on_pushButton_showReport_clicked();

    void on_pushButton_set_clicked();

private:
    Ui::OtchetPovyshKval *ui;
    Dal_prepodcontrol *dal_prepodcontrol;
    QTextDocument* toPrint;
    Styles *css;
};

#endif // OTCHETPOVYSHKVAL_H
