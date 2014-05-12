#ifndef ADDOREDIT_KNIGI_H
#define ADDOREDIT_KNIGI_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_prepodcontrol.h"
#include <QMessageBox>
namespace Ui {
class addoredit_knigi;
}

class addoredit_knigi : public QDialog
{
    Q_OBJECT

public:
    explicit addoredit_knigi(QWidget *parent = 0, bool isEdit  = false, int id_knigi = -1, int discipline_id = -1);
    ~addoredit_knigi();
    bool isEdit;
    int id_knigi;
    int discipline_id;
private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

    void on_pushButton_choose_clicked();

private:
    Ui::addoredit_knigi *ui;
    Dal_prepodcontrol* dal_prepodcontrol;
    QString NameFails;
    QString UrlFail;
};

#endif // ADDOREDIT_KONF_SEM_H
