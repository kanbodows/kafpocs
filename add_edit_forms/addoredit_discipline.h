#ifndef ADDOREDIT_DISCIPLINE_H
#define ADDOREDIT_DISCIPLINE_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_prepodcontrol.h"
#include "dals/dal_studentscontrol.h"
#include <QMessageBox>
#include "addoredit_metodichki.h"
#include "addoredit_knigi.h"
namespace Ui {
class addoredit_discipline;
}

class addoredit_discipline : public QDialog
{
    Q_OBJECT

public:
    explicit addoredit_discipline(QWidget *parent = 0, bool isEdit  = false, int id_discip = -1);
    ~addoredit_discipline();
    bool isEdit;
    int id_discip;
    QString foto;
private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_choosePhoto_clicked();

    void on_pushButton_addMet_clicked();

    void on_pushButton_editMet_clicked();

    void on_pushButton_delMet_clicked();

    void on_pushButton_addKnigi_clicked();

    void on_pushButton_editKnigi_clicked();

    void on_pushButton_delKnigi_clicked();

    void on_pushButton_cancel_clicked();

    void on_tableView_metodichki_doubleClicked(const QModelIndex &index);

    void on_tableView_knigi_doubleClicked(const QModelIndex &index);

    void on_actionEditMet_triggered();

    void on_actionEditKniga_triggered();

    void on_actionDelMet_triggered();

    void on_actionDelKniga_triggered();

private:
    Ui::addoredit_discipline *ui;
    Dal_prepodcontrol* dal_prepodcontrol;
    Dal_studentsControl* dal_studentsControl;
};

#endif // ADDOREDIT_STUDENT_H
