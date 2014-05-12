#ifndef ADDOREDIT_COURSE_WORK_H
#define ADDOREDIT_COURSE_WORK_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_prepodcontrol.h"
#include "dals/dal_studentscontrol.h"
#include <QMessageBox>

namespace Ui {
class addoredit_course_work;
}

class addoredit_course_work : public QDialog
{
    Q_OBJECT

public:
    explicit addoredit_course_work(QWidget *parent = 0, bool isEdit  = false, int id_course_work = -1);
    ~addoredit_course_work();
    bool isEdit;
    int id_course_work;
    int discipline_id;
private slots:
    void on_pushButton_cancel_clicked();

    void on_pushButton_ok_clicked();

    void on_comboBox_gruppa_currentIndexChanged(int index);

private:
    Ui::addoredit_course_work *ui;
    Dal_prepodcontrol* dal_prepodcontrol;
    Dal_studentsControl* dal_studentsControl;
};

#endif // ADDOREDIT_COURSE_WORK_H
