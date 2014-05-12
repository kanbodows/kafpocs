#ifndef ADDOREDITSOSTAVKOM_H
#define INSERT 1
#define EDIT 2
#define ADDOREDITSOSTAVKOM_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_prepodcontrol.h"
#include "styles.h"
extern DAL_main *dal_main;

namespace Ui {
class AddOrEditSostavKom;
}

class AddOrEditSostavKom : public QDialog
{
    Q_OBJECT

public:
    explicit AddOrEditSostavKom(QWidget *parent = 0, int select = 0, int id = -1, int viewsv = -1);
    ~AddOrEditSostavKom();
    bool isEdit;

private slots:
    void on_comboBox_Predsed_activated(int index);

    void on_comboBox_Secr_activated(int index);

    void on_comboBox_Ch1_activated(int index);

    void on_comboBox_Ch2_activated(int index);

    void on_comboBox_Ch3_activated(int index);

    void on_comboBox_Ch4_activated(int index);

    void on_comboBox_Ch5_activated(int index);

    void on_pushButton_begin_clicked();

    void on_pushButton_ok_clicked();

    void on_pushButtonCancel_clicked();

private:
    Ui::AddOrEditSostavKom *ui;
    Dal_prepodcontrol *dal_prepodcontrol;
    int rec_id;
    int preds;
    int secretar;
    int chl1;
    int chl2;
    int chl3;
    int chl4;
    int chl5;
    Styles *css;
};

#endif // ADDOREDITSOSTAVKOM_H
