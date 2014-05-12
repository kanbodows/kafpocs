#ifndef ADDOREDIT_GAK_BALLY_H
#define ADDOREDIT_GAK_BALLY_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_studentscontrol.h"
#include <QMessageBox>

namespace Ui {
class addoredit_gak_bally;
}

class addoredit_gak_bally : public QDialog
{
    Q_OBJECT

public:
    explicit addoredit_gak_bally(QWidget *parent = 0, int id_gak = -1);
    ~addoredit_gak_bally();
    int id_gak;
private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_save_clicked();

    void on_comboBox_group_activated(int index);

    void on_comboBox_spec_activated(int index);

private:
    Ui::addoredit_gak_bally *ui;
    Dal_studentsControl* dal_studentscontrol;
    int count;
};

#endif // ADDOREDIT_SREZ_H
