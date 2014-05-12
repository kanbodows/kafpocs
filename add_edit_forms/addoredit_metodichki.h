#ifndef ADDOREDIT_METODICHKI_H
#define ADDOREDIT_METODICHKI_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_prepodcontrol.h"
#include <QMessageBox>
namespace Ui {
class addoredit_metodichki;
}

class addoredit_metodichki : public QDialog
{
    Q_OBJECT

public:
    explicit addoredit_metodichki(QWidget *parent = 0, bool isEdit  = false, int id_met = -1, int discipline_id = -1);
    ~addoredit_metodichki();
    bool isEdit;
    int id_met;
    int discipline_id;
private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

    void on_pushButton_choose_clicked();

    void on_comboBox_discipline_activated(int index);

private:
    Ui::addoredit_metodichki *ui;
    Dal_prepodcontrol* dal_prepodcontrol;
    QString NameFails;
    QString UrlFail;
};

#endif // ADDOREDIT_KONF_SEM_H
