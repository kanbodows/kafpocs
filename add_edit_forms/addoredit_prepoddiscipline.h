#ifndef ADDOREDIT_PREPODDISCIPLINE_H
#define ADDOREDIT_PREPODDISCIPLINE_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_prepodcontrol.h"
#include <QMessageBox>

namespace Ui {
class addoredit_prepodDiscipline;
}

class addoredit_prepodDiscipline : public QDialog
{
    Q_OBJECT

public:
    explicit addoredit_prepodDiscipline(QWidget *parent = 0, int discipline_id = -1);
    ~addoredit_prepodDiscipline();
    int discipline_id;

private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

    void on_listView_gruppa_clicked(const QModelIndex &index);

private:
    Ui::addoredit_prepodDiscipline *ui;
    Dal_prepodcontrol* dal_prepodcontrol;

};

#endif // ADDOREDIT_PREPODDISCIPLINE_H
