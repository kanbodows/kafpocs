#ifndef ISSPORT_H
#define ISSPORT_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include "dals/dal_main.h"
#include <QString>
extern DAL_main  *dal_main;
namespace Ui {
class IsSport;
}

class IsSport : public QDialog
{
    Q_OBJECT

public:
    explicit IsSport(QWidget *parent = 0);
    ~IsSport();
    bool static isOpen;

private:
    Ui::IsSport *ui;
    QSqlRelationalTableModel *IsSportModel;
    QSqlTableModel *studentModel;
    int count;
    int student_id;
    QString naideno;
public slots:
    void formingSearch();
private slots:
    void on_pushButton_add_clicked();
    void on_pushButton_update_clicked();
    void on_pushButton_del_clicked();
    void on_pushButton_search_clicked();
    void on_pushButton_find_clicked();
    void on_pushButton_clear_clicked();
    void on_comboBox_student_activated(int index);
    void on_pushButton_save_clicked();
    void on_pushButton_cancel_clicked();
    void on_pushButton_edit_clicked();
};

#endif // ISSPORT_H
