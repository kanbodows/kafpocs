#ifndef ADDOREDITSUBWORK_H
#define ADDOREDITSUBWORK_H
#define INSERT 1
#define EDIT 2
#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_prepodcontrol.h"
#include <QMessageBox>
namespace Ui {
class AddOrEditSubWork;
}

class AddOrEditSubWork : public QDialog
{
    Q_OBJECT

public:
    explicit AddOrEditSubWork(QWidget *parent = 0, int select = 0, int id = -1);
    ~AddOrEditSubWork();
    bool isEdit;

private slots:
    void on_pushButton_ok_3_clicked();

private:
    Ui::AddOrEditSubWork *ui;
    Dal_prepodcontrol *dal_prepodcontrol;
    int discp_id;
    int sem_id;
    int prepd_id;
    int rec_id;
    bool sel;

};

#endif // ADDOREDITSUBWORK_H
