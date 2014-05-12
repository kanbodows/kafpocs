#ifndef VAKANSII_H
#define VAKANSII_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include "dals/dal_main.h"
#include <QString>
extern DAL_main  *dal_main;

namespace Ui {
class Vakansii;
}

class Vakansii : public QDialog
{
    Q_OBJECT

public:
    explicit Vakansii(QWidget *parent = 0);
    ~Vakansii();
    bool static isOpen;

private slots:
    void on_comboBox_znak_activated(const QString &arg1);

    void on_pushButton_add_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_del_clicked();


    void on_pushButton_search_clicked();

    void on_pushButton_find_clicked();

    void on_lineEdit_vakansia_editingFinished();

    void on_doubleSpinBox_ZarplataOt_editingFinished();

    void on_doubleSpinBox_zarplataDo_editingFinished();

    void on_pushButtonSave_clicked();

    void on_pushButton_Cancel_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_edit_clicked();

private:
    Ui::Vakansii *ui;
    QString Znak;
    QSqlRelationalTableModel *VakansiiModel;
    int count;
    QString naideno;
    QString naz_vakan;
    int ZarplataOt;
    int ZarplataDo;
    bool vidim;

public slots:
    void formingSearch();
};

#endif // VAKANSII_H
