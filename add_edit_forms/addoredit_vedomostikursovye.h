#ifndef ADDOREDIT_VEDOMOSTIKURSOVYE_H
#define ADDOREDIT_VEDOMOSTIKURSOVYE_H
#include "dals/dal_main.h"
#include <QSqlRelationalTableModel>
#include "dals/dal_studentscontrol.h"
#include <QString>
#include <QDialog>
#include <QTableWidgetItem>
#include "myprint.h"
#include <QPrintPreviewDialog>
#include "styles.h"

namespace Ui {
class AddOrEdit_VedomostiKursovye;
}

class AddOrEdit_VedomostiKursovye : public QDialog
{
    Q_OBJECT

public:
    explicit AddOrEdit_VedomostiKursovye(QWidget *parent = 0, bool isEdit = false, int id_vedomostiKurs = 0);
    ~AddOrEdit_VedomostiKursovye();
     int id_vedomostiKurs;
     bool isEdit;
private slots:

    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

    void on_tableWidget_itemChanged(QTableWidgetItem *item);

    void on_pushButton_print_clicked();

    void printDocument(QPrinter *printer);

    void on_comboBox_spec_activated(const QString &arg1);

    void on_comboBox_group_activated(const QString &arg1);

    void on_comboBox_discipline_activated(const QString &arg1);

private:
    Ui::AddOrEdit_VedomostiKursovye *ui;
    Dal_studentsControl *dal_studentsControl;
    void getCourseWorks();
    QTextDocument* toPrint;
    Styles *css;
};

#endif // ADDOREDIT_VEDOMOSTIKURSOVYE_H
