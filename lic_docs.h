#ifndef LIC_DOCS_H
#define LIC_DOCS_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include "dals/dal_main.h"
#include <QString>
extern DAL_main  *dal_main;
namespace Ui {
class Lic_docs;
}

class Lic_docs : public QDialog
{
    Q_OBJECT

public:
    explicit Lic_docs(QWidget *parent = 0);
    ~Lic_docs();
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

    void on_pushButton_search_clicked();

    void on_comboBox_type_lic_activated(int index);

    void on_comboBox_type_doc_activated(int index);

    void on_dateEdit_start_dateChanged(const QDate &date);

    void on_dateEdit_end_dateChanged(const QDate &date);

    void on_pushButton_edit_clicked();

private:
    Ui::Lic_docs *ui;
    QSqlRelationalTableModel *lic_docsModel;
    QSqlTableModel *type_licModel;
    QSqlTableModel *type_docModel;
    int count;
    int type_lic;
    int type_doc;
    QDate dateBeginning;
    QDate dateEnd;
    QString naideno;
    bool isDate;
public slots:
    void formingSearch();
};

#endif // Lic_docs_H
