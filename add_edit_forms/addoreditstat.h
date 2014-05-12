#ifndef ADDOREDITSTAT_H
#define INSERT 1
#define EDIT 2
#define ADDOREDITSTAT_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_prepodcontrol.h"

extern DAL_main *dal_main;
namespace Ui {
class AddOrEditStat;
}

class AddOrEditStat : public QDialog
{
    Q_OBJECT

public:
    explicit AddOrEditStat(QWidget *parent = 0, int select = 0, int id = -1);
    ~AddOrEditStat();
     bool isEdit;

private slots:
     void on_pushButton_add_clicked();

     void on_comboBox_avtor_currentIndexChanged(int index);

     void on_comboBox_soAvtor_activated(int index);

     void on_pushButton_cancel_clicked();

private:
    Ui::AddOrEditStat *ui;
    Dal_prepodcontrol *dal_prepodcontrol;
    int id_avtor;
    int id_soavtors;
    int rec_id;
};

#endif // ADDOREDITSTAT_H
