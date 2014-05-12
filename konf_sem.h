#ifndef KONF_SEM_H
#define KONF_SEM_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_studentscontrol.h"
#include "add_edit_forms/addoredit_konf_sem.h"
#include <QString>
extern DAL_main  *dal_main;
namespace Ui {
class Konf_sem;
}

class Konf_sem : public QDialog
{
    Q_OBJECT

public:
    explicit Konf_sem(QWidget *parent = 0);
    ~Konf_sem();
    bool static isOpen;
private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_find_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_search_clicked();

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_pushButton_edit_clicked();

    void on_actionEdit_triggered();

    void on_actionDelete_triggered();

private:
    Ui::Konf_sem *ui;
    Dal_studentsControl * dal_studentsControl;
    QSqlQueryModel *konf_semModel;
public slots:
    void refreshData();
};

#endif // Konf_sem_H
