#ifndef ADDOREDIT_STUDENT_H
#define ADDOREDIT_STUDENT_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_studentscontrol.h"
#include <QMessageBox>
#include <QNetworkAccessManager>
namespace Ui {
class addoredit_student;
}

class addoredit_student : public QDialog
{
    Q_OBJECT

public:
    explicit addoredit_student(QWidget *parent = 0, bool isEdit  = false, int id_stud = -1);
    ~addoredit_student();
    bool isEdit;
    int id_stud;
    QString foto;
private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_choosePhoto_clicked();

//    void ftpDone(bool error);

    void on_pushButton_cancel_clicked();

    void on_comboBox_spec_activated(int index);

private:
    Ui::addoredit_student *ui;
    Dal_studentsControl* dal_studentsControl;
//    QFtp ftp;
    QFile file;
    int id_spec;
};

#endif // ADDOREDIT_STUDENT_H
