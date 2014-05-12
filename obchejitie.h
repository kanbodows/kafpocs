#ifndef OBCHEJITIE_H
#define OBCHEJITIE_H

#include <QDialog>
#include "dals/dal_main.h"
#include "dals/dal_studentscontrol.h"
#include "add_edit_forms/addoredit_obchejitie.h"
#include <QString>
extern DAL_main  *dal_main;
namespace Ui {
class Obchejitie;
}

class Obchejitie : public QDialog
{
    Q_OBJECT

public:
    explicit Obchejitie(QWidget *parent = 0);
    ~Obchejitie();
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
    Ui::Obchejitie *ui;
    Dal_studentsControl * dal_studentsControl;
public slots:
    void refreshData();
};

#endif // Obchejitie_H
