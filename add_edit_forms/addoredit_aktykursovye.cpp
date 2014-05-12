#include "addoredit_aktykursovye.h"
#include "ui_addoredit_aktykursovye.h"
#include "dals/dal_studentscontrol.h"
#include "delegates/checkboxdelegate.h"
#include <QMessageBox>
#include "delegates/readonlydelegate.h"

AddOrEdit_AktyKursovye::AddOrEdit_AktyKursovye(QWidget *parent, bool isEdit, int id_vedomostiKurs) :
    QDialog(parent),
    isEdit(isEdit),
    id_vedomostiKurs(id_vedomostiKurs),
    ui(new Ui::AddOrEdit_AktyKursovye)
{
    ui->setupUi(this);
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    dal_studentsControl = new Dal_studentsControl(this);
    css = new Styles;
    toPrint = new QTextDocument;
    ui->tableView->setModel(dal_studentsControl->getCourseWorks(0, 0, "", id_vedomostiKurs));
    ui->tableView->setColumnHidden(0, true);
    ui->tableView->setColumnHidden(1, true);
    ui->tableView->setColumnHidden(2, true);
    ui->tableView->setColumnHidden(3, true);
    ui->tableView->setColumnHidden(4, true);
    ui->tableView->setColumnHidden(5, true);
    ui->tableView->setColumnHidden(6, true);
    ui->tableView->setColumnHidden(10, true);
    ui->tableView->setColumnHidden(14, true);
    ui->tableView->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView->verticalHeader()->hide();
    ui->tableView->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
    ui->pushButton_print->setEnabled(false);
    ui->label_kurator->setText("");
    ui->dateEdit_dataspis->setDate(QDate::currentDate());
    QSqlQuery *query = new QSqlQuery;
    query = dal_studentsControl->getCurrentVedomostiKursovye(this->id_vedomostiKurs);
    ui->label_spec->setText(query->value(4).toString());
    ui->label_gruppa->setText(query->value(5).toString());
    ui->label_discip->setText(query->value(6).toString());
    ui->label_spec->setText(query->value(4).toString());
    ui->dateEdit->setDate(query->value(9).toDate());
    ui->label_kolvo_rabot->setText(query->value(7).toString());
    ui->label_kolvo->setText(query->value(8).toString());
    ui->label_kurator->setText(ui->tableView->model()->index(0, 8).data().toString());
    ui->comboBox_2->setModel(dal_studentsControl->getcomborucPrac());
    ui->comboBox_2->setModelColumn(1);
    ui->comboBox_2->setCurrentIndex(-1);
    ui->comboBox_3->setModel(dal_studentsControl->getcomborucPrac());
    ui->comboBox_3->setModelColumn(1);
    ui->comboBox_3->setCurrentIndex(-1);
}

AddOrEdit_AktyKursovye::~AddOrEdit_AktyKursovye()
{
    delete ui;
}

void AddOrEdit_AktyKursovye::on_pushButton_ok_clicked()
{
    if(ui->lineEdit_1->text().isEmpty() || ui->comboBox_2->currentIndex() ==-1 || ui->comboBox_3->currentIndex() == -1 || ui->comboBox_2->currentIndex()==ui->comboBox_3->currentIndex())
    {
        QMessageBox::warning(this, tr("Ошибка"), tr("Состав комиссии не корректно введен"));
        return;
    }
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return ;
    }

    if(dal_studentsControl->addAktSpisaniyaKurs(id_vedomostiKurs,
                                                ui->dateEdit_dataspis->date(),
                                                ui->lineEdit_1->text(),
                                                ui->comboBox_2->currentText(),
                                                ui->comboBox_3->currentText(),
                                                ui->lineEdit_primechanie->text()))
    {
        QMessageBox::information(this,tr("Информация"),tr("Данные были добавлены"));
        ui->pushButton_print->setEnabled(true);
        ui->pushButton_ok->setEnabled(false);
    }
    else
        QMessageBox::information(this,tr("Ошибка"),tr("Курсовые не списаны"));
}

void AddOrEdit_AktyKursovye::on_pushButton_cancel_clicked()
{
    this->close();
}

void AddOrEdit_AktyKursovye::printDocument(QPrinter *printer)
{
    toPrint->print(printer);
}

void AddOrEdit_AktyKursovye::on_pushButton_print_clicked()
{
    QString body;
    body.append("<html><head>"
                "<link rel='stylesheet' type='text/css' href='format.css'>"
                "</head><body>"
                "<center><H3>Акт</H3></center><hr><table width='100%'>"
                "<tr><td colspan='2'>списания курсовых работ (проектов) по истечении годичного срока хранения_________</p></td></tr>"
                "<tr><td colspan='2'>студентов группы <b>" + ui->label_gruppa->text() + "</b>, специальности <b>" + ui->label_spec->text() + "</b>, факультета <b>ФИТ</b></td></tr>"
                "<tr><td colspan='2'>дисциплина <b>" + ui->label_discip->text() + "</b></td></tr>"
                "<tr><td>Комиссия в составе:</td><td></td></tr>"
                "<tr><td></td><td>1. " + ui->lineEdit_1->text() + "</td></tr>"
                "<tr><td></td><td>2. " + ui->comboBox_2->currentText() + "</td></tr>"
                "<tr><td></td><td>3. " + ui->comboBox_3->currentText() + "</td></tr>"
                "<tr><td colspan='2'>комиссия потверждает наличие <b>" + ui->label_kolvo_rabot->text() + "(ти)</b> курсовых работ (проектов) в количестве <b>" + ui->label_kolvo->text() + " (ти)</b> листов</td></tr>"
                "<tr><td colspan='2'>и подвергла их уничтожению за исключением <b>" + ui->lineEdit_primechanie->text() + "</b></td></tr>"
                "<tr><td></td><td>Дата списания: <b>" + ui->dateEdit_dataspis->date().toString("dd-MM-yyyy") + "</b></td></tr>"
                "<tr><td>Подпись комиссии:</td><td></td></tr>"
                "<tr><td></td><td>1. " + ui->lineEdit_1->text() + "_________________________</td></tr>"
                "<tr><td></td><td>2. " + ui->comboBox_2->currentText() + "_________________________</td></tr>"
                "<tr><td></td><td>3. " + ui->comboBox_3->currentText() + "_________________________</td></tr>"
                "</table></body></html>");
    toPrint->addResource(QTextDocument::StyleSheetResource, QUrl( "format.css" ), css->reportCss);
    toPrint->setHtml(body);
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("file.pdf");
    QPrintPreviewDialog *preview = new QPrintPreviewDialog (&printer);
    preview->setAttribute(Qt::WA_DeleteOnClose);
    preview->setWindowFlags(Qt::Window);
    QList<QPrintPreviewWidget*> list2 = preview->findChildren<QPrintPreviewWidget*>();
    if(!list2.isEmpty()) // paranoiac safety check
        list2.first()->setZoomMode(QPrintPreviewWidget::FitToWidth);
    connect(preview, SIGNAL(paintRequested(QPrinter *)), SLOT(printDocument(QPrinter *)));
    preview->exec();
}

