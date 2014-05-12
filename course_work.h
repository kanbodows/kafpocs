#ifndef COURSE_WORK_H
#define COURSE_WORK_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include "dals/dal_main.h"
#include <QString>
#include "dals/dal_studentscontrol.h"
#include "add_edit_forms/addoredit_course_work.h"

//extern DAL_main  *dal_main;
namespace Ui {
class Course_work;
}

class Course_work : public QDialog
{
    Q_OBJECT

public:
    explicit Course_work(QWidget *parent = 0);
    ~Course_work();
    bool static isOpen;
private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_find_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_search_clicked();

    void on_comboBox_group_activated(int index);

    void on_comboBox_discipline_activated(int index);

    void on_pushButton_edit_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_actionEdit_triggered();

    void on_actionDelete_triggered();

private:
    Ui::Course_work *ui;
    QSqlTableModel *groupsModel;
    QSqlTableModel *disciplineModel;
    Dal_studentsControl *dal_studentsControl;
    int group_id;
    int discipline;
public slots:
   void refreshData();
};

#endif // Course_work_H
