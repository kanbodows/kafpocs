#ifndef PARTNERS_H
#define PARTNERS_H

#include <QDialog>
#include "dals/dal_main.h"
#include"QSqlRelationalTableModel"
#include <QString>
extern DAL_main  *dal_main;
namespace Ui {
class Partners;
}

class Partners : public QDialog
{
    Q_OBJECT

public:
    explicit Partners(QWidget *parent = 0);
    ~Partners();
     bool static isOpen;
     bool checkRelation(QString partner_id);

private:
    Ui::Partners *ui;
    QSqlRelationalTableModel *PartnerModel;
    int count;
    QString naideno;
public slots:
    void formingSearch();
private slots:
    void on_pushButton_add_clicked();
    void on_pushButton_update_clicked();
    void on_pushButton_del_clicked();
    void on_pushButton_search_clicked();
    void on_pushButton_find_clicked();
    void on_pushButton_clear_clicked();
    void on_pushButton_save_clicked();
    void on_pushButton_cancel_clicked();
    void on_pushButton_edit_clicked();
};

#endif // PARTNERS_H
