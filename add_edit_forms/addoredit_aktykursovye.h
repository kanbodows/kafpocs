#ifndef ADDOREDIT_AKTYKURSOVYE_H
#define ADDOREDIT_AKTYKURSOVYE_H
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
class AddOrEdit_AktyKursovye;
}

class AddOrEdit_AktyKursovye : public QDialog
{
    Q_OBJECT

public:
    explicit AddOrEdit_AktyKursovye(QWidget *parent = 0, bool isEdit = false, int id_vedomostiKurs = 0);
    ~AddOrEdit_AktyKursovye();
     int id_vedomostiKurs;
     bool isEdit;
private slots:
    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_print_clicked();

    void printDocument(QPrinter *printer);

private:
    Ui::AddOrEdit_AktyKursovye *ui;
    Dal_studentsControl *dal_studentsControl;
    QTextDocument* toPrint;
    Styles *css;
};

#endif // ADDOREDIT_VEDOMOSTIKURSOVYE_H
