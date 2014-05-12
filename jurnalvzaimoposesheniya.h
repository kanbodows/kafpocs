#ifndef JURNALVZAIMOPOSESHENIYA_H
#define JURNALVZAIMOPOSESHENIYA_H

#include <QDialog>
#include "dals/dal_main.h"
#include"dals/dal_studentscontrol.h"
#include"add_edit_forms/addoreditjurnvzaimopos.h"
#include <QString>
#include <QList>
#include <QStringList>
#include "myprint.h"
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QPrintDialog>


namespace Ui {
class JurnalVzaimoposesheniya;
}

class JurnalVzaimoposesheniya : public QDialog
{
    Q_OBJECT

public:
    explicit JurnalVzaimoposesheniya(QWidget *parent = 0);
    ~JurnalVzaimoposesheniya();
    bool static isOpen;
    void refreshData();


private slots:
    void on_pushButton_del_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_edit_clicked();

    void on_actionEdit_triggered();

    void on_actionDelete_triggered();

    void on_tableView_jurnalVzaimopos_doubleClicked(const QModelIndex &index);

    void on_pushButton_search_clicked();

    void on_pushButtonPrint_clicked();

    void printDocument(QPrinter *printer);

    void on_pushButton_find_clicked();

    void on_dateEdit_editingFinished();

    void on_dateEdit_end_editingFinished();

    void on_pushButton_clear_clicked();

    void on_lineEdit_prepod_editingFinished();

    void on_tableView_jurnalVzaimopos_clicked(const QModelIndex &index);

private:
    Ui::JurnalVzaimoposesheniya *ui;
    QSqlQueryModel *JurnalVzaimoposModels;
    Dal_studentsControl *dal_studentsControl;
    AddOrEditJurnVzaimopos *addOrEditJurnVzaimoposform;
    bool vidim;
    int count;
//    QString Student;
//    QString Group;
    QString naideno;
    bool isDate;
    QDate dateBeginning;
    QDate dateEnd;
    QTextDocument *body;
    QString fioProver;

};

#endif // JURNALVZAIMOPOSESHENIYA_H
