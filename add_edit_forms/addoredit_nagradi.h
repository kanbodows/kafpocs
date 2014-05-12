#ifndef ADDOREDIT_NAGRADI_H
#define ADDOREDIT_NAGRADI_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_prepodcontrol.h"
#include <QMessageBox>
#include <QDoubleValidator>

namespace Ui {
class Addoredit_Nagradi;
}

class Addoredit_Nagradi : public QDialog
{
    Q_OBJECT

public:
    explicit Addoredit_Nagradi(QWidget *parent = 0, bool isEdit  = false, int id_nagradi = -1);
    ~Addoredit_Nagradi();
    bool isEdit;
    int id_nagradi;

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

    void on_pushButton_choose_clicked();

private:
    Ui::Addoredit_Nagradi *ui;
    Dal_prepodcontrol* dal_prepodcontrol;
};

#endif // ADDOREDIT_NAGRADI_H
