#ifndef LIC_DOC_TYPES_H
#define LIC_DOC_TYPES_H

#include <QDialog>
#include <dals/dal_main.h>
extern DAL_main  *dal_main;
namespace Ui {
class Lic_doc_types;
}

class Lic_doc_types : public QDialog
{
    Q_OBJECT

public:
    explicit Lic_doc_types(QWidget *parent = 0);
    ~Lic_doc_types();
    bool static isOpen;
    bool checkRelation(QString lic_doc_type_id);

private slots:
    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_edit_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_del_clicked();

private:
    Ui::Lic_doc_types *ui;
    QSqlTableModel *lic_doc_typesModel;
};

#endif // Lic_doc_types_H
