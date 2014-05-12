#ifndef ADDOREDITPRACWORK_H
#define INSERT 1
#define EDIT 2
#define VIEWSV 3
#define ADDOREDITPRACWORK_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_studentscontrol.h"
extern DAL_main *dal_main;
namespace Ui {
class AddOrEditPracWork;
}

class AddOrEditPracWork : public QDialog
{
    Q_OBJECT

public:
    explicit AddOrEditPracWork(QWidget *parent = 0, int select = 0, int id = -1, int viewsv = -1);
    ~AddOrEditPracWork();
    bool isEdit;


private slots:
    void on_comboBox_grupa_activated(int index);

    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

private:
    Ui::AddOrEditPracWork *ui;
    Dal_studentsControl *dal_studentsControl;
    int stud_id;
    int group_id;
    int rucKafedr;
    int semmestrd;
    int type_prac;
    int rec_id;
};

#endif // ADDOREDITPRACWORK_H
