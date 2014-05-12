#ifndef ADDOREDIT_POVYSHKVAL_H
#define ADDOREDIT_POVYSHKVAL_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_prepodcontrol.h"
#include <QMessageBox>
#include <QDoubleValidator>

namespace Ui {
class Addoredit_PovyshKval;
}

class Addoredit_PovyshKval : public QDialog
{
    Q_OBJECT

public:
    explicit Addoredit_PovyshKval(QWidget *parent = 0, bool isEdit  = false, int id_povyshKval = -1);
    ~Addoredit_PovyshKval();
    bool isEdit;

    int id_povyshKval;

private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

    void on_pushButton_choose_clicked();

private:
    Ui::Addoredit_PovyshKval *ui;
    Dal_prepodcontrol* dal_prepodcontrol;
    QString NameFails;
    QString UrlFail;
};

#endif // ADDOREDIT_KONF_SEM_H
