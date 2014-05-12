#ifndef ADD_OR_EDIT_PLATNIEOBRAZOVATKURSI_H
#define ADD_OR_EDIT_PLATNIEOBRAZOVATKURSI_H
#define INSERT 1
#define EDIT 2
#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_prepodcontrol.h"
extern DAL_main *dal_main;

namespace Ui {
class add_or_edit_PlatnieObrazovatKursi;
}

class add_or_edit_PlatnieObrazovatKursi : public QDialog
{
    Q_OBJECT

public:
    explicit add_or_edit_PlatnieObrazovatKursi(QWidget *parent = 0, int select = 0, int id = -1);
    ~add_or_edit_PlatnieObrazovatKursi();
    bool isEdit;

private slots:
    void on_comboBoxPrepod_activated(int index);

    void on_pushButton_Cancel_clicked();

    void on_pushButtonOk_clicked();

private:
    Ui::add_or_edit_PlatnieObrazovatKursi *ui;
    Dal_prepodcontrol *dal_prepodcontrol;
    int sotr_ids;
    int rec_id;
};

#endif // ADD_OR_EDIT_PLATNIEOBRAZOVATKURSI_H
