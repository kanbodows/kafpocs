#ifndef ADDOREDITVIPUSKNIK_H
#define INSERT 1
#define EDIT 2
#define VIEWSV 3
#define ADDOREDITVIPUSKNIK_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_studentscontrol.h"
extern DAL_main *dal_main;
namespace Ui {
class AddOrEditVipusknik;
}

class AddOrEditVipusknik : public QDialog
{
    Q_OBJECT

public:
    explicit AddOrEditVipusknik(QWidget *parent = 0, int select = 0, int id = -1, int viewsv = -1);
    ~AddOrEditVipusknik();
    bool isEdit;

private slots:
    void on_comboBox_grupa_activated(int index);

    void on_pushButton_addFoto_clicked();

    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

    void on_comboBox_student_activated(int index);

private:
    Ui::AddOrEditVipusknik *ui;
    Dal_studentsControl *dal_studentsControl;
    int stud_id;
    int group_id;
    int rec_id;
    QString foto;
};

#endif // ADDOREDITVIPUSKNIK_H
