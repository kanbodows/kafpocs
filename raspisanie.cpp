#include "raspisanie.h"
#include "ui_raspisanie.h"

bool Raspisanie::isOpen = false;
Raspisanie::Raspisanie(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::Raspisanie)
{
    ui->setupUi(this);
    if (!isOpen)
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
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    days << "Понедельник" << "Вторник" << "Среда" << "Четверг" << "Пятница" << "Суббота";
    vr << "8.00-9.20" << "9.30-10.50" << "11.00-12.20" << "12.40-14.00" << "14.10-15.30" << "15.40-17.00" << "17.10-18.30";
    createRaspisanie();
    ui->label->setVisible(false);
    ui->comboBox_den->setVisible(false);
}

Raspisanie::~Raspisanie()
{
    this->isOpen = false;
    delete ui;
}

void Raspisanie::createRaspisanie()
{
    if ( ui->scrollAreaWidgetContents->layout() != NULL ) // очистка кнопок
    {
        QLayoutItem* item;
        while ((item = ui->scrollAreaWidgetContents->layout()->takeAt(0) ) != NULL )
        {
            delete item->widget();
            delete item;
        }
        delete ui->scrollAreaWidgetContents->layout();
    }
    QSqlQuery *query = new QSqlQuery();
    query->prepare("SELECT * FROM sotr_kafedri");
    query->exec();
    query->first();
    QGridLayout *gridLayout = new QGridLayout(ui->scrollAreaWidgetContents);
    gridLayout->setContentsMargins(0,0,0,0);
    ui->scrollAreaWidgetContents->setLayout(gridLayout);
    ui->scrollAreaWidgetContents->setContentsMargins(0,0,0,0);
    ui->scrollArea->setContentsMargins(0,0,0,0);
    QLabel *sotrBtn1 = new QLabel("ФИО\nсотрудника");
    sotrBtn1->setStyleSheet("font-size:18px; qproperty-alignment: AlignCenter;");
    gridLayout->addWidget(sotrBtn1, 0, 0);
    for (int j = 0; j < days.count(); j++)
    {
        QGroupBox *box  = new QGroupBox();
        box->setStyleSheet("QGroupBox{border:red 3px solid; background-color:rgb(162, 166, 147);}");
        QGridLayout* boxl = new QGridLayout();
        boxl->setContentsMargins(0,0,0,0);
        box->setLayout(boxl);
        QLabel* dayLb = new QLabel(days.at(j));
        dayLb->setStyleSheet("font-size:25px; qproperty-alignment: AlignCenter; background-color: transparent;");
        boxl->addWidget(dayLb, 0, 0, 1, 7);
        for (int k = 0; k < vr.count(); k++)
        {
            QLabel *vrLabel = new QLabel(vr.at(k));
            vrLabel->setStyleSheet("font-size:15px; qproperty-alignment: AlignCenter; background-color: transparent;");
            boxl->addWidget(vrLabel, 1, k);
        }
        gridLayout->addWidget(box, 0, j+1);
    }
    for (int i = 1; i < query->size(); i++)
    {
        QPushButton *sotrBtn = new QPushButton(query->value(1).toString().replace("(", "\n("));
        sotrBtn->setMinimumSize(250,70);
        sotrBtn->setObjectName(query->value(0).toString());
        sotrBtn->setStyleSheet("QPushButton{font-size:18px; font: bold; background-color: rgb(81, 177, 79); background-color: qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, stop:0.482955 rgba(39, 74, 157, 255), stop:1 rgba(126, 154, 255, 255)); color:white;}"
                               "QPushButton:hover{background-color: qlineargradient(spread:pad, x1:1, y1:1, x2:1, y2:0, stop:0 rgba(70, 3, 18, 255), stop:0.75 rgba(255, 15, 67, 255), stop:1 rgba(255, 1, 5, 255));}");
        gridLayout->addWidget(sotrBtn, i, 0);
        QSqlQuery *queryRasp = new QSqlQuery;
        queryRasp = dal_prepodcontrol->getCurrentRaspisanie(query->value(0).toInt());
        connect(sotrBtn, SIGNAL(clicked()), SLOT(on_sotr_clicked()));
        if(queryRasp->size()<=0)
        {
            query->next();
            continue;
        }
        for (int j = 0; j < days.count(); j++)
        {
            QGroupBox *box  = new QGroupBox();
            box->setMaximumHeight(70);
            QGridLayout* boxl = new QGridLayout();
            boxl->setContentsMargins(0,0,10,0);
            box->setLayout(boxl);
            for (int j = 0; j < vr.count(); j++)
            {
                QPlainTextEdit *label = new QPlainTextEdit(queryRasp->value(4).toString());
                label->setMaximumSize(120,70);
                label->setMinimumSize(120,70);
                label->setStyleSheet("font-size:12px; background:transparent;");
                label->setReadOnly(true);
                boxl->addWidget(label, 0, j);
                queryRasp->next();
            }
            gridLayout->addWidget(box, i, j+1);
        }
        query->next();
    }
}
void Raspisanie::on_sotr_clicked()
{
    int id = ((QPushButton*)sender())->objectName().toInt();
    addoredir_raspisanie* form = new addoredir_raspisanie(this, id);
    form->exec();
    createRaspisanie();
}

void Raspisanie::on_pushButton_print_clicked()
{
    ui->label->setVisible(true);
    ui->comboBox_den->setVisible(true);
}

void Raspisanie::on_comboBox_den_currentIndexChanged(int index)
{
    if(ui->comboBox_den->currentIndex()==-1)
    {
        return;
    }
    body = "";
    body.append("<table border=\"1\"><tr><td rowspan=\"2\" align=\"center\" valign=\"middle\">ФИО сотрудника</td>");
    body.append("<td colspan=\"7\" align=\"center\"><H2>" + ui->comboBox_den->currentText() + "</H2></td>");
    body.append("</tr><tr>");
    for (int k = 0; k < vr.count(); k++)
        body.append("<td><b>" + vr.at(k) + "</b></td>");
    body.append("</tr>");
    QSqlQuery *query = new QSqlQuery("SELECT * FROM sotr_kafedri"); // получаем всех рабочих сотрудников
    query->exec();
    query->first();
    for (int i = 1; i < query->size(); i++)
    {
        body.append("<tr>");
        body.append("<td align=\"center\" valign = \"middle\"><H2>" + query->value(1).toString() + "</H2><p align=\"center\" valign = \"middle\">\n" + query->value(5).toString() + "</p></td>");// имя сотрудника и должность
        QSqlQuery *queryRasp = new QSqlQuery;
        queryRasp = dal_prepodcontrol->getCurrentRaspisanie(query->value(0).toInt());// получаем его расписание
        if(queryRasp->size()<=0)// не пусто ли
        {
            query->next();
            continue;
        }
        for (int k = 0; k < ui->comboBox_den->currentIndex(); k++) // переходим на выбранный день
        {
            queryRasp->next();queryRasp->next();queryRasp->next();queryRasp->next();queryRasp->next();queryRasp->next();queryRasp->next();
        }
        for (int j = 1; j < vr.count(); j++) // выводим дисциплины на весь выбранный день
        {
            body.append("<td>" + queryRasp->value(4).toString() + "</td>");
            queryRasp->next();
        }
        body.append("</tr>");
        query->next();
    }
    body.append("</table>");
//    QPrinter printer(QPrinter::HighResolution);
//    printer.setOrientation(QPrinter::Portrait);
//    printer.setPaperSize(QPrinter::A4);
//    printer.setFullPage(true);
    QPrinter printer(QPrinter::HighResolution);
    printer.setOutputFormat(QPrinter::PdfFormat);
    printer.setOutputFileName("file.pdf");
    QPrintPreviewDialog  *preview = new QPrintPreviewDialog (&printer);
    preview->setAttribute(Qt::WA_DeleteOnClose);
    preview->setWindowTitle("Печать");
    preview->setWindowFlags(Qt::Window);
    connect(preview, SIGNAL(paintRequested(QPrinter *)), SLOT(printDocument(QPrinter *)));
    preview->exec();
    ui->label->setVisible(false);
    ui->comboBox_den->setVisible(false);
    ui->comboBox_den->setCurrentIndex(-1);
}

void Raspisanie::printDocument(QPrinter *printer)
{
    QTextEdit *ted = new QTextEdit;
    ted->insertHtml(body);
    ted->document()->print(printer);
}
