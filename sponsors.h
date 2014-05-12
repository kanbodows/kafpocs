#ifndef SPONSORS_H
#define SPONSORS_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include "dals/dal_main.h"
#include <QString>
extern DAL_main  *dal_main;

namespace Ui {
class Sponsors;
}

class Sponsors : public QDialog
{
    Q_OBJECT

public:
    explicit Sponsors(QWidget *parent = 0);
    ~Sponsors();
    bool static isOpen;

private:
    Ui::Sponsors *ui;
    QSqlRelationalTableModel *SponsorModel;
    int count;
    int id_sponsors;
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

#endif // SPONSORS_H
