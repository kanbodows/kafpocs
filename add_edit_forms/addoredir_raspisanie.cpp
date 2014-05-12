#include "addoredir_raspisanie.h"
#include "ui_addoredir_raspisanie.h"

addoredir_raspisanie::addoredir_raspisanie(QWidget *parent, int id_sotr) :
    QDialog(parent),
    isEdit(isEdit),
    id_sotr(id_sotr),
    ui(new Ui::addoredir_raspisanie)
{
    ui->setupUi(this);
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        throw std::exception();
    }
    dal_prepodcontrol = new Dal_prepodcontrol(this);
    ui->comboBox_sotr->setModel(dal_prepodcontrol->getcomboPrepod());
    ui->comboBox_sotr->setModelColumn(1);
    ui->comboBox_sotr->setCurrentIndex(-1);
    days << "Понедельник" << "Вторник" << "Среда" << "Четверг" << "Пятница" << "Суббота";


    QSqlQuery *query = new QSqlQuery;
    query = dal_prepodcontrol->getCurrentRaspisanie(this->id_sotr);

    for (int j = 0; j < ui->comboBox_sotr->count(); j++)
    {
        if (ui->comboBox_sotr->model()->index(j, 0).data().toInt() == id_sotr)
        {
            ui->comboBox_sotr->setCurrentIndex(j);
            ui->comboBox_sotr->view()->setCurrentIndex(ui->comboBox_sotr->model()->index(j, 0));
            break;
        }
    }
    // здесь сработает метод on_comboBox_sotr_currentIndexChanged(int index) к-й подставит текущее расписание

    if(dal_main->getCurrentEmployee() != id_sotr)
    {
        this->setWindowTitle("Просмотр расписания");
        ui->comboBox_sotr->setEnabled(false);
        ui->pushButton_save->setVisible(false);
        ui->tableWidget_rasp->setEnabled(false);
    }
    ui->tableWidget_rasp->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableWidget_rasp->horizontalHeader()->setStretchLastSection(true);
    ui->tableWidget_rasp->verticalHeader()->hide();
    ui->tableWidget_rasp->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);
}

addoredir_raspisanie::~addoredir_raspisanie()
{
    delete ui;
}

void addoredir_raspisanie::on_pushButton_save_clicked()
{
    if (ui->comboBox_sotr->currentIndex() ==-1)
    {
        QMessageBox::warning(this, tr("Ошибка"), tr("Выберите сотрудника"));
        return;
    }
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка соединения"), tr("Соединение не установлено"));
        return;
    }
    QSqlQuery* insertQuery = new QSqlQuery();
    insertQuery->prepare("INSERT INTO is_raspisanie(sotr_id, den_nedeli, vremya, predmet) VALUES (:sotr_id, :den_nedeli, :vremya, :predmet)");
    insertQuery->bindValue(":sotr_id", ui->comboBox_sotr->model()->index(ui->comboBox_sotr->currentIndex(),0).data().toInt());

    if (!this->isEdit)
    {
        for (int den = 0; den < 6; den++)
        {
            for (int i = 0; i < 7; i++)
            {
                QString vr = ui->tableWidget_rasp->item(i, 0)->text();
                QString pr = ui->tableWidget_rasp->item(i, den+1)->text();
                if(pr.isEmpty())
                    pr = "-";
                insertQuery->bindValue(":den_nedeli", days.at(den));
                insertQuery->bindValue(":vremya", vr);
                insertQuery->bindValue(":predmet", pr);
                insertQuery->exec();
            }
        }
        QMessageBox::information(this, tr("Информация"), tr("Запись успешно добавлена"));
    }
    else
    {
        QSqlQuery* deleteQuery = new QSqlQuery;
        int id = ui->comboBox_sotr->model()->index(ui->comboBox_sotr->currentIndex(),0).data().toInt();
        deleteQuery->prepare("DELETE FROM is_raspisanie WHERE sotr_id = " + QString::number(id));
        deleteQuery->exec();
        qDebug()<<ui->comboBox_sotr->model()->index(ui->comboBox_sotr->currentIndex(),0).data().toInt()<<deleteQuery->executedQuery()<<deleteQuery->lastError();
        for (int den = 0; den < 6; den++)
        {
            for (int i = 0; i < 7; i++)
            {
                QString vr = ui->tableWidget_rasp->item(i, 0)->text();
                QString pr = ui->tableWidget_rasp->item(i, den+1)->text();
                if(pr.isEmpty())
                    pr = "-";
                insertQuery->bindValue(":den_nedeli", days.at(den));
                insertQuery->bindValue(":vremya", vr);
                insertQuery->bindValue(":predmet", pr);
                insertQuery->exec();
            }
        }
        QMessageBox::information(this, tr("Информация"), tr("Данные успешно отредактированы"));

    }
    this->close();
}

void addoredir_raspisanie::on_pushButton_cancel_clicked()
{
    this->close();
}

void addoredir_raspisanie::on_pushButton_print_clicked()
{
    if(ui->comboBox_sotr->currentIndex()==-1)
    {
        return;
    }
    body = "";
    body += "<H2>Расписание " + ui->comboBox_sotr->currentText() + "</H2>";
    body += "<table border=\"1\">";
    body += "<tr><td>Время</td>";
    for (int k = 0; k < days.count(); k++)
        body += "<td align=\"center\" valign = \"middle\" style=\"padding:3px;\"><b>" + days.at(k) + "</b></td>";
    body += "</tr>";

    for (int vr = 1; vr < 7; vr++)
    {
        body += "<tr>";
        body += "<td align=\"center\" valign = \"middle\" style=\"padding:3px;\"><b>" + ui->tableWidget_rasp->item(vr, 0)->text() + "</b></td>";// время
        for (int den = 1; den < 7; den++)
        {
            body += "<td style=\"padding:3px;\">" + ui->tableWidget_rasp->item(vr, den)->text() + "</td>";
        }
        body += "</tr>";
    }
    body += "</table>";
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
}

void addoredir_raspisanie::printDocument(QPrinter *printer)
{
    QTextEdit *ted = new QTextEdit;
    ted->insertHtml(body);
    ted->document()->print(printer);
}

void addoredir_raspisanie::on_comboBox_sotr_currentIndexChanged(int index)
{
    this->id_sotr = ui->comboBox_sotr->model()->index(ui->comboBox_sotr->currentIndex(), 0).data().toInt();
    QSqlQuery *query = new QSqlQuery;
    query = dal_prepodcontrol->getCurrentRaspisanie(this->id_sotr);
    for (int den = 0; den < 6; den++)
    {
        for (int i = 0; i < 7; i++)
        {
            QString pr = query->value(4).toString();
            ui->tableWidget_rasp->setItem(i, den+1, new QTableWidgetItem(pr));
            query->next();
        }
    }
}
