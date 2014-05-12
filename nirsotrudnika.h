#ifndef NIRSOTRUDNIKA_H
#define NIRSOTRUDNIKA_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include "dals/dal_main.h"
#include <QString>
extern DAL_main  *dal_main;
namespace Ui {
class NirSotrudnika;
}

class NirSotrudnika : public QDialog
{
    Q_OBJECT

public:
    explicit NirSotrudnika(QWidget *parent = 0);
    ~NirSotrudnika();
     bool static isOpen;

private:
    Ui::NirSotrudnika *ui;
    QSqlRelationalTableModel *nir_sotrTable;
    QSqlTableModel *ComboSotr;
    int count;
    int nir_sotr_id;
    QString naideno;

public slots:
    void formingSearch();
private slots:
    void on_pushButton_add_clicked();
    void on_pushButton_update_clicked();
    void on_pushButton_del_clicked();
    void on_pushButton_search_clicked();
    void on_comboBox_sotrudnik_activated(int index);
    void on_pushButton_find_clicked();
    void on_pushButton_clear_clicked();
    void on_pushButton_save_clicked();
    void on_pushButton_cancel_clicked();
    void on_pushButton_edit_clicked();
    void on_tableView_nirSotr_clicked(const QModelIndex &index);
};

#endif // NIRSOTRUDNIKA_H
