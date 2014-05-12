#ifndef ADDOREDITSOTRUDNIK_H
#define INSERT 1
#define EDIT 2
#define VIEWSV 3
#define ADDOREDITSOTRUDNIK_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_prepodcontrol.h"
#include"smenaparrolya.h"
extern DAL_main *dal_main;
namespace Ui {
class AddOrEditSotrudnik;
}

class AddOrEditSotrudnik : public QDialog
{
    Q_OBJECT

public:
    explicit AddOrEditSotrudnik(QWidget *parent = 0, int select = 0, int id = -1, int viewsv = -1);
    ~AddOrEditSotrudnik();

    bool isEdit;

private slots:
    void on_pushButton_OK_clicked();

    void on_pushButton_Cancel_clicked();

    void on_pushButton_changePassword_clicked();

private:
    Ui::AddOrEditSotrudnik *ui;
    Dal_prepodcontrol *dal_prepodcontrol;
     SmenaParrolya *smenaForm;
    int doljnost;
    int rec_id;
    QString foto;
};

#endif // ADDOREDITSOTRUDNIK_H
