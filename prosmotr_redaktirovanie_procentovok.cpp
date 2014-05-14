#include "prosmotr_redaktirovanie_procentovok.h"
#include "ui_prosmotr_redaktirovanie_procentovok.h"
#include <QMessageBox>
#include "delegates/word_wrap_delegate.h"

Prosmotr_redaktirovanie_procentovok::Prosmotr_redaktirovanie_procentovok(QWidget *parent, int id, bool prosmtr) :
    QDialog(parent),
    ui(new Ui::Prosmotr_redaktirovanie_procentovok)
{
    ui->setupUi(this);
    this->stud_ids = 0;
     dal_studentsControl = new Dal_studentsControl(this);
     ProcentkaListModels = dal_studentsControl->getProcentkaList(this->stud_ids);
     ui->tableView_procentovkiStudenta->setModel(ProcentkaListModels);
     ui->tableView_procentovkiStudenta->setColumnHidden(0,true);
     ui->tableView_procentovkiStudenta->setColumnHidden(1,true);
     ui->tableView_procentovkiStudenta->setColumnHidden(2,true);
     ui->tableView_procentovkiStudenta->setColumnHidden(14,true);

     ui->tableView_procentovkiStudenta->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
     ui->tableView_procentovkiStudenta->horizontalHeader()->setStretchLastSection(true);

     if(prosmtr == true)
     {
         this->setWindowTitle("Просмотр данных по процентовкам");
     }
     else
     {
     this->setWindowTitle("Редактирование процентовок");
     }
}

Prosmotr_redaktirovanie_procentovok::~Prosmotr_redaktirovanie_procentovok()
{
    delete ui;
}

void Prosmotr_redaktirovanie_procentovok::on_pushButtonClose_clicked()
{
    this->close();
}
