#ifndef DISCIPLINE_H
#define DISCIPLINE_H
#include <QDialog>
#include <QSqlRelationalTableModel>
#include <dals/dal_main.h>
#include <dals/dal_prepodcontrol.h>
#include <QString>
#include "add_edit_forms/addoredit_discipline.h"
#include "add_edit_forms/addoredit_prepoddiscipline.h"

extern DAL_main  *dal_main;
namespace Ui {
class Discipline;
}

class Discipline : public QDialog
{
    Q_OBJECT

public:
    explicit Discipline(QWidget *parent = 0);
    ~Discipline();
     bool static isOpen;

private slots:

    void on_pushButton_add_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_update_clicked();

    void on_comboBox_semestr_activated(int index);

    void on_pushButton_search_clicked();

    void on_pushButton_find_clicked();

    void on_pushButton_clear_clicked();

    void on_tableView_discipline_clicked();

    void on_pushButton_edit_clicked();

    void on_actionEditDiscipline_triggered();

    void on_actionDelDiscipline_triggered();

    void on_pushButton_setPrepod_clicked();

    void on_tableView_discipline_doubleClicked(const QModelIndex &index);

private:
    Ui::Discipline *ui;
    Dal_prepodcontrol *dal_prepodcontrol;
    QSqlTableModel *ComboBoxSemestrModel;
    int semestr_id;
public slots:
    void refreshData();

};

#endif // DISCIPLINE_H
