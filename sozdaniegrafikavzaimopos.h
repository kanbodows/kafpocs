#ifndef SOZDANIEGRAFIKAVZAIMOPOS_H
#define SOZDANIEGRAFIKAVZAIMOPOS_H

#include <QDialog>

namespace Ui {
class SozdanieGrafikaVzaimopos;
}

class SozdanieGrafikaVzaimopos : public QDialog
{
    Q_OBJECT

public:
    explicit SozdanieGrafikaVzaimopos(QWidget *parent = 0);
    ~SozdanieGrafikaVzaimopos();

private:
    Ui::SozdanieGrafikaVzaimopos *ui;
};

#endif // SOZDANIEGRAFIKAVZAIMOPOS_H
