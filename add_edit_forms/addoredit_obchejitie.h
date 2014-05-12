#ifndef ADDOREDIT_OBCHEJITIE_H
#define ADDOREDIT_OBCHEJITIE_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_studentscontrol.h"
#include <QMessageBox>
namespace Ui {
class addoredit_obchejitie;
}

class addoredit_obchejitie : public QDialog
{
    Q_OBJECT

public:
    explicit addoredit_obchejitie(QWidget *parent = 0, bool isEdit  = false, int id_obch = -1);
    ~addoredit_obchejitie();
    bool isEdit;
    int id_obch;
private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

    void on_comboBox_group_currentIndexChanged(int index);

private:
    Ui::addoredit_obchejitie *ui;
    Dal_studentsControl* dal_studentsControl;
};

#endif // ADDOREDIT_KONF_SEM_H
