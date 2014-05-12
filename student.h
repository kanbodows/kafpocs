#ifndef STUDENT_H
#define STUDENT_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_studentscontrol.h"
#include <QString>
#include "add_edit_forms/addoredit_student.h"

extern DAL_main  *dal_main;
namespace Ui {
class Student;
}

class Student : public QDialog
{
    Q_OBJECT

public:
    explicit Student(QWidget *parent = 0);
    ~Student();
    bool static isOpen;

private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_find_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_search_clicked();

    void on_pushButton_edit_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_comboBox_gruppa_activated(int index);

    void on_actionEdit_triggered();

    void on_actionDelete_triggered();

    void on_comboBox_spec_activated(const QString &arg1);

private:
    Ui::Student *ui;
    bool vidim;
    Dal_studentsControl *dal_studentsControl;
    int gruppa;
    int spec;
public slots:
    void refreshData();
};

#endif // Student_H
