#ifndef ADDOREDITGAK_H
#define INSERT 1
#define EDIT 2
#define ADDOREDITGAK_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_studentscontrol.h"
#include "styles.h"
extern DAL_main *dal_main;

namespace Ui {
class AddOrEditGAK;
}

class AddOrEditGAK : public QDialog
{
    Q_OBJECT

public:
    explicit AddOrEditGAK(QWidget *parent = 0, int select = 0, int id = -1);
    ~AddOrEditGAK();
     bool isEdit;

private slots:
     void on_comboBoxd1_activated(int index);

     void on_comboBoxd2_activated(int index);

     void on_comboBoxd3_activated(int index);

     void on_comboBoxd4_activated(int index);

     void on_pushButton_clicked();


     void on_pushButton_2_clicked();

     void on_lineEdit_GodFormir_activated(int index);

private:
    Ui::AddOrEditGAK *ui;
    Dal_studentsControl *dal_studentsControl;
    int rec_id;
    int discp1;
    int discp2;
    int discp3;
    int discp4;
    int nomRaporta;
    Styles *css;
};

#endif // ADDOREDITGAK_H
