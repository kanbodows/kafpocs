#ifndef MET_TYPES_H
#define MET_TYPES_H

#include <QDialog>
#include <dals/dal_main.h>
extern DAL_main  *dal_main;
namespace Ui {
class Met_types;
}

class Met_types : public QDialog
{
    Q_OBJECT

public:
    explicit Met_types(QWidget *parent = 0);
    ~Met_types();
    bool static isOpen;
    bool checkRelation(QString id_met_typ);

private slots:
    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_add_clicked();

    void on_pushButton_edit_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_del_clicked();

private:
    Ui::Met_types *ui;
    QSqlTableModel *met_typesModel;
};

#endif // Met_types_H
