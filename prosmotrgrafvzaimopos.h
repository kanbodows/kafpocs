#ifndef PROSMOTRGRAFVZAIMOPOS_H
#define PROSMOTRGRAFVZAIMOPOS_H

#include <QDialog>
#include "dals/dal_prepodcontrol.h"
#include "prosmotrgrafvzaimopos.h"
#include "styles.h"
#include "add_edit_forms/addgrafvzaimopos.h"
#include <QList>
#include <QStringList>
#include "myprint.h"
#include <QPrinter>
#include <QPrintPreviewDialog>
#include <QPrintDialog>
#include <dals/dal_main.h>
#include <QString>

extern DAL_main *dal_main;

namespace Ui {
class ProsmotrGrafVzaimopos;
}

class ProsmotrGrafVzaimopos : public QDialog
{
    Q_OBJECT

public:
    explicit ProsmotrGrafVzaimopos(QWidget *parent = 0, QString GodSS="", QString GodPo="");
    ~ProsmotrGrafVzaimopos();

private slots:

    void on_pushButtonClose_clicked();

    void on_pushButtonAdd_clicked();

    void on_pushButtonEdit_clicked();

    void on_pushButtonPrint_clicked();

    void printDocument(QPrinter *printer);

    void on_tableView_ProsmotrGraf_clicked(const QModelIndex &index);

private:
    Ui::ProsmotrGrafVzaimopos *ui;
    QSqlTableModel *ProsmotrGrafModel;
    ProsmotrGrafVzaimopos *ProsmotrGrafVzaimoposForm;
    AddGrafVzaimopos *AddGrafVzaimoposForm;
    QString GodStart;
    QString GodEnd;
    QTextDocument *body;
};

#endif // PROSMOTRGRAFVZAIMOPOS_H
