#ifndef BOLEZNISOTRUDNIKOV_H
#define BOLEZNISOTRUDNIKOV_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include "dals/dal_main.h"
#include <QString>
extern DAL_main  *dal_main;
namespace Ui {
class BolezniSotrudnikov;
}

class BolezniSotrudnikov : public QDialog
{
    Q_OBJECT

public:
    explicit BolezniSotrudnikov(QWidget *parent = 0);
    ~BolezniSotrudnikov();
    bool static isOpen;

private:
    Ui::BolezniSotrudnikov *ui;
    QSqlRelationalTableModel *BoleznSotrModel;
    QSqlTableModel *ComboSotrudnikiModel;
    int count;
    int sotrudnik_id;
    QDate dateBeginning;
    QDate dateEnd;
    QString naideno;
    bool isDate;
public slots:
    void formingSearch();
private slots:
    void on_pushButton_add_clicked();
    void on_pushButton_update_clicked();
    void on_pushButton_del_clicked();
    void on_pushButton_search_clicked();
    void on_pushButton_find_clicked();
    void on_pushButton_clear_clicked();
    void on_comboBox_sotrudnik_activated(int index);
    void on_pushButton_save_clicked();
    void on_pushButton_cancel_clicked();
    void on_dateEdit_start_dateChanged(const QDate &date);
    void on_dateEdit_end_dateChanged(const QDate &date);
    void on_pushButton_edit_clicked();
    void on_actionEdit_triggered();
    void on_actionDelete_triggered();
    void on_tableView_sotrBolez_doubleClicked(const QModelIndex &index);
};

#endif // BOLEZNISOTRUDNIKOV_H
