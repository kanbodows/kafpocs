#ifndef GAK_H
#define GAK_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include "dals/dal_main.h"
#include <QString>
#include "dals/dal_studentscontrol.h"
#include "add_edit_forms/addoreditgak.h"
#include "add_edit_forms/addoredit_gak_bally.h"
namespace Ui {
class GAK;
}

class GAK : public QDialog
{
    Q_OBJECT

public:
    explicit GAK(QWidget *parent = 0);
    ~GAK();
     bool static isOpen;

private:
    Ui::GAK *ui;
    QSqlQueryModel *GAKModel;
    QSqlTableModel *disciplineModel;
    QSqlQueryModel *SostKomGAKModel;
    Dal_studentsControl *dal_studentsControl;
    AddOrEditGAK *AddOrEditGAKform;
    int nomer_prikaza;
    int kom;
    int discipline;
    bool vidim;
    int id_raport;
    QSqlRelationalTableModel *sostGAKModel;
public slots:
   void refreshData();
private slots:
   void on_pushButton_search_clicked();
   void on_pushButton_clear_clicked();
   void on_pushButton_find_clicked();
   void on_pushButton_del_clicked();
   void on_pushButton_update_clicked();
   void on_lineEdit_nomerPrikaza_editingFinished();
   void on_pushButton_add_clicked();
   void on_pushButton_edit_clicked();
   void on_tableView_gak_clicked(const QModelIndex &index);
   void on_actionEdit_triggered();
   void on_actionDelete_triggered();
   void on_pushButton_addBally_clicked();
};

#endif // GAK_H
