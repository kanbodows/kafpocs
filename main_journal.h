#ifndef MAIN_JOURNAL_H
#define MAIN_JOURNAL_H

#include <QDialog>
#include <QSqlRelationalTableModel>
#include "dals/dal_main.h"
#include <QString>
extern DAL_main  *dal_main;
namespace Ui {
class Main_journal;
}

class Main_journal : public QDialog
{
    Q_OBJECT

public:
    explicit Main_journal(QWidget *parent = 0);
    ~Main_journal();
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

    void on_comboBox_student_activated(int index);

    void on_comboBox_discipline_activated(int index);

private:
    Ui::Main_journal *ui;
    QSqlRelationalTableModel *main_journalModel;
    QSqlTableModel *studentModel;
    QSqlTableModel *disciplineModel;
    int count;
    int student;
    int discipline;
    QString naideno;
public slots:
    void formingSearch();
};

#endif // Main_journal_H
