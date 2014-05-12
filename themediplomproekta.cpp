#include "themediplomproekta.h"
#include "ui_themediplomproekta.h"
#include <QMessageBox>
#include "delegates/textnndelegate.h"
//#include "delegates/word_wrap_delegate.h"

bool ThemeDiplomProekta::isOpen = false;
ThemeDiplomProekta::ThemeDiplomProekta(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::ThemeDiplomProekta)
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
    ThemeCourseWorkModel = new QSqlTableModel(this);
    ThemeCourseWorkModel->setTable("is_diplom_themes");
    ThemeCourseWorkModel->setHeaderData(1,Qt::Horizontal,tr("Тема на русском"));
    ThemeCourseWorkModel->setHeaderData(2,Qt::Horizontal,tr("Тема на кыргызском"));
    ThemeCourseWorkModel->setHeaderData(3,Qt::Horizontal,tr("Номер приказа"));
    ThemeCourseWorkModel->select();
    ui->tableView_Theme_Dipl->setModel(ThemeCourseWorkModel);
    ui->tableView_Theme_Dipl->setColumnHidden(0,true);
    ui->tableView_Theme_Dipl->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    ui->tableView_Theme_Dipl->horizontalHeader()->setStretchLastSection(true);
    ui->tableView_Theme_Dipl->verticalHeader()->hide();
    ui->tableView_Theme_Dipl->verticalHeader()->setSectionResizeMode(QHeaderView::ResizeToContents);

    ui->groupBox_search->setVisible(false);
    ui->label_result->setVisible(false);
    this->id_diplomTheme=0;
}
ThemeDiplomProekta::~ThemeDiplomProekta()
{
    delete ui;
}

void ThemeDiplomProekta::on_pushButton_add_clicked()
{
    ThemeCourseWorkModel->insertRow(ThemeCourseWorkModel->rowCount());
    ui->tableView_Theme_Dipl->edit(ThemeCourseWorkModel->index(ThemeCourseWorkModel->rowCount() - 1, 1));
    ui->tableView_Theme_Dipl->setCurrentIndex(ThemeCourseWorkModel->index(ThemeCourseWorkModel->rowCount()-1,1));
}

void ThemeDiplomProekta::on_pushButton_save_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    for(int i=0;i<ui->tableView_Theme_Dipl->model()->rowCount(); i++)
    {
        if(ui->tableView_Theme_Dipl->model()->index(i,1).data().isNull() || ui->tableView_Theme_Dipl->model()->index(i,2).data().isNull()||ui->tableView_Theme_Dipl->model()->index(i,3).data().isNull())
        {
            QMessageBox::information(this, tr("Информация"),tr("Поля 'Тема на русском','Тема на кыргызском' и 'Номер приказа' должны быть заполнены"));
            return;
        }

    }
    ThemeCourseWorkModel->submitAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные успешно сохранены"));
}

void ThemeDiplomProekta::on_pushButton_cancel_clicked()
{
    ThemeCourseWorkModel->revertAll();
    QMessageBox::information(this, tr("Информация"), tr("Данные не сохранены"));
}

void ThemeDiplomProekta::on_pushButton_update_clicked()
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    ThemeCourseWorkModel->select();
}

void ThemeDiplomProekta::on_pushButton_search_clicked()
{
    if (ui->groupBox_search->isVisible())
    {

        // ui->pushButton_search->setStyleSheet(css->currentStyle);
        ui->groupBox_search->setVisible(false);
        this->id_diplomTheme = 0;
        ui->lineEdit_nomer_prikaza->clear();
        ui->lineEdit_themeRus->clear();
        ui->lineEdit_Theme_kg->clear();
        ThemeCourseWorkModel->setFilter("");
        ui->label_naideno->setVisible(false);
        ui->label_result->setVisible(false);
        on_pushButton_update_clicked();

    }
    else
    {
        if (! dal_main->checkConnection())
        {
            QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
            return;
        }
        ui->groupBox_search->setVisible(true);
        //  ui->pushButton_search->setStyleSheet(css->currentPressedStyle);
        ThemeCourseWorkModel->select();
        this->id_diplomTheme = 0;
    }
}

void ThemeDiplomProekta::on_pushButton_find_clicked()
{
    this->naideno.clear();
    ui->label_naideno->setVisible(true);
    this->nomer_prikaza = ui->lineEdit_nomer_prikaza->text();
    this->tema_kg = ui->lineEdit_Theme_kg->text();
    this->tema_rus = ui->lineEdit_themeRus->text();
    this->formingSearch();
}

void ThemeDiplomProekta::formingSearch()
{
    QString condition = "";

    if (! ui->lineEdit_nomer_prikaza->text().isEmpty())
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " nomer_prikaza like '%" + ui->lineEdit_nomer_prikaza->text() + "%'";
    }

    if (! ui->lineEdit_themeRus->text().isEmpty())
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " tema_rus like '%" + ui->lineEdit_themeRus->text() + "%'";
    }

    if (! ui->lineEdit_Theme_kg->text().isEmpty())
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition = condition + " tema_kr like '%" + ui->lineEdit_Theme_kg->text() + "%'";
    }


    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    ThemeCourseWorkModel->setFilter(condition);
    if(ui->tableView_Theme_Dipl->model()->rowCount()>0)
    {
        this->naideno.append("    Найдено: " + QString::number(ui->tableView_Theme_Dipl->model()->rowCount()));
        ui->label_result->setVisible(false);
        ui->label_naideno->setText(this->naideno);
        this->naideno.clear();
    }
    else
    {
        this->naideno.append("    Найдено: 0");
        ui->label_result->setVisible(true);
        ui->label_naideno->setText(this->naideno);
        this->naideno.clear();

    }
}

void ThemeDiplomProekta::on_pushButton_clear_clicked()
{
    this->id_diplomTheme = 0;
    ui->lineEdit_nomer_prikaza->clear();
    ui->lineEdit_themeRus->clear();
    ui->lineEdit_Theme_kg->clear();
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return;
    }
    ui->label_result->setVisible(false);
    ui->label_naideno->setVisible(false);
    ThemeCourseWorkModel->setFilter("");
    ThemeCourseWorkModel->select();
}

void ThemeDiplomProekta::on_pushButton_dell_clicked()
{
    if (!ui->tableView_Theme_Dipl->model()->index(ui->tableView_Theme_Dipl->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка удаления"), tr("Ни одной записи не выбрано"));
        return;
    }
    if (this->checkRelation(ui->tableView_Theme_Dipl->model()->index(ui->tableView_Theme_Dipl->currentIndex().row(), 0).data().toString()))
    {
        QMessageBox::critical(this, tr("Ошибка удаления"), tr("Невозможно удалить запись, \n так как она используется в других таблицах"));
        return;
    }
    else
    {
        if (QMessageBox::warning(this, tr("Удаление записи"), tr("Вы уверены, что хотите удалить запись? \n Восстановить запись невозможно"),
                                 QMessageBox::Yes | QMessageBox::No) == QMessageBox::Yes)
        {
            if (! dal_main->checkConnection())
            {
                QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
                return;
            }
            ThemeCourseWorkModel->removeRow(ui->tableView_Theme_Dipl->currentIndex().row());
            ThemeCourseWorkModel->submitAll();
            ThemeCourseWorkModel->select();
        }
        else
        {
            return;
        }
    }
}

bool ThemeDiplomProekta::checkRelation(QString id_diplomTheme)
{
    if (! dal_main->checkConnection())
    {
        QMessageBox::warning(this, tr("Ошибка подключения"), tr("Соединение не установлено"));
        return NULL;
    }
    QSqlQuery query;
    query.prepare("select tema from is_diploms where tema = :id_diplomTheme");
    query.bindValue(":id_diplomTheme", id_diplomTheme);
    query.exec();
    query.next();

    if (query.isValid())
    {
        return true;
    }
    else
    {
        return false;
    }
}

void ThemeDiplomProekta::on_pushButton_edit_clicked()
{
    if (!ui->tableView_Theme_Dipl->model()->index(ui->tableView_Theme_Dipl->currentIndex().row(), 0).data().isValid())
    {
        QMessageBox::warning(this, tr("Ошибка редактирования"), tr("Ни одной записи не выбрано"));
        return;
    }
    ui->tableView_Theme_Dipl->edit(ui->tableView_Theme_Dipl->currentIndex());
}
