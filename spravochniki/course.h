#ifndef COURSE_H
#define COURSE_H

#include <QWidget>
#include "dals/dal_main.h"
#include "mainwindow.h"
extern DAL_main  *dal_main;
namespace Ui {
class Course;
}

class Course : public QWidget
{
    Q_OBJECT

public:
    explicit Course(QWidget *parent = 0);
    ~Course();
    bool static isOpen;
    bool checkRelation(QString id_course);

private slots:

    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_dell_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_edit_clicked();

private:
    Ui::Course *ui;
    QSqlTableModel *CourseModel;
    int count;

};

#endif // COURSE_H
