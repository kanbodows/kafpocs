#ifndef ADDOREDITOLIMP_H
#define INSERT 1
#define EDIT 2
#define ADDOREDITOLIMP_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_studentscontrol.h"
extern DAL_main *dal_main;
namespace Ui {
class AddOrEditOlimp;
}

class AddOrEditOlimp : public QDialog
{
    Q_OBJECT

public:
    explicit AddOrEditOlimp(QWidget *parent = 0, int select = 0, int id = -1);
    ~AddOrEditOlimp();
    bool isEdit;

private slots:
    void on_pushButton_ok_clicked();

    void on_comboBox_grupa_activated(int index);

    void on_comboBox_student_activated(int index);

private:
    Ui::AddOrEditOlimp *ui;
     Dal_studentsControl *dal_studentsControl;
     int stud_id;
     int group_id;
     int rec_id;

};

#endif // ADDOREDITOLIMP_H
