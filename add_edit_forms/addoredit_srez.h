#ifndef ADDOREDIT_SREZ_H
#define ADDOREDIT_SREZ_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_studentscontrol.h"
#include <QMessageBox>

namespace Ui {
class addoredit_srez;
}

class addoredit_srez : public QDialog
{
    Q_OBJECT

public:
    explicit addoredit_srez(QWidget *parent = 0, bool isEdit  = false, int id_srez = -1);
    ~addoredit_srez();
    bool isEdit;
    int id_srez;
private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

    void on_pushButton_choose_clicked();

private:
    Ui::addoredit_srez *ui;
    Dal_studentsControl* dal_studentscontrol;
};

#endif // ADDOREDIT_SREZ_H
