#ifndef ADDOREDIT_SREZ_BALLY_H
#define ADDOREDIT_SREZ_BALLY_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_studentscontrol.h"
#include <QMessageBox>

namespace Ui {
class addoredit_srez_bally;
}

class addoredit_srez_bally : public QDialog
{
    Q_OBJECT

public:
    explicit addoredit_srez_bally(QWidget *parent = 0, int id_srez = -1);
    ~addoredit_srez_bally();
    int id_srez;
private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_save_clicked();

private:
    Ui::addoredit_srez_bally *ui;
    Dal_studentsControl* dal_studentscontrol;
    int count;
};

#endif // ADDOREDIT_SREZ_H
