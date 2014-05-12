#ifndef ADDOREDIT_KONF_SEM_H
#define ADDOREDIT_KONF_SEM_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_studentscontrol.h"
#include <QMessageBox>
namespace Ui {
class addoredit_konf_sem;
}

class addoredit_konf_sem : public QDialog
{
    Q_OBJECT

public:
    explicit addoredit_konf_sem(QWidget *parent = 0, bool isEdit  = false, int id_konf = -1);
    ~addoredit_konf_sem();
    bool isEdit;
    int id_konf;
private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

    void on_comboBox_group_currentIndexChanged(int index);

private:
    Ui::addoredit_konf_sem *ui;
    Dal_studentsControl* dal_studentsControl;
};

#endif // ADDOREDIT_KONF_SEM_H
