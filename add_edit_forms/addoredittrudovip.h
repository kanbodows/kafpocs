#ifndef ADDOREDITTRUDOVIP_H
#define INSERT 1
#define EDIT 2
#define ADDOREDITTRUDOVIP_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_studentscontrol.h"
extern DAL_main *dal_main;
namespace Ui {
class AddOrEditTrudoVip;
}

class AddOrEditTrudoVip : public QDialog
{
    Q_OBJECT

public:
    explicit AddOrEditTrudoVip(QWidget *parent = 0, int select = 0, int id = -1);
    ~AddOrEditTrudoVip();
    bool isEdit;

private slots:
    void on_comboBoxGroup_activated(int index);

    void on_pushButton_clicked();

    void on_comboBox_Vipusk_activated(int index);

    void on_pushButton_2_clicked();

private:
    Ui::AddOrEditTrudoVip *ui;
    Dal_studentsControl *dal_studentsControl;
    int vip_id;
    int group_id;
    int rec_id;
};

#endif // ADDOREDITTRUDOVIP_H
