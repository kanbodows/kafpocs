#include "sozdaniegrafikavzaimopos.h"
#include "ui_sozdaniegrafikavzaimopos.h"

SozdanieGrafikaVzaimopos::SozdanieGrafikaVzaimopos(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SozdanieGrafikaVzaimopos)
{
    ui->setupUi(this);
}

SozdanieGrafikaVzaimopos::~SozdanieGrafikaVzaimopos()
{
    delete ui;
}
