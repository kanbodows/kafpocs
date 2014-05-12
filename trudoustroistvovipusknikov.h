#ifndef TRUDOUSTROISTVOVIPUSKNIKOV_H
#define TRUDOUSTROISTVOVIPUSKNIKOV_H

#include <QDialog>
#include "dals/dal_main.h"
#include"dals/dal_studentscontrol.h"
#include <QString>
#include "add_edit_forms/addoredittrudovip.h"
extern DAL_main  *dal_main;

namespace Ui {
class TrudoustroistvoVipusknikov;
}

class TrudoustroistvoVipusknikov : public QDialog
{
    Q_OBJECT

public:
    explicit TrudoustroistvoVipusknikov(QWidget *parent = 0);
    ~TrudoustroistvoVipusknikov();
    bool static isOpen;
    void refreshData();

private slots:
    void on_pushButton_search_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_find_clicked();

    void on_comboBox_group_activated(int index);

    void on_pushButton_del_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_edit_clicked();

    void on_actionEditRec_triggered();

    void on_actionDelete_triggered();

    void on_tableView_TrudoustrVipuskniki_doubleClicked(const QModelIndex &index);

private:
    Ui::TrudoustroistvoVipusknikov *ui;
    QSqlQueryModel *TrudoVipusknikModels;
    AddOrEditTrudoVip *AddOrEditTrudoVipform;
    Dal_studentsControl *dal_studentsControl;
    bool vidim;
    int count;
    QString Student;
    int Group;
    QString naideno;
};

#endif // TRUDOUSTROISTVOVIPUSKNIKOV_H
