#ifndef KNIGI_H
#define KNIGI_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include "dals/dal_main.h"
#include <QString>
#include "add_edit_forms/addoredit_knigi.h"
#include "dals/dal_prepodcontrol.h"

extern DAL_main  *dal_main;
namespace Ui {
class Knigi;
}

class Knigi : public QDialog
{
    Q_OBJECT
public:
    explicit Knigi(QWidget *parent = 0);
    ~Knigi();
    bool static isOpen;
private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_edit_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_find_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_search_clicked();

    void on_comboBox_discipline_activated(int index);

    void on_tableView_doubleClicked(const QModelIndex &index);

    void on_lineEdit_nazvanieKnigi_editingFinished();

    void on_actionEdit_triggered();

    void on_actionDelete_triggered();

private:
    Ui::Knigi *ui;
     Dal_prepodcontrol *dal_prepodcontrol;
    QSqlQueryModel *knigiModel;
    QSqlTableModel *disciplineModel;
    int discipline;
    QString nazvanies;

public slots:
    void refreshData();
};

#endif // Knigi_H
