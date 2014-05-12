#ifndef THEMEDIPLOMPROEKTA_H
#define THEMEDIPLOMPROEKTA_H

#include <QDialog>
#include <dals/dal_main.h>
#include <QString>
extern DAL_main  *dal_main;

namespace Ui {
class ThemeDiplomProekta;
}

class ThemeDiplomProekta : public QDialog
{
    Q_OBJECT

public:
    explicit ThemeDiplomProekta(QWidget *parent = 0);
    ~ThemeDiplomProekta();
    bool static isOpen;
    bool checkRelation(QString id_dipl_tema);

private slots:
    void on_pushButton_add_clicked();

    void on_pushButton_save_clicked();

    void on_pushButton_cancel_clicked();

    void on_pushButton_update_clicked();

    void on_pushButton_search_clicked();

    void on_pushButton_find_clicked();

    void on_pushButton_clear_clicked();

    void on_pushButton_dell_clicked();

    void on_pushButton_edit_clicked();

private:
    Ui::ThemeDiplomProekta *ui;
    QSqlTableModel *ThemeCourseWorkModel;
    int id_diplomTheme;
    QString naideno;
    QString nomer_prikaza;
    QString tema_rus;
    QString tema_kg;

public slots:
    void formingSearch();
};

#endif // THEMEDIPLOMPROEKTA_H
