#ifndef PROEKTY_H
#define PROEKTY_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include "dals/dal_main.h"
#include <QString>
extern DAL_main  *dal_main;
namespace Ui {
class Proekty;
}

class Proekty : public QDialog
{
    Q_OBJECT

public:
    explicit Proekty(QWidget *parent = 0);
    ~Proekty();
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

    void on_comboBox_sotr_activated(int index);

    void on_pushButton_search_clicked();

    void on_pushButton_edit_clicked();

    void on_tableView_clicked(const QModelIndex &index);

private:
    Ui::Proekty *ui;
    QSqlRelationalTableModel *proektyModel;
    QSqlTableModel *sotrudnikiModel;
    int sotrudnik_id;
public slots:
    void formingSearch();
};

#endif // PROEKTY_H
