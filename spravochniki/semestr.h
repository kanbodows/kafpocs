#ifndef SEMESTR_H
#define SEMESTR_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include <dals/dal_main.h>
#include <QString>
extern DAL_main  *dal_main;
namespace Ui {
class Semestr;
}

class Semestr : public QDialog
{
    Q_OBJECT

public:
    explicit Semestr(QWidget *parent = 0);
    ~Semestr();
    bool static isOpen;
    bool checkRelation(QString id_semestr);

private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_update_clicked();

    void on_pushBatton_cancel_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_find_clicked();

     void on_pushButton_search_clicked();

     void on_comboBox_course_activated(int index);

     void on_pushButton_clear_clicked();

     void on_pushButton_edit_clicked();

private:
       Ui::Semestr *ui;
       QSqlRelationalTableModel *SemestrModel;

       QSqlTableModel *CourseWorkModel2;
       int id_cour;
       QString naideno;

public slots:
       void formingSearch();
};

#endif // SEMESTR_H
