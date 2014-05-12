#ifndef KUR_RAB_H
#define KUR_RAB_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include "dals/dal_main.h"
#include <QString>
extern DAL_main  *dal_main;
namespace Ui {
class Kur_rab;
}

class Kur_rab : public QDialog
{
    Q_OBJECT

public:
    explicit Kur_rab(QWidget *parent = 0);
    ~Kur_rab();
    bool static isOpen;
    bool checkRelation(QString discipline_id);
private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_del_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_find_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

    void on_comboBox_kutator_activated(int index);

    void on_pushButton_search_clicked();

    void on_pushButton_edit_clicked();

    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::Kur_rab *ui;
    QSqlRelationalTableModel *kur_rabModel;
    QSqlTableModel *sotrudnikiModel;
    int count;
    int sotrudnik_id;
    QString naideno;
public slots:
    void formingSearch();
};

#endif // Kur_rab_H
