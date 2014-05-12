#ifndef ADDOREDIT_PORUCHENIE_H
#define ADDOREDIT_PORUCHENIE_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_prepodcontrol.h"
#include <QMessageBox>
#include <QDoubleValidator>

namespace Ui {
class Addoredit_Poruchenie;
}

class Addoredit_Poruchenie : public QDialog
{
    Q_OBJECT

public:
    explicit Addoredit_Poruchenie(QWidget *parent = 0, bool isEdit  = false, int id_porucheniya = -1);
    ~Addoredit_Poruchenie();
    bool isEdit;
    int id_porucheniya;
    QString NameFails;
    QString UrlFail;

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

    void on_pushButton_provereno_clicked();

    void on_pushButton_choose_clicked();

private:
    Ui::Addoredit_Poruchenie *ui;
    Dal_prepodcontrol* dal_prepodcontrol;
};

#endif // ADDOREDIT_PORUCHENIE_H
