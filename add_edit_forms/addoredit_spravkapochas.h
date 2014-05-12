#ifndef ADDOREDIT_SPRAVKAPOCHAS_H
#define ADDOREDIT_SPRAVKAPOCHAS_H
#include "dals/dal_main.h"
#include <QSqlRelationalTableModel>
#include "dals/dal_studentscontrol.h"
#include "dals/dal_prepodcontrol.h"
#include <QString>
#include <QDialog>
#include <QTableWidgetItem>
#include "myprint.h"
#include <QPrintPreviewDialog>
#include "styles.h"

namespace Ui {
class AddOrEdit_SpravkaPochas;
}

class AddOrEdit_SpravkaPochas : public QDialog
{
    Q_OBJECT

public:
    explicit AddOrEdit_SpravkaPochas(QWidget *parent = 0, bool isEdit = false, int id_spravkaPochas = 0);
    ~AddOrEdit_SpravkaPochas();
     int id_spravkaPochas;
     bool isEdit;
private slots:

    void on_pushButton_ok_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_print_clicked();

    void printDocument(QPrinter *printer);

    void on_toolButton_add_clicked();

    void on_toolButton_delete_clicked();

    void on_toolButton_edit_clicked();

    void on_tableWidget_cellChanged(int row, int column);

private:
    Ui::AddOrEdit_SpravkaPochas *ui;
    Dal_studentsControl *dal_studentsControl;
    Dal_prepodcontrol *dal_prepodcontrol;
    QTextDocument* toPrint;
    Styles *css;
};

#endif // ADDOREDIT_SPRAVKAPOCHAS_H
