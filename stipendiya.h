#ifndef STIPENDIYA_H
#define STIPENDIYA_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include "dals/dal_main.h"
#include <QString>
extern DAL_main  *dal_main;
namespace Ui {
class Stipendiya;
}

class Stipendiya : public QDialog
{
    Q_OBJECT

public:
    explicit Stipendiya(QWidget *parent = 0);
    ~Stipendiya();
       bool static isOpen;

private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_search_clicked();

    void on_pushButton_find_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

    void on_comboBox_type_stipen_activated(int index);

    void on_pushButton_edit_clicked();

private:
    Ui::Stipendiya *ui;
    QSqlRelationalTableModel *IsStipendiaModel;
    QSqlTableModel *IsTypeStipendia;
    int count;
    int typeSTIP_id;
    QString naideno;

public slots:
    void formingSearch();
};

#endif // STIPENDIYA_H
