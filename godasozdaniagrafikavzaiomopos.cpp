#include "godasozdaniagrafikavzaiomopos.h"
#include "ui_godasozdaniagrafikavzaiomopos.h"
#include <QMessageBox>
bool GodaSozdaniaGrafikaVzaiomopos::isOpen = false;
GodaSozdaniaGrafikaVzaiomopos::GodaSozdaniaGrafikaVzaiomopos(QWidget *parent, QString GodSS, QString GodPo) :
    QDialog(parent),
    ui(new Ui::GodaSozdaniaGrafikaVzaiomopos)
{
    ui->setupUi(this);
    if (! isOpen)
    {
        isOpen = true;
    }
    else
    {
        QMessageBox::warning(this, tr("Ошибка"), tr("Окно уже открыто"));
        throw std::exception();
    }
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        this->isOpen = false;
        throw std::exception();
    }
    GodSozdGrafModel = new QSqlTableModel(this);
    GodSozdGrafModel->setTable("is_GodSozdGrafika");
    GodSozdGrafModel->setHeaderData(1,Qt::Horizontal,tr("Год создания графика взаимопосещения"));

    GodSozdGrafModel->select();

    ui->tableViewGodSozdGrafika->setModel(GodSozdGrafModel);
    ui->tableViewGodSozdGrafika->setColumnHidden(0,true);
    ui->tableViewGodSozdGrafika->setColumnHidden(2,true);
    ui->tableViewGodSozdGrafika->setColumnHidden(3,true);
    ui->tableViewGodSozdGrafika->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableViewGodSozdGrafika->horizontalHeader()->setStretchLastSection(true);
}

GodaSozdaniaGrafikaVzaiomopos::~GodaSozdaniaGrafikaVzaiomopos()
{
    this->isOpen = false;
    delete ui;
}

void GodaSozdaniaGrafikaVzaiomopos::on_pushButton_clicked()
{
    try
    {
        AddGrafVzaimoposForm = new AddGrafVzaimopos(this);
        AddGrafVzaimoposForm->exec();
    }
    catch (...)
    {
        return;
    }
    GodSozdGrafModel->select();
}

void GodaSozdaniaGrafikaVzaiomopos::on_pushButton_ProsmotraGrfika_clicked()
{
    if(!ui->tableViewGodSozdGrafika->currentIndex().isValid())
    {
        QMessageBox::information(this, tr("Информация"), tr("Выберите год создания графика взаимопосещения"));
        return;
    }
    QString godS = ui->tableViewGodSozdGrafika->model()->index(ui->tableViewGodSozdGrafika->currentIndex().row(), 2).data().toString();
    QString godPo = ui->tableViewGodSozdGrafika->model()->index(ui->tableViewGodSozdGrafika->currentIndex().row(), 3).data().toString();
    try
    {
        ProsmotrGrafVzaimoposForm = new ProsmotrGrafVzaimopos(this,godS,godPo);
        ProsmotrGrafVzaimoposForm->exec();
        ui->tableViewGodSozdGrafika->selectionModel()->clear();
    }
    catch (...)
    {
        return;
    }
}
