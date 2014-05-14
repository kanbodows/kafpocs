#include "dal_studentscontrol.h"
#include <QMessageBox>
#include <QDebug>
#include <QDialog>
Dal_studentsControl::Dal_studentsControl(QObject *parent)
{    
}

QSqlQueryModel *Dal_studentsControl::getKonf_sem(int gruppa, QString student, QString theme)
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    QString query = "SELECT * FROM konf_sem_view where 1=1 ";
    if(gruppa != 0)
        query.append(" and id_group = " + QString::number(gruppa));
    if(student != "")
        query.append(" and s_fio like '%" + student + "%' ");
    if(theme != "")
        query.append(" and theme like '%" + theme + "%' ");
    qDebug()<<query;
    model->setQuery(query);
    model->setHeaderData(1,Qt::Horizontal,tr("Группа"));
    model->setHeaderData(2,Qt::Horizontal,tr("Студент"));
    model->setHeaderData(3,Qt::Horizontal,tr("Тема"));
    model->setHeaderData(4,Qt::Horizontal,tr("Комментарии"));
    model->setHeaderData(5,Qt::Horizontal,tr("Место"));
    model->setHeaderData(6,Qt::Horizontal,tr("Сертификат"));
    model->setHeaderData(7,Qt::Horizontal,tr("Дата"));
    return model;
}

QSqlQueryModel *Dal_studentsControl::getProcentka(int ruk)
{
    QSqlQueryModel *procmodel = new QSqlQueryModel(this);
    QString query = "SELECT * FROM stud_dopush_k_procentovke_view where 1=1 ";
    if(ruk != 0)
        query.append(" and rukovoditel = " + QString::number(ruk));
    procmodel->setQuery(query);
    procmodel->setHeaderData(12,Qt::Horizontal,tr("ФИО студента"));
    return procmodel;
}

QSqlQueryModel *Dal_studentsControl::getProcentkaList(int id_stud)
{
    QSqlQueryModel *procListmodel = new QSqlQueryModel(this);
    QString query = "SELECT * FROM procentovochniilist_view where 1=1 ";
    if(id_stud != 0)
        query.append(" and stud_id = " + QString::number(id_stud));
    procListmodel->setQuery(query);
    procListmodel->setHeaderData(3,Qt::Horizontal,tr("Дата"));
    procListmodel->setHeaderData(4,Qt::Horizontal,tr("ФИО студента"));
    procListmodel->setHeaderData(5,Qt::Horizontal,tr("Введение"));
    procListmodel->setHeaderData(6,Qt::Horizontal,tr("Анализ\nтребований"));
    procListmodel->setHeaderData(7,Qt::Horizontal,tr("Констр-ий\nраздел"));
    procListmodel->setHeaderData(8,Qt::Horizontal,tr("Документирование"));
    procListmodel->setHeaderData(9,Qt::Horizontal,tr("Эксперементальный"));
    procListmodel->setHeaderData(10,Qt::Horizontal,tr("Экономический"));
    procListmodel->setHeaderData(11,Qt::Horizontal,tr("Заключение"));
    procListmodel->setHeaderData(12,Qt::Horizontal,tr("Работа ПП"));
    procListmodel->setHeaderData(13,Qt::Horizontal,tr("Презентация"));
    return procListmodel;
}

QSqlQueryModel *Dal_studentsControl::getGAK(int nomPrik)
QSqlQueryModel *Dal_studentsControl::getGAK(int nomPrik, int gak_id)
{
    QSqlQueryModel *GAKsmodel = new QSqlQueryModel(this);
    QString query = "SELECT * FROM GAKView where 1=1 ";
    if(nomPrik != 0)
        query.append(" and gak_nomer_prikaza = " + QString::number(nomPrik));
    if(gak_id != 0)
        query.append(" and gak_id = " + QString::number(gak_id));
    GAKsmodel->setQuery(query);
    GAKsmodel->setHeaderData(6,Qt::Horizontal,tr("Дисциплина №1"));
    GAKsmodel->setHeaderData(7,Qt::Horizontal,tr("Дисциплина №2"));
    GAKsmodel->setHeaderData(8,Qt::Horizontal,tr("Дисциплина №3"));
    GAKsmodel->setHeaderData(9,Qt::Horizontal,tr("Дисциплина №4"));
    GAKsmodel->setHeaderData(10,Qt::Horizontal,tr("Дата"));
    GAKsmodel->setHeaderData(11,Qt::Horizontal,tr("Кабинет"));
    GAKsmodel->setHeaderData(12,Qt::Horizontal,tr("Номер приказа"));
    GAKsmodel->setHeaderData(13,Qt::Horizontal,tr("Год\n формирования\n комиссии"));
    return GAKsmodel;
}

QSqlQuery *Dal_studentsControl::getCurrentKonf_sem(int id_konf)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT * FROM konf_sem_view WHERE id_konf = " + QString::number(id_konf));
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

QSqlQuery *Dal_studentsControl::getCurrentPracWork(int id_PracWork)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT * FROM PracWorkView WHERE id_practice = " + QString::number(id_PracWork));
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

QSqlQueryModel *Dal_studentsControl::getObchejitie(int gruppa, QString student)
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    QString query = "SELECT * FROM obchejitie_view where 1=1 ";
    if(gruppa != 0)
        query.append(" and id_group = " + QString::number(gruppa));
    if(student != "")
        query.append(" and s_fio like '%" + student + "%' ");
    qDebug()<<query;
    model->setQuery(query);
    model->setHeaderData(1,Qt::Horizontal,tr("Группа"));
    model->setHeaderData(2,Qt::Horizontal,tr("Студент"));
    model->setHeaderData(3,Qt::Horizontal,tr("Адрес"));
    model->setHeaderData(4,Qt::Horizontal,tr("Оплата"));
    model->setHeaderData(5,Qt::Horizontal,tr("Отзыв"));
    return model;
}

QSqlQuery *Dal_studentsControl::getCurrentObchejitie(int id_obch)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT * FROM obchejitie_view WHERE id_obch = " + QString::number(id_obch));
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

QSqlQuery *Dal_studentsControl::getCurrentVedomostiKursovye(int id_vedomosti)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT * FROM vedomostiKursovye_view WHERE id_vedomosti = " + QString::number(id_vedomosti));
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

QSqlQuery *Dal_studentsControl::getCurrentGAK(int gaks_id)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT * FROM GAKView WHERE gak_id = " + QString::number(gaks_id));
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

QSqlQuery *Dal_studentsControl::getCurrentStudent(int id_student)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT * FROM students_view WHERE id_student = " + QString::number(id_student));
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

QSqlQuery *Dal_studentsControl::CurrentBoleznStud(int id_bolezStud)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT * FROM BolezniStudView  WHERE id_stud_bolezni = " + QString::number(id_bolezStud));
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}



QSqlQueryModel *Dal_studentsControl::getOlimpiada(QString stud, QString pred, int ZanMesto)
{
    QString query="", condition="",filter="";
    QSqlQueryModel *OlimpiadaModelView = new QSqlQueryModel(this);
    if(stud!="" || pred!="" || ZanMesto!=0)
        filter.append(" where ");
    if(stud!="")
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition.append(" Olimpiada_view.`s_fio` like '%" + stud + "%'  ");
    }

    if(pred!="")
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition.append(" Olimpiada_view.`ol_subj` like '%" + pred + "%'  ");
    }

    if(ZanMesto!=0)
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition.append(" Olimpiada_view.`ol_mesto` = " + QString::number(ZanMesto));
    }

    query = "select * from Olimpiada_view " + filter + condition;
    OlimpiadaModelView->setQuery(query);
    OlimpiadaModelView->setHeaderData(3,Qt::Horizontal,tr("Группа"));
    OlimpiadaModelView->setHeaderData(4,Qt::Horizontal,tr("ФИО студента"));
    OlimpiadaModelView->setHeaderData(5,Qt::Horizontal,tr("Предмет"));
    OlimpiadaModelView->setHeaderData(6,Qt::Horizontal,tr("Дата"));
    OlimpiadaModelView->setHeaderData(7,Qt::Horizontal,tr("Комментарии"));
    OlimpiadaModelView->setHeaderData(8,Qt::Horizontal,tr("Занятое место"));
    OlimpiadaModelView->setHeaderData(9,Qt::Horizontal,tr("Место проведения"));
    OlimpiadaModelView->setHeaderData(10,Qt::Horizontal,tr("Проверяющие"));
    return OlimpiadaModelView;
}

QSqlQueryModel *Dal_studentsControl::getVedomostiKursovye(int group_id, int discipline_id , QDate dateBegin, QDate dateEnd)
{
    QString query = "SELECT * FROM vedomostiKursovye_view WHERE 1=1 ";
    QSqlQueryModel  *model = new QSqlQueryModel(this);
    if(group_id!=0)
        query.append(" AND group_id = " + QString::number(group_id));
    if(discipline_id!=0)
        query.append(" AND discipline_id = " + QString::number(discipline_id));
    if(!dateBegin.isNull())
        query.append(" AND data_zashity BETWEEN '" + dateBegin.toString("yyyy-MM-dd") + "' AND '" + dateEnd.toString("yyyy-MM-dd 23:59:59") + "'");
    model->setQuery(query);
    model->setHeaderData(4,Qt::Horizontal,tr("Специальность"));
    model->setHeaderData(5,Qt::Horizontal,tr("Группа"));
    model->setHeaderData(6,Qt::Horizontal,tr("Дисциплина"));
    model->setHeaderData(7,Qt::Horizontal,tr("Кол-во работ"));
    model->setHeaderData(8,Qt::Horizontal,tr("Кол-во страниц"));
    model->setHeaderData(9,Qt::Horizontal,tr("Дата защиты"));
    model->setHeaderData(10,Qt::Horizontal,tr("Списаны"));
    return model;
}

// добавление информации о ведомости
int Dal_studentsControl::addVedomostiKursovye(int group_id, int discipline_id, QDate data_zashity, int kolvo_rabot, int kolvo_listov)
{
    QSqlQuery q;
    q.prepare("INSERT INTO is_vedomostiKursovye (group_id, discipline_id, data_zashity, kolvo_rabot, kolvo_listov) VALUES (:group_id, :discipline_id, :data_zashity, :kolvo_rabot, :kolvo_listov)");
    q.bindValue(":group_id", group_id);
    q.bindValue(":discipline_id", discipline_id);
    q.bindValue(":data_zashity", data_zashity);
    q.bindValue(":kolvo_rabot", kolvo_rabot);
    q.bindValue(":kolvo_listov", kolvo_listov);
    q.exec();
    return q.lastInsertId().toInt();
}

bool Dal_studentsControl::addAktSpisaniyaKurs(int vedomostKurs_id, QDate data_spis, QString komissiya1, QString komissiya2, QString komissiya3, QString primechanie)
{
    QSqlQuery q1;
    q1.prepare("UPDATE is_vedomostiKursovye SET spisany = 1 WHERE id_vedomosti = :id_vedomosti");
    q1.bindValue(":id_vedomosti", vedomostKurs_id);
    q1.exec();
    QSqlQuery q;
    q.prepare("INSERT INTO is_akty_spisaniya_kurs (vedomostKurs_id, data_spis, komissiya1, komissiya2, komissiya3, primechanie) VALUES (:vedomostKurs_id, :data_spis, :komissiya1, :komissiya2, :komissiya3, :primechanie)");
    q.bindValue(":id_vedomosti", vedomostKurs_id);
    q.bindValue(":data_spis", data_spis);
    q.bindValue(":komissiya1", komissiya1);
    q.bindValue(":komissiya2", komissiya2);
    q.bindValue(":komissiya3", komissiya3);
    q.bindValue(":primechanie", primechanie);
    q.exec();
}

// добавление курсовых в ведомости
bool Dal_studentsControl::addKursovyeVedomosti(int vedomostiKursovye_id, QString primechanie, int id_course_work)
{
    QSqlQuery q;
    q.prepare("UPDATE is_course_work SET vedomostiKursovye_id = :vedomostiKursovye_id, primechanie = :primechanie WHERE id_course_work = :id_course_work");
    q.bindValue(":vedomostiKursovye_id", vedomostiKursovye_id);
    q.bindValue(":primechanie", primechanie);
    q.bindValue(":id_course_work", id_course_work);
    return q.exec();
}

bool Dal_studentsControl::addKonfSem(int student_id, QString theme, QString comments, int mesto, QString sert, QDate date)
{
    QSqlQuery q;
    q.prepare("INSERT INTO is_konf_sem (student, theme, comments, mesto, sert, date) VALUES (:student_id, :theme, :comments, :mesto, :sert, :date)");
    q.bindValue(":student_id", student_id);
    q.bindValue(":theme", theme);
    q.bindValue(":comments", comments);
    if(mesto!=0)
        q.bindValue(":mesto", mesto);
    q.bindValue(":sert", sert);
    q.bindValue(":date", date);
    return q.exec();
}

bool Dal_studentsControl::editKonfSem(int id_konf_sem, int student_id, QString theme, QString comments, int mesto, QString sert, QDate dates)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("call updKonfSem(:id_konf_sem, :student_id, :theme, :comments, :mesto, :sert, :dates, @result)");
    query->bindValue(":id_konf_sem", id_konf_sem);
    query->bindValue(":student_id", student_id);
    query->bindValue(":theme", theme);
    query->bindValue(":mesto", mesto);
    query->bindValue(":comments", comments);
    query->bindValue(":mesto", mesto);
    query->bindValue(":sert", sert);
    query->bindValue(":dates", dates);
    query->exec();
    query->exec(" select @result");
    query->first();
    int result = query->value(0).toInt();
    if(result == 0)
    {
        return true;

    }
    else
        return false;
}

bool Dal_studentsControl::addStudent(QString fio, int group_id, QDate dateBirth, QDate dateReg, QString pasport, QString tel, QString rodTel, QString email, QString rodEmail, QString adresProj, QString adresProp, QString shkola, int formaObuch_id, double skidka, int formaOplaty_id, bool otch, QString photo, bool starost, QDate datePost)
{
    QSqlQuery q, queryForPos;
    q.prepare("INSERT INTO is_student (s_fio,s_group_id ,s_date_birth ,s_date_reg ,s_num_pasport ,s_num_tel ,s_rod_tel ,s_email ,s_r_email ,s_address_now ,s_address_prop ,s_shcool ,s_form_obuch ,s_skidka ,s_form_oplat ,s_otch ,s_photo ,s_starosta ,s_date_post ) \
              VALUES  (:s_fio,:s_group_id, :s_date_birth, :s_date_reg, :s_num_pasport, :s_num_tel, :s_rod_tel, :s_email  , :s_r_email, :s_address_now, :s_address_prop, :s_shcool, :s_form_obuch, :s_skidka, :s_form_oplat, :s_otch  , :s_photo , :starost, :s_date_post);  ");
            q.bindValue(":s_fio", fio);
    q.bindValue(":s_group_id", group_id);
    q.bindValue(":s_date_birth", dateBirth);
    q.bindValue(":s_date_reg", dateReg);
    q.bindValue(":s_num_pasport", pasport);
    q.bindValue(":s_num_tel", tel);
    q.bindValue(":s_rod_tel", rodTel);
    q.bindValue(":s_email", email);
    q.bindValue(":s_r_email", rodEmail);
    q.bindValue(":s_address_now", adresProj);
    q.bindValue(":s_address_prop", adresProp);
    q.bindValue(":s_shcool", shkola);
    q.bindValue(":s_form_obuch", formaObuch_id);
    q.bindValue(":s_skidka", skidka);
    q.bindValue(":s_form_oplat", formaOplaty_id);
    q.bindValue(":s_otch", otch);
    q.bindValue(":s_photo", photo);
    q.bindValue(":starost", starost);
    q.bindValue(":s_date_post", datePost);
    return q.exec();
}

bool Dal_studentsControl::editStudent(int id_student, QString fio, int group_id, QDate dateBirth, QDate dateReg, QString pasport, QString tel, QString rodTel, QString email, QString rodEmail, QString adresProj, QString adresProp, QString shkola, int formaObuch_id, double skidka, int formaOplaty_id, bool otch, QString photo, bool starost, QDate dataPost)
{
    QSqlQuery q;
    q.prepare("UPDATE kafedraDB.is_student SET s_fio = :s_fio , s_group_id = :s_group_id , s_date_birth = :s_date_birth , s_date_reg = :s_date_reg , s_num_pasport = :s_num_pasport , s_num_tel = :s_num_tel , s_rod_tel = :s_rod_tel , s_email = :s_email , s_r_email = :s_r_email , s_address_now = :s_address_now , s_address_prop = :s_address_prop , s_shcool = :s_shcool , s_form_obuch = :s_form_obuch , s_skidka = :s_skidka , s_form_oplat = :s_form_oplat , s_otch = :s_otch, s_photo = :s_photo, s_starosta = :starost, s_date_post = :s_date_post WHERE id_student = :id_student");
    q.bindValue(":id_student", id_student);
    q.bindValue(":s_fio", fio);
    q.bindValue(":s_group_id", group_id);
    q.bindValue(":s_date_birth", dateBirth);
    q.bindValue(":s_date_reg", dateReg);
    q.bindValue(":s_num_pasport", pasport);
    q.bindValue(":s_num_tel", tel);
    q.bindValue(":s_rod_tel", rodTel);
    q.bindValue(":s_email", email);
    q.bindValue(":s_r_email", rodEmail);
    q.bindValue(":s_address_now", adresProj);
    q.bindValue(":s_address_prop", adresProp);
    q.bindValue(":s_shcool", shkola);
    q.bindValue(":s_form_obuch", formaObuch_id);
    q.bindValue(":s_skidka", skidka);
    q.bindValue(":s_form_oplat", formaOplaty_id);
    q.bindValue(":s_otch", otch);
    q.bindValue(":s_photo", photo);
    q.bindValue(":starost", starost);
    q.bindValue(":s_date_post", dataPost);
    return q.exec();
}

bool Dal_studentsControl::DiplomOcenkaUpdate(int id_dip, QString ocenk, QDate dates)
{
    QSqlQuery q;
    q.prepare("UPDATE is_diploms SET ocenka = :ocenk, 	data_zaschiti = :dates WHERE id_diplom = " + QString::number(id_dip));
    q.bindValue(":ocenk", ocenk);
    q.bindValue(":dates", dates.toString("yyyy-MM-dd"));
    q.bindValue(":id_dip", id_dip);
    return q.exec();
}

bool Dal_studentsControl::addObchejitie(int student_id, QString adres, double oplata, QString otzivi)
{
    QSqlQuery q;
    q.prepare("INSERT INTO is_obchejitie (obch_student, obch_adress, obch_oplata, obch_otzivi) VALUES (:student_id, :adres, :oplata, :otzivi)");
    q.bindValue(":student_id", student_id);
    q.bindValue(":adres", adres);
    q.bindValue(":oplata", oplata);
    q.bindValue(":otzivi", otzivi);
    return q.exec();
}

bool Dal_studentsControl::editObchejitie(int id_obch, int student_id, QString adres, double oplata, QString otzivi)
{
    QSqlQuery q;
    q.prepare("UPDATE is_obchejitie SET obch_student = :student_id, obch_adress = :adres, obch_oplata = :oplata, obch_otzivi = :otzivi WHERE id_obch = " + QString::number(id_obch));
    q.bindValue(":student_id", student_id);
    q.bindValue(":adres", adres);
    q.bindValue(":oplata", oplata);
    q.bindValue(":otzivi", otzivi);
    return q.exec();
}

QSqlQueryModel *Dal_studentsControl::getComboGroup(int id_spec)
{
    QString query="";
    QSqlQueryModel *comboGroupModel = new QSqlQueryModel(this);
    query="select id_group, g_name from is_group";
    if(id_spec!=0)
        query.append(" where specialnost_id = " + QString::number(id_spec));
    comboGroupModel->setQuery(query);
    return comboGroupModel;
}

QSqlQueryModel *Dal_studentsControl::getSpec()
{
    QSqlQueryModel *comboSpecModel = new QSqlQueryModel(this);
    comboSpecModel->setQuery("select * from is_specialnosti");
    return comboSpecModel;
}

QSqlQueryModel *Dal_studentsControl::getComboTypePrac()
{
    QSqlQueryModel *comboTypePracModel = new QSqlQueryModel(this);
    comboTypePracModel->setQuery("select * from is_prac_type");
    return comboTypePracModel;
}

QSqlQueryModel *Dal_studentsControl::getStudentGroup(int group_id)
{
    QSqlQueryModel *comboStudentModel = new QSqlQueryModel(this);
    comboStudentModel->setQuery("select * from is_student where s_otch = 0 and s_group_id = " + QString::number(group_id));
    return comboStudentModel;
}

QSqlQueryModel *Dal_studentsControl::getGakBally(int spec_id, int group_id, int gak_id)
{
    QString query = "select * from gak_bally_view where 1=1";
    QSqlQueryModel *model = new QSqlQueryModel(this);
    if(spec_id!=0)
        query.append(" and specialnost_id = " + QString::number(spec_id));
    if(group_id!=0)
        query.append(" and id_group = " + QString::number(group_id));
    if(gak_id!=0)
        query.append(" and gak_id = " + QString::number(gak_id));
    model->setQuery(query);
    model->setHeaderData(4, Qt::Horizontal,tr("Специальность"));
    model->setHeaderData(5, Qt::Horizontal,tr("Группа"));
    model->setHeaderData(6, Qt::Horizontal,tr("ФИО студента"));
    model->setHeaderData(11, Qt::Horizontal,tr("Не допущен"));
    model->setHeaderData(12, Qt::Horizontal,tr("Средняя оценка"));
    return model;
}

QSqlQueryModel *Dal_studentsControl::getDiplomsModel(QString stud, int grupa)
{
    QString query = "";
    QSqlQueryModel *DiplModel = new QSqlQueryModel(this);
    query.append("select * from DiplomView where 1=1");
    if(stud!="")
        query.append(" and s_fio_stud like '%" + stud + "%'  ");
    if(grupa!=0)
        query.append(" and id_group = " + QString::number(grupa));
    DiplModel->setQuery(query);
    DiplModel->setHeaderData(6,Qt::Horizontal,tr("ФИО студента"));
    DiplModel->setHeaderData(7,Qt::Horizontal,tr("Тема на русском"));
    DiplModel->setHeaderData(8,Qt::Horizontal,tr("Тема на кыргызском"));
    DiplModel->setHeaderData(9,Qt::Horizontal,tr("Год формирования\n комиссии"));
    DiplModel->setHeaderData(10,Qt::Horizontal,tr("Оценка"));
    DiplModel->setHeaderData(11,Qt::Horizontal,tr("ФИО преподавателя"));
    DiplModel->setHeaderData(12,Qt::Horizontal,tr("Дата защиты"));
    return DiplModel;
}

QSqlQueryModel *Dal_studentsControl::getcomborucPrac()
{
    QSqlQueryModel *comboRukPracModel = new QSqlQueryModel(this);
    comboRukPracModel->setQuery("select * from is_sotrudniki");
    return comboRukPracModel;
}

QSqlQueryModel *Dal_studentsControl::getcomboSem()
{
    QSqlQueryModel *comboSemmestrModel = new QSqlQueryModel(this);
    comboSemmestrModel->setQuery("select * from is_semmestr");
    return comboSemmestrModel;
}



QSqlQueryModel *Dal_studentsControl::getTemadiploma()
{
    QSqlQueryModel *comboTemaModel = new QSqlQueryModel(this);
    comboTemaModel->setQuery("select * from is_diplom_themes");
    return comboTemaModel;
}

QSqlQueryModel *Dal_studentsControl::getFormaObuch()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("select * from is_form_obuch");
    return model;
}

QSqlQueryModel *Dal_studentsControl::getFormaOplaty()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("select * from is_form_opl");
    return model;
}

QSqlQueryModel *Dal_studentsControl::getJurnVzaimopos(QString prov, QDate datestart, QDate dateend)
{
    QString query = "";
    QSqlQueryModel *JurnVzaimoposmodel = new QSqlQueryModel(this);
    query.append("select * from JurnalVzaimopos_view where 1=1");
    if(prov!="")
        query.append(" and FIO_prov like '%" + prov + "%'  ");
    if(!datestart.isNull())
        query.append(" and datePosesh BETWEEN '" + datestart.toString("yyyy-MM-dd") + "' AND '" + dateend.toString("yyyy-MM-dd 23:59:59") + "'");
    JurnVzaimoposmodel->setQuery(query);
    JurnVzaimoposmodel->setHeaderData(5,Qt::Horizontal,tr("ФИО преподавателя"));
    JurnVzaimoposmodel->setHeaderData(6,Qt::Horizontal,tr("Вид дисциплины"));
    JurnVzaimoposmodel->setHeaderData(7,Qt::Horizontal,tr("Дисциплина"));
    JurnVzaimoposmodel->setHeaderData(8,Qt::Horizontal,tr("Группа"));
    JurnVzaimoposmodel->setHeaderData(9,Qt::Horizontal,tr("Дата посещения"));
    JurnVzaimoposmodel->setHeaderData(10,Qt::Horizontal,tr("Отзыв"));
    JurnVzaimoposmodel->setHeaderData(11,Qt::Horizontal,tr("Проверяющий"));
    JurnVzaimoposmodel->setHeaderData(12,Qt::Horizontal,tr("Оценка"));
    return JurnVzaimoposmodel;
}

//QSqlQueryModel *Dal_studentsControl::getNomRapFromSostavGAK()
//{
//    QSqlQueryModel *GodFormKomiss = new QSqlQueryModel(this);
//    GodFormKomiss->setQuery("select `gak-id`,date_format(date,'%Y') from is_sostav_gak");
//    return GodFormKomiss;
//}

QSqlQueryModel *Dal_studentsControl::getSotrKaf(int sotr_id)
{
    QString query="";
    QSqlQueryModel *getSotrKafedrModel = new QSqlQueryModel(this);
    query.append("select * from sotr_kafedri where 1=1 ");
    if(sotr_id!=0)
        query.append(" and id_sotr <> " + QString::number(sotr_id));
    getSotrKafedrModel->setQuery(query);
    return getSotrKafedrModel;

}

QSqlQueryModel *Dal_studentsControl::getDiscipl()
{
    QSqlQueryModel *Disciplina = new QSqlQueryModel(this);
    Disciplina->setQuery("select * from is_discipline group by d_name");
    return Disciplina;
}

QSqlQueryModel *Dal_studentsControl::getKafedry()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("select * from is_kafedry");
    return model;
}

QSqlQueryModel *Dal_studentsControl::getFormKom()
{
    QSqlQueryModel *FormKomGod = new QSqlQueryModel(this);
    FormKomGod->setQuery("select `gak-id`,date_format(`date`,'%Y') from is_sostav_gak");
    return FormKomGod;
}

QSqlQueryModel *Dal_studentsControl::getTemaRusKG(int id_theme)
{
    QString query = "";
    QSqlQueryModel *ComboTemaRusKg = new QSqlQueryModel(this);
    query.append("select * from is_diplom_themes where 1=1");
    if(id_theme!=0)
        query.append(" and id_diplom_tema = " + QString::number(id_theme));
    ComboTemaRusKg->setQuery(query);
    return ComboTemaRusKg;
}

QSqlQueryModel *Dal_studentsControl::getComboRuk()
{
    QSqlQueryModel *ComboRukovod = new QSqlQueryModel(this);
    ComboRukovod->setQuery("select * from sotr_kafedri");
    return ComboRukovod;
}

QSqlQueryModel *Dal_studentsControl::getVipusk_ki()
{
    QSqlQueryModel *Vipusk_kis = new QSqlQueryModel(this);
    Vipusk_kis->setQuery("select * from OkonchivshieStudenti order by g_name");
    Vipusk_kis->setHeaderData(4,Qt::Horizontal,tr("Группа"));
    Vipusk_kis->setHeaderData(5,Qt::Horizontal,tr("ФИО"));
    Vipusk_kis->setHeaderData(6,Qt::Horizontal,tr("Номер телефона"));
    Vipusk_kis->setHeaderData(7,Qt::Horizontal,tr("E-mail"));
    //    Vipusk_kis->setHeaderData(8,Qt::Horizontal,tr("Дата защиты"));
    return Vipusk_kis;
}

QSqlQueryModel *Dal_studentsControl::getBolezniStud(int group, QString bolez, QDate dateZbol, QDate dateVizdorav)
{
    QString query = "";
    QSqlQueryModel *boleznistudModels = new QSqlQueryModel(this);
    query.append("select * from BolezniStudView where 1=1");
    if(group!=0)
        query.append(" and id_group = " + QString::number(group));
    if(bolez!="")
        query.append(" and bolezni like '%" + bolez + "%'  ");
    if(!dateZbol.isNull())
        query.append(" and date_in > '" + dateZbol.toString("yyyy-MM-dd") + "' and date_out < '" + dateVizdorav.toString("yyyy-MM-dd 23:59:59") + "'");
    qDebug()<<query;
    boleznistudModels->setQuery(query);
    boleznistudModels->setHeaderData(4,Qt::Horizontal,tr("Группа"));
    boleznistudModels->setHeaderData(5,Qt::Horizontal,tr("ФИО студента"));
    boleznistudModels->setHeaderData(6,Qt::Horizontal,tr("Болезнь"));
    boleznistudModels->setHeaderData(7,Qt::Horizontal,tr("Дата заболевания"));
    boleznistudModels->setHeaderData(8,Qt::Horizontal,tr("Дата выздоравления"));
    return boleznistudModels;
}

QSqlQueryModel *Dal_studentsControl::getStudents(int spec_id, int group_id, QString student)
{
    QString query = "";
    QSqlQueryModel *studentModel = new QSqlQueryModel(this);
    query.append("select * from students_view where 1=1 ");
    if(spec_id!=0)
        query.append(" and specialnost_id = " + QString::number(spec_id));
    if(group_id!=0)
        query.append(" and s_group_id = " + QString::number(group_id));
    if(student!="")
        query.append(" and s_fio like '%" + student + "%'  ");
    studentModel->setQuery(query);
    studentModel->setHeaderData(4,Qt::Horizontal,tr("ФИО"));
    studentModel->setHeaderData(5,Qt::Horizontal,tr("Специальность"));
    studentModel->setHeaderData(6,Qt::Horizontal,tr("Группа"));
    studentModel->setHeaderData(9,Qt::Horizontal,tr("Дата рождения"));
    studentModel->setHeaderData(10,Qt::Horizontal,tr("Телефон"));
    studentModel->setHeaderData(11,Qt::Horizontal,tr("Email"));
    studentModel->setHeaderData(7,Qt::Horizontal,tr("Форма обучения"));
    studentModel->setHeaderData(8,Qt::Horizontal,tr("Форма оплаты"));
    return studentModel;
}

QSqlQueryModel *Dal_studentsControl::getStudentsReport(int specialnost_id, int group_id, QDate dateStart, QDate dateEnd, int formaObuch_id, int formaOplaty_id)
{
    QString query = "";
    QSqlQueryModel *studentModel = new QSqlQueryModel(this);
    query.append("select * from students_view_report where 1=1 ");
    if(specialnost_id!=0)
        query.append(" AND id_specialnosti = " + QString::number(specialnost_id));
    if(group_id!=0)
        query.append(" AND s_group_id = " + QString::number(group_id));
    if(!dateStart.isNull())
        query.append(" AND s_date_post BETWEEN '" + dateStart.toString("yyyy-MM-dd") + "' AND '" + dateEnd.toString("yyyy-MM-dd 23:59:59") + "'");
    if(formaObuch_id!=0)
        query.append(" AND id_form_obuch = " + QString::number(formaObuch_id));
    if(formaOplaty_id!=0)
        query.append(" AND id_form_oplat = " + QString::number(formaOplaty_id));
    query.append(" ORDER BY id_specialnosti, s_group_id, id_form_oplat, s_fio");
    studentModel->setQuery(query);
    return studentModel;
}

QSqlQueryModel *Dal_studentsControl::getPractikiSvodReport(int specialnost_id, int tipPrac, QDate dateStart, QDate dateEnd)
{
    QString query = ""; // охрененный сводный отчёт ))
    QSqlQueryModel *model = new QSqlQueryModel(this);
    query = "SELECT Sel1.*, Sel4.`Kolvo3`, Sel3.`Kolvo2`, Sel2.`Kolvo1`,Sel0.`Kolvo`\
            FROM (SELECT `is_group`.`id_group`, `is_group`.`specialnost_id`, `is_prac_work`.`pr_type`, `is_specialnosti`.`spec_name`, `is_prac_type`.`type`, `is_group`.`g_name`\
                  FROM `is_group` AS `is_group`, \
                  `is_specialnosti` AS `is_specialnosti`, \
                  `is_student` AS `is_student`, \
                  `is_prac_work` AS `is_prac_work`, \
                  `is_prac_type` AS `is_prac_type` \
                  WHERE `is_group`.`specialnost_id` = `is_specialnosti`.`id_specialnosti` \
            AND `is_student`.`s_group_id` = `is_group`.`id_group` \
            AND `is_prac_work`.`pr_student` = `is_student`.`id_student` \
            AND `is_prac_work`.`pr_type` = `is_prac_type`.`id_practice_type`";
            if(specialnost_id != 0) query += " AND `is_group`.`specialnost_id` = :specialnost_id";
            if(tipPrac != 0)  query += " AND `is_prac_work`.`pr_type` = :tipPrac ";
    query += " AND `is_prac_work`.`pr_year` BETWEEN :beginDate AND :endDate \
            GROUP BY `is_group`.`id_group`) as sel1\
            LEFT OUTER JOIN (\
                SELECT `is_group`.`id_group`, COUNT( `is_prac_work`.`id_practice` ) AS `Kolvo` \
                FROM `is_group` AS `is_group`, \
                `is_specialnosti` AS `is_specialnosti`, \
                `is_student` AS `is_student`, \
                `is_prac_work` AS `is_prac_work`, \
                `is_prac_type` AS `is_prac_type` \
                WHERE `is_group`.`specialnost_id` = `is_specialnosti`.`id_specialnosti` \
            AND `is_student`.`s_group_id` = `is_group`.`id_group` \
            AND `is_prac_work`.`pr_student` = `is_student`.`id_student` \
            AND `is_prac_work`.`pr_type` = `is_prac_type`.`id_practice_type`";
            if(specialnost_id != 0) query += " AND `is_group`.`specialnost_id` = :specialnost_id";
            if(tipPrac != 0)  query += " AND `is_prac_work`.`pr_type` = :tipPrac ";
    query += " AND `is_prac_work`.`pr_year` BETWEEN :beginDate AND :endDate \
            AND `is_prac_work`.`pr_ocenka` BETWEEN 0 AND 60 \
            GROUP BY `is_group`.`id_group`) AS sel0 ON sel1.`id_group` = sel0.`id_group`\
            LEFT OUTER JOIN (\
                SELECT `is_group`.`id_group`, COUNT( `is_prac_work`.`id_practice` ) AS `Kolvo1` \
                FROM `is_group` AS `is_group`, \
                `is_specialnosti` AS `is_specialnosti`, \
                `is_student` AS `is_student`, \
                `is_prac_work` AS `is_prac_work`, \
                `is_prac_type` AS `is_prac_type` \
                WHERE `is_group`.`specialnost_id` = `is_specialnosti`.`id_specialnosti` \
            AND `is_student`.`s_group_id` = `is_group`.`id_group` \
            AND `is_prac_work`.`pr_student` = `is_student`.`id_student` \
            AND `is_prac_work`.`pr_type` = `is_prac_type`.`id_practice_type`";
            if(specialnost_id != 0) query += " AND `is_group`.`specialnost_id` = :specialnost_id";
            if(tipPrac != 0)  query += " AND `is_prac_work`.`pr_type` = :tipPrac ";
    query += " AND `is_prac_work`.`pr_year` BETWEEN :beginDate AND :endDate \
            AND `is_prac_work`.`pr_ocenka` BETWEEN 61 AND 73 \
            GROUP BY `is_group`.`id_group`) AS sel2 ON sel1.`id_group` = sel2.`id_group`\
            LEFT OUTER JOIN (\
                SELECT `is_group`.`id_group`, COUNT( `is_prac_work`.`id_practice` ) AS `Kolvo2` \
                FROM `is_group` AS `is_group`, \
                `is_specialnosti` AS `is_specialnosti`, \
                `is_student` AS `is_student`, \
                `is_prac_work` AS `is_prac_work`, \
                `is_prac_type` AS `is_prac_type` \
                WHERE `is_group`.`specialnost_id` = `is_specialnosti`.`id_specialnosti` \
            AND `is_student`.`s_group_id` = `is_group`.`id_group` \
            AND `is_prac_work`.`pr_student` = `is_student`.`id_student` \
            AND `is_prac_work`.`pr_type` = `is_prac_type`.`id_practice_type` ";
            if(specialnost_id != 0) query += " AND `is_group`.`specialnost_id` = :specialnost_id";
            if(tipPrac != 0)  query += " AND `is_prac_work`.`pr_type` = :tipPrac ";
    query += " AND `is_prac_work`.`pr_year` BETWEEN :beginDate AND :endDate \
            AND `is_prac_work`.`pr_ocenka` BETWEEN 74 AND 86 \
            GROUP BY `is_group`.`id_group`) AS sel3 ON sel1.`id_group` = sel3.`id_group`\
            LEFT OUTER JOIN (\
                SELECT `is_group`.`id_group`, COUNT( `is_prac_work`.`id_practice` ) AS `Kolvo3` \
                FROM `is_group` AS `is_group`, \
                `is_specialnosti` AS `is_specialnosti`, \
                `is_student` AS `is_student`, \
                `is_prac_work` AS `is_prac_work`, \
                `is_prac_type` AS `is_prac_type` \
                WHERE `is_group`.`specialnost_id` = `is_specialnosti`.`id_specialnosti` \
            AND `is_student`.`s_group_id` = `is_group`.`id_group` \
            AND `is_prac_work`.`pr_student` = `is_student`.`id_student` \
            AND `is_prac_work`.`pr_type` = `is_prac_type`.`id_practice_type`";
            if(specialnost_id != 0) query += " AND `is_group`.`specialnost_id` = :specialnost_id";
            if(tipPrac != 0)  query += " AND `is_prac_work`.`pr_type` = :tipPrac ";
    query += " AND `is_prac_work`.`pr_year` BETWEEN :beginDate AND :endDate \
            AND `is_prac_work`.`pr_ocenka` BETWEEN 87 AND 100 \
            GROUP BY `is_group`.`id_group`) AS sel4 ON sel1.`id_group` = sel4.`id_group`\
            ORDER BY sel1.`specialnost_id`, sel1.`pr_type`, sel1.`g_name`";
            query.replace(":beginDate","\"" + dateStart.toString("yyyy-MM-dd") + "\"");
    query.replace(":endDate", "\"" + dateEnd.toString("yyyy-MM-dd") + "\"");
    query.replace(":specialnost_id", QString::number(specialnost_id));
    query.replace(":tipPrac", QString::number(tipPrac));
    model->setQuery(query);
    model->setHeaderData(3,Qt::Horizontal,tr("Специальность"));
    model->setHeaderData(4,Qt::Horizontal,tr("Тип практики"));
    model->setHeaderData(5,Qt::Horizontal,tr("Группа"));
    model->setHeaderData(6,Qt::Horizontal,tr("Кол-во 5-к"));
    model->setHeaderData(7,Qt::Horizontal,tr("Кол-во 4-к"));
    model->setHeaderData(8,Qt::Horizontal,tr("Кол-во 3-к"));
    model->setHeaderData(9,Qt::Horizontal,tr("Кол-во 2-к"));
    //    qDebug()<<query;
    return model;
}

QSqlQueryModel *Dal_studentsControl::getGakSvodReport(int specialnost_id, int group_id, int gak_id)
{
    QString query = "";
    QSqlQueryModel *model = new QSqlQueryModel(this);
    query = "SELECT `gak_id` as `gak`,\
            `specialnost_id`, \
            `id_group` as `gr`, \
            `spec_name`,  \
            `g_name`, \
            COUNT(`sred`) AS `vsego`, \
            (SELECT COUNT( `sred` ) FROM `gak_bally_view` \
             WHERE  `nedopusk` = 0 AND `gak_id` = `gak` AND `id_group` = `gr`) AS `dopusheno`, \
            (SELECT COUNT( `student_id` ) FROM `zdavshie_gak_view` \
             WHERE  `gak_id` = `gak` AND `id_group` = `gr`) AS `sdali`,\
            ((SELECT COUNT( `sred` ) FROM `gak_bally_view` \
              WHERE  `nedopusk` = 0 AND `gak_id` = `gak` AND `id_group` = `gr`) - (SELECT COUNT( `student_id` ) FROM `zdavshie_gak_view` \
                                                                                   WHERE  `gak_id` = `gak` AND `id_group` = `gr`)) AS `neud`,\
            (SELECT COUNT( `sred` ) FROM `zdavshie_gak_view` \
             WHERE  `sred` BETWEEN 61 AND 73 AND `gak_id` = `gak` AND `id_group` = `gr`) AS `ud`, \
            (SELECT COUNT( `sred` ) FROM `zdavshie_gak_view` \
             WHERE  `sred` BETWEEN 74 AND 86 AND `gak_id` = `gak` AND `id_group` = `gr`) AS `hor`,\
            (SELECT COUNT( `sred` ) FROM `zdavshie_gak_view` \
             WHERE  `sred` >= 87 AND `gak_id` = `gak` AND `id_group` = `gr`) AS `otl`\
            FROM `gak_bally_view` WHERE 1 = 1 ";
            if(gak_id != 0 ) query += " AND `gak_id` = " + QString::number(gak_id);
    if(specialnost_id != 0 ) query += " AND `specialnost_id` = " + QString::number(specialnost_id);
    if(group_id != 0 ) query += " AND `id_group` = " + QString::number(group_id);
    query += " GROUP BY `id_group` \
            ORDER BY `spec_name`, `g_name`";
            model->setQuery(query);
    model->setHeaderData(3,Qt::Horizontal,tr("Специальность"));
    model->setHeaderData(4,Qt::Horizontal,tr("Группа"));
    model->setHeaderData(5,Qt::Horizontal,tr("Всего студентов"));
    model->setHeaderData(6,Qt::Horizontal,tr("Допущено"));
    model->setHeaderData(7,Qt::Horizontal,tr("Сдали"));
    model->setHeaderData(8,Qt::Horizontal,tr("Кол-во 2-к"));
    model->setHeaderData(9,Qt::Horizontal,tr("Кол-во 3-к"));
    model->setHeaderData(10,Qt::Horizontal,tr("Кол-во 4-к"));
    model->setHeaderData(11,Qt::Horizontal,tr("Кол-во 5-к"));
    qDebug()<<query;
    return model;
}

QSqlQueryModel *Dal_studentsControl::getStudent(int groups)
{
    QSqlQueryModel *StudentModel = new QSqlQueryModel(this);
    StudentModel->setQuery("select * from is_student where s_group_id = " + QString::number(groups) + " and s_otch = 0");
    StudentModel->setHeaderData(1,Qt::Horizontal,tr("ФИО студента"));
    return StudentModel;
}

QSqlQueryModel *Dal_studentsControl::getStudentineit(int id_stud)
{
    QSqlQueryModel *StudentModel = new QSqlQueryModel(this);
    StudentModel->setQuery("select * from is_student where id_student = " + QString::number(id_stud) + " and s_otch = 0");
    StudentModel->setHeaderData(1,Qt::Horizontal,tr("ФИО студента"));
    return StudentModel;
}

QSqlQueryModel *Dal_studentsControl::SendMessageStud(int groups)
{
    QString query="";
    QSqlQueryModel *RassilkaStudModel = new QSqlQueryModel(this);
    query.append("select * from NeOtchStud where 1=1");
    if(groups!=0)
        query.append(" and s_group_id = " + QString::number(groups));
    RassilkaStudModel->setQuery(query);
    //    RassilkaStudModel->setHeaderData(1,Qt::Horizontal,tr("Группа"));
    RassilkaStudModel->setHeaderData(1,Qt::Horizontal,tr("ФИО студента"));
    RassilkaStudModel->setHeaderData(8,Qt::Horizontal,tr("E-mail"));
    return RassilkaStudModel;
}

QSqlQuery *Dal_studentsControl::getStudentNeotch(int id_stud)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select id_student, s_fio from is_student where s_otch = 0 and id_student = :id_stud");
    query->bindValue(":id_stud", id_stud);
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

QSqlQueryModel *Dal_studentsControl::getTrudoVipusk(QString Stud, int groups)
{
    QString query="";
    QSqlQueryModel *TrudoVipModel = new QSqlQueryModel(this);
    query.append("select * from TrrudoUstrVipusknikovView where 1=1");
    if(Stud!="")
        query.append(" and s_fio LIKE '%" + Stud + "%'  ");
    if(groups!=0)
        query.append(" and s_group_id = " + QString::number(groups));
    TrudoVipModel->setQuery(query);
    TrudoVipModel->setHeaderData(3,Qt::Horizontal,tr("Группа"));
    TrudoVipModel->setHeaderData(4,Qt::Horizontal,tr("ФИО студента"));
    TrudoVipModel->setHeaderData(5,Qt::Horizontal,tr("Адрес работы"));
    TrudoVipModel->setHeaderData(6,Qt::Horizontal,tr("Работает за границей"));
    TrudoVipModel->setHeaderData(7,Qt::Horizontal,tr("Занимаемая должность"));
    TrudoVipModel->setHeaderData(8,Qt::Horizontal,tr("Год выпуска"));
    return TrudoVipModel;

}

QSqlQueryModel *Dal_studentsControl::getVipGroup(int groupss)
{
    QSqlQueryModel *VipGroup = new QSqlQueryModel(this);
    VipGroup->setQuery("select * from OkonchivshieStudenti where s_group_id = " + QString::number(groupss));
    //    Vipusk_kis->setHeaderData(4,Qt::Horizontal,tr("Группа"));
    //    Vipusk_kis->setHeaderData(5,Qt::Horizontal,tr("ФИО"));
    //    Vipusk_kis->setHeaderData(6,Qt::Horizontal,tr("Номер телефона"));
    //    Vipusk_kis->setHeaderData(7,Qt::Horizontal,tr("E-mail"));
    //    Vipusk_kis->setHeaderData(8,Qt::Horizontal,tr("Дата защиты"));
    return VipGroup;
}

QSqlQueryModel *Dal_studentsControl::getRepTrudoVipusk()
{
    QSqlQueryModel *RepTrudoVipusk_kis = new QSqlQueryModel(this);
    RepTrudoVipusk_kis->setQuery("select * from ReportUstrVipusk");
    RepTrudoVipusk_kis->setHeaderData(0,Qt::Horizontal,tr("Год выпуска"));
    RepTrudoVipusk_kis->setHeaderData(1,Qt::Horizontal,tr("Всего окончили"));
    RepTrudoVipusk_kis->setHeaderData(2,Qt::Horizontal,tr("Трудоустроено чел."));
    RepTrudoVipusk_kis->setHeaderData(3,Qt::Horizontal,tr("Трудоустроено в % чел."));
    RepTrudoVipusk_kis->setHeaderData(4,Qt::Horizontal,tr("Выехали\n за пределы республики"));

    return RepTrudoVipusk_kis;
}

bool Dal_studentsControl::addDiplom(int stud_id, int tema, int kom, int ruk)
{
    QSqlQuery *AddDiploms= new QSqlQuery;
    AddDiploms->prepare("Call ProcedureAddDiplom(:stud_id,:tema,:kom,:ruk,@result)");
    AddDiploms->bindValue(":stud_id",stud_id);
    AddDiploms->bindValue(":tema",tema);
    AddDiploms->bindValue(":kom",kom);
    AddDiploms->bindValue(":ruk",ruk);
    AddDiploms->exec();
    AddDiploms->exec("select @result");
    AddDiploms->first();
    int result = AddDiploms->value(0).toInt();
    if(result == 1)
    {
        QMessageBox::critical(NULL, tr("Ошибка"),tr("Данные не добавлены"));
        return false;

    }
    else
        return true;
}

bool Dal_studentsControl::addProcentovka(int stud_id, QString dat, int veden, int anTreb, int konstr, int docum, int experementRazd, int econom, int zakl, int rabotaPP, int prezentac)
{
    qDebug()<<stud_id<<dat<<veden<<anTreb<<konstr<<anTreb<<docum<<experementRazd<<econom<<zakl<<rabotaPP<<prezentac;
    QSqlQuery *qu = new QSqlQuery;
    qu->prepare("insert into is_procentovka (stud_id, datas, vvedenie, analizTreb, konstruktrRazdel, documentir, ekspertnrazdel, ekonomich, zakl, rabotaPP, prezentacia) \
                                     VALUES (:stud_id,:dat, :veden, :anTreb, :konstr, :docum, :experementRazd, :econom, :zakl, :rabotaPP, :prezentac)");
    qu->bindValue(":stud_id", stud_id);
    qu->bindValue(":dat", dat); //.toString("yyyy-MM-dd")
    qu->bindValue(":veden", veden);
    qu->bindValue(":anTreb", anTreb);
    qu->bindValue(":konstr", konstr);
    qu->bindValue(":docum", docum);
    qu->bindValue(":experementRazd", experementRazd);
    qu->bindValue(":econom", econom);
    qu->bindValue(":zakl", zakl);
    qu->bindValue(":rabotaPP", rabotaPP);
    qu->bindValue(":prezentac", prezentac);
    qu->exec();
    if (!qu->isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_studentsControl::BolezniStudAdd(int stud, QString bolez, QDate datZabol, QDate datVizd)
{
    QSqlQuery *qu = new QSqlQuery;
    qu->prepare("insert into is_stud_bolezni (student, bolezni, date_in, date_out) \
                VALUES (:stud,:bolez, :datZabol, :datVizd)");
                qu->bindValue(":stud", stud); //.toString("yyyy-MM-dd")
            qu->bindValue(":bolez", bolez);
    qu->bindValue(":datZabol", datZabol);
    qu->bindValue(":datZabol", datZabol.toString("yyyy-MM-dd"));
    qu->bindValue(":datVizd", datVizd.toString("yyyy-MM-dd"));
    qu->exec();
    if (!qu->isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

QSqlQueryModel *Dal_studentsControl::getCourseWorks(int group_id, int discipline_id, QString tema, int vedomost_id)
{
    QString query = "SELECT * FROM course_work_view WHERE 1=1 ";
    QSqlQueryModel  *model = new QSqlQueryModel(this);
    if(group_id!=0)
        query.append(" AND id_group = " + QString::number(group_id));
    if(discipline_id!=0)
        query.append(" AND discipline_id = " + QString::number(discipline_id));
    if(!tema.isEmpty())
        query.append(" AND tema LIKE '%" + tema + "%'  ");
    if(vedomost_id!=0)
        query.append(" AND vedomostiKursovye_id = " + QString::number(vedomost_id));
    model->setQuery(query);
    model->setHeaderData(5,Qt::Horizontal,tr("Дисциплина"));
    model->setHeaderData(6,Qt::Horizontal,tr("Группа"));
    model->setHeaderData(7,Qt::Horizontal,tr("ФИО студента"));
    model->setHeaderData(8,Qt::Horizontal,tr("ФИО преподавателя"));
    model->setHeaderData(9,Qt::Horizontal,tr("Тема"));
    model->setHeaderData(11,Qt::Horizontal,tr("Оценка"));
    model->setHeaderData(12,Qt::Horizontal,tr("Дата сдачи"));
    model->setHeaderData(13,Qt::Horizontal,tr("Кол-во листов"));
    model->setHeaderData(15,Qt::Horizontal,tr("Примечание"));
    return model;
}

QSqlQueryModel *Dal_studentsControl::getSrezy(int specialnost_id, int group_id, QDate dateBegin, QDate dateEnd)
{
    QString query = "SELECT * FROM srez_view WHERE 1=1 ";
    QSqlQueryModel  *model = new QSqlQueryModel(this);
    if(specialnost_id!=0)
        query.append(" AND id_specialnosti = " + QString::number(specialnost_id));
    if(group_id!=0)
        query.append(" AND group_id = " + QString::number(group_id));
    if(!dateBegin.isNull())
        query.append(" AND data_sreza between '" + dateBegin.toString("yyyy-MM-dd") + "' AND '" + dateEnd.toString("yyyy-MM-dd 23:59:59") + "'");
    model->setQuery(query);
    model->setHeaderData(2,Qt::Horizontal,tr("Группа"));
    model->setHeaderData(3,Qt::Horizontal,tr("Дисциплина 1"));
    model->setHeaderData(4,Qt::Horizontal,tr("Дисциплина 2"));
    model->setHeaderData(5,Qt::Horizontal,tr("Дисциплина 3"));
    model->setHeaderData(6,Qt::Horizontal,tr("Дисциплина 4"));
    model->setHeaderData(7,Qt::Horizontal,tr("Дисциплина 5"));
    model->setHeaderData(8,Qt::Horizontal,tr("Дата проведения"));
    model->setHeaderData(9,Qt::Horizontal,tr("Специальность"));
    return model;
}

QSqlQuery *Dal_studentsControl::getSrezyQuery(int group_id, QDate dateBegin, QDate dateEnd)
{
    QString q = "SELECT * FROM srez_view WHERE 1=1 ";
    if(group_id!=0)
        q.append(" AND group_id = " + QString::number(group_id));
    if(!dateBegin.isNull())
        q.append(" AND data_sreza between '" + dateBegin.toString("yyyy-MM-dd") + "' AND '" + dateEnd.toString("yyyy-MM-dd 23:59:59") + "'");
    QSqlQuery *query = new QSqlQuery;
    query->prepare(q);
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

QSqlQuery *Dal_studentsControl::getSrezySrednie(int srez_id)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT * FROM srez_srednnie_view WHERE srez_id = " + QString::number(srez_id));
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

bool Dal_studentsControl::getOlimpAdd(int stud_id, QString pred, QDate dates, QString comm, int mest, QString mestProv, QString prveril)
{
    qDebug()<<dates;
    QSqlQuery *query = new QSqlQuery;
    query->prepare("insert into is_olymp (ol_student, ol_subj, ol_date,ol_comments,ol_mesto,ol_mesto_prov, ol_proveril) values (:stud_id, :pred, :dates, :comm, :mest, :mestProv, :prveril)");
    query->bindValue(":stud_id", stud_id);
    query->bindValue(":pred", pred);
    query->bindValue(":dates", dates.toString("yyyy-MM-dd"));
    query->bindValue(":comm", comm);
    if(mest==0)
    {
        //Если место равно 0 то ничего не передавать, будет добавлен NULL
    }
    else
    {
        query->bindValue(":mest", mest);
    }
    query->bindValue(":mestProv", mestProv);
    if(prveril.isEmpty())
    {
        //Если проверяющего не было или нам это не важно то ничего не передавать, будет добавлен NULL
    }
    else
    {
        query->bindValue(":prveril", prveril);
    }
    query->exec();
    if (! query->isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_studentsControl::getJurnVzaimoposAdd(int sotr_id, int discp_id, int group_id, int prover_id, QDateTime dates, QString commentarii, QString vidDiscipl, QString Ocenka)
{
    QSqlQuery *qu = new QSqlQuery;
    qu->prepare("insert into is_JurnalVzaimoposrshenii (sotr_id, discipl_id, group_id, Proveryashii_id, datePosesh, Otziv, tip_discipl, Ocenka) \
                VALUES (:sotr_id,:discp_id, :group_id, :prover_id, :dates, :commentarii, :vidDiscipl, :Ocenka)");
                qu->bindValue(":sotr_id", sotr_id);
            qu->bindValue(":discp_id", discp_id);
    qu->bindValue(":group_id", group_id);
    qu->bindValue(":prover_id", prover_id);
    qu->bindValue(":dates", dates.toString("yyyy-MM-dd H:mm:ss"));
    qu->bindValue(":commentarii", commentarii);
    qu->bindValue(":vidDiscipl", vidDiscipl);
    qu->bindValue(":Ocenka", Ocenka);
    qu->exec();
    if (!qu->isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_studentsControl::GAKAdd(int discp1, int discp2, int discp3, int discp4, QDate dates, QString kab, int nomerPrikaza, int GodFormKom)
{
    QSqlQuery *qu = new QSqlQuery;
    qu->prepare("insert into is_gak (gak_date, gak_kabinet, gak_nomer_prikaza, gak_disceplina1, gak_disceplina2, gak_disceplina3, gak_disceplina4, komissia) \
                VALUES (:dates,:kab, :nomerPrikaza, :discp1, :discp2, :discp3, :discp4, :GodFormKom)");
                qu->bindValue(":dates", dates.toString("yyyy-MM-dd"));
            qu->bindValue(":kab", kab);
    qu->bindValue(":nomerPrikaza", nomerPrikaza);
    qu->bindValue(":discp1", discp1);
    qu->bindValue(":discp2", discp2);
    qu->bindValue(":discp3", discp3);
    qu->bindValue(":discp4", discp4);
    qu->bindValue(":GodFormKom", GodFormKom);
    qu->exec();
    if (!qu->isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_studentsControl::getTrudoVipAdd(int vip_ids, QString addresRab, QString ZanDolj, int RabotZaGranic)
{
    qDebug()<<vip_ids<<addresRab<<ZanDolj<<RabotZaGranic;
    QSqlQuery *qu = new QSqlQuery;
    qu->prepare("insert into is_trudo_vipusknik (vipusknik_id, Adres_raboti, ZanimaemayaDoljnost, Viehali_zaGranicu) \
                VALUES (:vip_ids,:addresRab, :ZanDolj, :RabotZaGranic)");
                qu->bindValue(":vip_ids", vip_ids);
            qu->bindValue(":addresRab", addresRab);
    qu->bindValue(":ZanDolj", ZanDolj);
    qu->bindValue(":RabotZaGranic", RabotZaGranic);
    qu->exec();
    if (!qu->isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_studentsControl::getVipusknikAdd(int stud_id, QString nomer_diploma,  QString addressFoto, QDate dates3, QString emails, QString telefon, QString mestRab, QString dolj)
{
    QSqlQuery *qu = new QSqlQuery;
    qu->prepare("insert into is_vipuskniki (vp_student, vp_nomer_doplom, vp_diplom_scan, vp_finish_year, vp_email, vp_tel, vp_workplace, vp_doljnost)  VALUES (:stud_id, :nomer_diploma, :addressFoto, :dates3, :emails, :telefon, :mestRab, :dolj)");
    qu->bindValue(":stud_id", stud_id);
    qu->bindValue(":nomer_diploma", nomer_diploma);
    qu->bindValue(":addressFoto", addressFoto);
    qu->bindValue(":dates3", dates3.toString("yyyy-MM-dd"));
    if(!emails.isEmpty())
        qu->bindValue(":emails", emails);
    if(!telefon.isEmpty())
        qu->bindValue(":telefon", telefon);

    if(!mestRab.isEmpty())
        qu->bindValue(":mestRab", mestRab);
    if(!dolj.isEmpty())
        qu->bindValue(":dolj", dolj);
    qu->exec();
    if (!qu->isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_studentsControl::getActVnedreniaAdd(int stud_id, QString nomer_acta, QDate dates, QString predpriatie, int tema_id, QString dopolnenia)
{
    QSqlQuery *qu = new QSqlQuery;
    qu->prepare("insert into is_akti_vnedreniya (nomer_akta, data, predpriyatie, proekt, student, dopolnitelno)  VALUES (:nomer_acta, :dates, :predpriatie, :tema_id, :stud_id, :dopolnenia)");
    qu->bindValue(":nomer_acta", nomer_acta);
    qu->bindValue(":dates", dates.toString("yyyy-MM-dd"));
    qu->bindValue(":predpriatie", predpriatie);
    qu->bindValue(":stud_id", stud_id);
    qu->bindValue(":tema_id", tema_id);
    if(!dopolnenia.isEmpty())
        qu->bindValue(":dopolnenia", dopolnenia);
    qu->exec();
    if (!qu->isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_studentsControl::editOlimp(int id_ol, int students, QString predm,  QDate dates, QString comm, int zanMest,QString mestProv, QString prover)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("call updOlimp(:id_ol, :students, :predm, :dates, :comm, :zanMest, :mestProv, :prover, @result)");
    query->bindValue(":id_ol", id_ol);
    query->bindValue(":students", students);
    query->bindValue(":predm", predm);
    query->bindValue(":dates", dates.toString("yyyy-MM-dd"));
    query->bindValue(":comm", comm);
    query->bindValue(":zanMest", zanMest);
    query->bindValue(":mestProv", mestProv);
    query->bindValue(":prover", prover);
    query->exec();
    query->exec(" select @result");
    query->first();
    int result = query->value(0).toInt();
    if(result == 0)
    {
        return true;

    }
    else
        return false;
}

bool Dal_studentsControl::getPracWorkAdd(int typePr_id, QDate dates, int sem_id, int stud_id, int lid_kaf_id, QString lid_predpr, QString mestProh, QString ocenka, QString otziv, QString Nom_prik)
{
    QSqlQuery *qu = new QSqlQuery;
    qu->prepare("insert into is_prac_work (pr_type, pr_year, pr_semestr, pr_student, pr_lead_kaf, pr_lead_place, pr_place,pr_ocenka,pr_otziv, pr_nom_prikaza)  VALUES (:typePr_id, :dates, :sem_id, :stud_id, :lid_kaf_id, :lid_predpr, :mestProh, :ocenka, :otziv, :Nom_prik)");
    qu->bindValue(":typePr_id", typePr_id);
    qu->bindValue(":dates", dates.toString("yyyy-MM-dd"));
    qu->bindValue(":sem_id", sem_id);
    qu->bindValue(":stud_id", stud_id);
    qu->bindValue(":lid_kaf_id", lid_kaf_id);
    if(!lid_predpr.isEmpty())
        qu->bindValue(":lid_predpr", lid_predpr);
    qu->bindValue(":mestProh", mestProh);
    if(!ocenka.isEmpty())
        qu->bindValue(":ocenka", ocenka);
    if (!otziv.isEmpty())
        qu->bindValue(":otziv", otziv);
    if (!Nom_prik.isEmpty())
        qu->bindValue(":Nom_prik", Nom_prik);
    qu->exec();
    if (!qu->isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

QSqlQuery *Dal_studentsControl::getCurrentOlipm(int id_olim)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select Olimpiada_view.id_olymp,\
                   Olimpiada_view.s_group_id,\
                   Olimpiada_view.ol_student,\
                   Olimpiada_view.ol_subj, \
                   Olimpiada_view.ol_date,\
                   Olimpiada_view.ol_comments,\
                   Olimpiada_view.ol_mesto,\
                   Olimpiada_view.ol_mesto_prov, \
                   Olimpiada_view.ol_proveril from Olimpiada_view where Olimpiada_view.id_olymp = :id_olim");
            query->bindValue(":id_olim", id_olim);
            query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

QSqlQuery *Dal_studentsControl::getCurrentJurnalVzaimoPos(int rec)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select * from is_JurnalVzaimoposrshenii where is_JurnalVzaimoposrshenii.id_JurnalVzaimoposrshenii = :rec");
    query->bindValue(":rec", rec);
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

bool Dal_studentsControl::deleteOlimp(int id_olimp)
{
    QSqlQuery query;
    query.prepare("delete from is_olymp where id_olymp = :id_olimp");
    query.bindValue(":id_olimp", id_olimp);
    query.exec();
    if (! query.isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_studentsControl::deleteJurnVzaimPos(int id_jurnalVzaimopos)
{
    QSqlQuery query;
    query.prepare("delete from is_JurnalVzaimoposrshenii where id_JurnalVzaimoposrshenii = :id_jurnalVzaimopos");
    query.bindValue(":id_jurnalVzaimopos", id_jurnalVzaimopos);
    query.exec();
    if (! query.isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_studentsControl::deleteDiploms(int id_diploms)
{
    QSqlQuery query;
    query.prepare("delete from is_diploms where id_diplom = :id_diploms");
    query.bindValue(":id_diploms", id_diploms);
    query.exec();
    if (! query.isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_studentsControl::deleteVipusk(int id_vip)
{
    QSqlQuery query;
    query.prepare("delete from is_vipuskniki where id_vipusknik = :id_vip");
    query.bindValue(":id_vip", id_vip);
    query.exec();
    if (! query.isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_studentsControl::deleteBolezniSotr(int id_bolStud)
{
    QSqlQuery query;
    query.prepare("delete from is_stud_bolezni where id_stud_bolezni = :id_bolStud");
    query.bindValue(":id_bolStud", id_bolStud);
    query.exec();
    if (! query.isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_studentsControl::deletePracWork(int id_prWork)
{
    QSqlQuery query;
    query.prepare("delete from is_prac_work where id_practice = :id_prWork");
    query.bindValue(":id_prWork", id_prWork);
    query.exec();
    if (! query.isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_studentsControl::deleteKonfSem(int konf_sem)
{
    QSqlQuery query;
    query.prepare("delete from is_konf_sem where id_konf = :konf_sem");
    query.bindValue(":konf_sem", konf_sem);
    query.exec();
    if (! query.isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_studentsControl::deleteObshejit(int id_obshejit)
{
    QSqlQuery query;
    query.prepare("delete from is_obchejitie where id_obch = :id_obshejit");
    query.bindValue(":id_obshejit", id_obshejit);
    query.exec();
    if (! query.isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_studentsControl::deleteGAK(int id_gak)
{
    QSqlQuery query;
    query.prepare("delete from is_gak where gak_id = :id_gak");
    query.bindValue(":id_gak", id_gak);
    query.exec();
    if (! query.isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_studentsControl::deleteStudent(int id_stud)
{
    qDebug()<<id_stud;
    QSqlQuery query;
    query.prepare("delete from is_student where id_student = :id_stud");
    query.bindValue(":id_stud", id_stud);
    query.exec();
    if (! query.isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_studentsControl::deleteTrudoVipusk(int id_TrudoVip)
{
    QSqlQuery query;
    query.prepare("delete from is_trudo_vipusknik where id_trudo_vipusknik = :id_TrudoVip");
    query.bindValue(":id_TrudoVip", id_TrudoVip);
    query.exec();
    if (! query.isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

QSqlQueryModel *Dal_studentsControl::getVipusknik(QString stud, QString group, QDate dates, QDate datesEnd)
{
    QString query="", condition="",filter="";
    QSqlQueryModel  *VipusknilModelView = new QSqlQueryModel(this);
    if(stud!="" || group!="" || !dates.isNull())
        filter.append(" where ");
    if(stud!="")
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition.append(" VipusknikiView.`s_fio` like '%" + stud + "%'  ");
    }

    if(group!="")
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition.append(" VipusknikiView.`g_name` like '%" + group + "%'  ");
    }

    if(!dates.isNull())
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition.append(" VipusknikiView.`vp_finish_year` between '" + dates.toString("yyyy-MM-dd") + "' and '" + datesEnd.toString("yyyy-MM-dd 23:59:59") + "'");
    }
    query = "select * from VipusknikiView " + filter + condition;
    VipusknilModelView->setQuery(query);
    VipusknilModelView->setHeaderData(3,Qt::Horizontal,tr("Группа"));
    VipusknilModelView->setHeaderData(4,Qt::Horizontal,tr("ФИО"));
    VipusknilModelView->setHeaderData(5,Qt::Horizontal,tr("Номер диплома"));
    VipusknilModelView->setHeaderData(7,Qt::Horizontal,tr("Год окончания"));
    VipusknilModelView->setHeaderData(8,Qt::Horizontal,tr("E-mail"));
    VipusknilModelView->setHeaderData(9,Qt::Horizontal,tr("Телефон"));
    return VipusknilModelView;
}

QSqlQueryModel *Dal_studentsControl::SendMessageVip(QString group)
{
    QString query="";
    QSqlQueryModel *SendMessageVp_Kam = new QSqlQueryModel(this);
    query.append("select * from OkonchivshieStudenti where 1=1");
    if(group!="")
        query.append(" and `g_name` like '%" + group + "%'  ");
    SendMessageVp_Kam->setQuery(query);
    // SendMessageVp_Kam->setHeaderData(4,Qt::Horizontal,tr("Группа"));
    SendMessageVp_Kam->setHeaderData(5,Qt::Horizontal,tr("ФИО"));
    SendMessageVp_Kam->setHeaderData(7,Qt::Horizontal,tr("E-mail"));
    return SendMessageVp_Kam;
}

QSqlQueryModel *Dal_studentsControl::SendMessageSotrKaf()
{
    QSqlQueryModel *sendMessSotrKaf = new QSqlQueryModel(this);
    sendMessSotrKaf->setQuery("select * from sotr_kafedri");
    sendMessSotrKaf->setHeaderData(1,Qt::Horizontal,tr("ФИО"));
    sendMessSotrKaf->setHeaderData(4,Qt::Horizontal,tr("E-mail"));
    return sendMessSotrKaf;
}

QSqlQueryModel *Dal_studentsControl::getSostavKom(int komis_id)
{
    QString query="";
    QSqlQueryModel *SostavKomismodel = new QSqlQueryModel(this);
    query.append("select * from SostavKomissii ");
    if(komis_id != 0)
        query.append(" where `gak-id` = " + QString::number(komis_id));
    SostavKomismodel->setQuery(query);
    SostavKomismodel->setHeaderData(8, Qt::Horizontal,tr("Председатель"));
    SostavKomismodel->setHeaderData(9, Qt::Horizontal,tr("Секретарь"));
    SostavKomismodel->setHeaderData(10, Qt::Horizontal,tr("Член комиссии №1"));
    SostavKomismodel->setHeaderData(11, Qt::Horizontal,tr("Член комиссии №2"));
    SostavKomismodel->setHeaderData(12, Qt::Horizontal,tr("Член комиссии №3"));
    SostavKomismodel->setHeaderData(13, Qt::Horizontal,tr("Член комиссии №4"));
    SostavKomismodel->setHeaderData(14, Qt::Horizontal,tr("Член комиссии №5"));
    SostavKomismodel->setHeaderData(15, Qt::Horizontal,tr("Дата"));
    SostavKomismodel->setHeaderData(16, Qt::Horizontal,tr("Номер рапорта"));
    return SostavKomismodel;
}

QSqlQuery *Dal_studentsControl::getCurrentVipusknik(int id_vip)
{
    qDebug()<<id_vip;
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select VipusknikiView.id_vipusknik,VipusknikiView.vp_student,VipusknikiView.id_group,VipusknikiView.vp_nomer_doplom,VipusknikiView.vp_diplom_scan,VipusknikiView.vp_finish_year,VipusknikiView.vp_email,VipusknikiView.vp_tel,VipusknikiView.vp_workplace, VipusknikiView.vp_doljnost from VipusknikiView where VipusknikiView.`id_vipusknik` = :id_vip");
    query->bindValue(":id_vip", id_vip);
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

QSqlQuery *Dal_studentsControl::getCurrentActVnedr(int id_act)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select * from ActVnedreniaView where ActVnedreniaView.`id_akt_vnedreniya` = :id_act");
    query->bindValue(":id_act", id_act);
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

bool Dal_studentsControl::editVipusknik(int id_vip, int stud_id, QString nomer_diploma, QString addressFoto, QDate dates3, QString emails, QString telefon, QString mestRab, QString dolj)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("call updVipusk(:id_vip, :stud_id, :nomer_diploma, :addressFoto, :dates3, :emails, :telefon, :mestRab,:dolj, @result)");
    query->bindValue(":id_vip", id_vip);
    query->bindValue(":stud_id", stud_id);
    query->bindValue(":nomer_diploma", nomer_diploma);
    query->bindValue(":addressFoto", addressFoto);
    query->bindValue(":dates3", dates3.toString("yyyy-MM-dd"));
    query->bindValue(":emails", emails);
    query->bindValue(":telefon", telefon);
    query->bindValue(":mestRab", mestRab);
    query->bindValue(":dolj", dolj);
    query->exec();
    query->exec(" select @result");
    query->first();
    int result = query->value(0).toInt();
    if(result == 0)
    {
        return true;

    }
    else
        return false;
}

bool Dal_studentsControl::editBoleznStud(int id_bol_stud, int stud_id, QString bolezn, QDate dateStart, QDate dateEnds)
{
    QSqlQuery q;
    q.prepare("UPDATE is_stud_bolezni SET student = :stud_id, bolezni = :bolezn, date_in = :dateStart, date_out = :dateEnds WHERE id_stud_bolezni = " + QString::number(id_bol_stud));

    q.bindValue(":stud_id", stud_id);
    q.bindValue(":bolezn", bolezn);
    q.bindValue(":dateStart", dateStart.toString("yyyy-MM-dd H:mm:ss"));
    q.bindValue(":dateEnds", dateEnds.toString("yyyy-MM-dd H:mm:ss"));
    return q.exec();
}

bool Dal_studentsControl::editPracWork(int id_prac, int prac_id, QDate dates, int semestr_id, int stud_id, int lid_kaf, QString lid_placePredp, QString Mestoprohoj, QString ocenka, QString otziv, QString nom_prikaza)
{
    qDebug()<<id_prac << "," << prac_id<< ","<< dates<< "," << semestr_id << ","<< stud_id<< ","<< lid_kaf<< ","<< lid_placePredp <<","<< Mestoprohoj <<","<< ocenka << "," << otziv <<"," <<nom_prikaza;
    QSqlQuery *query = new QSqlQuery;
    query->prepare("call updPracWork(:id_prac, :prac_id, :dates, :semestr_id, :stud_id, :lid_kaf, :lid_placePredp, :Mestoprohoj, :ocenka, :otziv, :nom_prikaza, @result)");
    query->bindValue(":id_prac", id_prac);
    query->bindValue(":prac_id", prac_id);
    query->bindValue(":dates", dates.toString("yyyy-MM-dd"));
    query->bindValue(":semestr_id", semestr_id);
    query->bindValue(":stud_id", stud_id);

    query->bindValue(":lid_kaf", lid_kaf);
    query->bindValue(":lid_placePredp", lid_placePredp);
    query->bindValue(":Mestoprohoj", Mestoprohoj);
    query->bindValue(":ocenka", ocenka);
    query->bindValue(":otziv", otziv);
    query->bindValue(":nom_prikaza", nom_prikaza);
    query->exec();
    query->exec(" select @result");
    query->first();
    int result = query->value(0).toInt();
    if(result == 0)
    {
        return true;

    }
    else
        return false;
}

bool Dal_studentsControl::editActVnedr(int id_actvndr, int stud_id, QString nomer_acta, QDate dates, QString predpriatie, int tema_id, QString dopolnenia)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("call updActVnedren(:id_actvndr, :stud_id, :nomer_acta, :dates, :predpriatie, :tema_id, :dopolnenia, @result)");
    query->bindValue(":id_actvndr", id_actvndr);
    query->bindValue(":stud_id", stud_id);
    query->bindValue(":nomer_acta", nomer_acta);
    query->bindValue(":dates", dates.toString("yyyy-MM-dd"));
    query->bindValue(":predpriatie", predpriatie);
    query->bindValue(":tema_id", tema_id);
    query->bindValue(":dopolnenia", dopolnenia);
    query->exec();
    query->exec(" select @result");
    query->first();
    int result = query->value(0).toInt();
    if(result == 0)
    {
        return true;

    }
    else
        return false;
}

bool Dal_studentsControl::JurnVzaimoposEdit(int record, int sotr_id, int discp_id, int group_id, int prover_id, QDateTime dates, QString commentarii, QString tip_discp, QString Ocenka)
{
    qDebug()<<record<<sotr_id<<discp_id<<group_id<<prover_id<<dates<<commentarii;
    QSqlQuery q;
    q.prepare("UPDATE is_JurnalVzaimoposrshenii SET sotr_id = :sotr_id, discipl_id = :discp_id, group_id = :group_id, Proveryashii_id = :prover_id, datePosesh = :dates, Otziv = :commentarii, tip_discipl = :tip_discp, Ocenka = :Ocenka WHERE id_JurnalVzaimoposrshenii = " + QString::number(record));

    q.bindValue(":sotr_id", sotr_id);
    q.bindValue(":discp_id", discp_id);
    q.bindValue(":group_id", group_id);
    q.bindValue(":prover_id", prover_id);
    q.bindValue(":dates", dates.toString("yyyy-MM-dd H:mm:ss"));
    q.bindValue(":commentarii", commentarii);
    q.bindValue(":tip_discp", tip_discp);
    q.bindValue(":Ocenka", Ocenka);

    return q.exec();
}

bool Dal_studentsControl::GAKsEdit(int zap, int discp1, int discp2, int discp3, int discp4, QDate dates, QString kab, int nomerPrikaza, int GodFormKom)
{
    QSqlQuery q;
    q.prepare("UPDATE is_gak SET gak_date = :dates, gak_kabinet = :kab, gak_nomer_prikaza = :nomerPrikaza, gak_disceplina1 = :discp1, gak_disceplina2 = :discp2, gak_disceplina3 = :discp3, gak_disceplina4 = :discp4, komissia = :GodFormKom WHERE gak_id = " + QString::number(zap));
    q.bindValue(":dates", dates.toString("yyyy-MM-dd"));
    q.bindValue(":kab", kab);
    q.bindValue(":nomerPrikaza", nomerPrikaza);
    q.bindValue(":discp1", discp1);
    q.bindValue(":discp2", discp2);
    q.bindValue(":discp3", discp3);
    q.bindValue(":discp4", discp4);
    q.bindValue(":GodFormKom", GodFormKom);
    return q.exec();
}

QSqlQueryModel *Dal_studentsControl::getActVnedr(QString stud, QString group, QDate dates, QDate datesEnd)
{
    QString query="", condition="",filter="";
    QSqlQueryModel  *ActVnedrModelView = new QSqlQueryModel(this);
    if(stud!="" || group!="" || !dates.isNull())
        filter.append(" where ");
    if(stud!="")
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition.append(" ActVnedreniaView.`s_fio` like '%" + stud + "%'  ");
    }

    if(group!="")
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition.append(" ActVnedreniaView.`g_name` like '%" + group + "%'  ");
    }

    if(!dates.isNull())
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition.append(" ActVnedreniaView.`data` between '" + dates.toString("yyyy-MM-dd") + "' and '" + datesEnd.toString("yyyy-MM-dd 23:59:59") + "'");
    }
    query = "select * from ActVnedreniaView " + filter + condition;
    ActVnedrModelView->setQuery(query);
    ActVnedrModelView->setHeaderData(4,Qt::Horizontal,tr("ФИО"));
    ActVnedrModelView->setHeaderData(5,Qt::Horizontal,tr("Группа"));
    ActVnedrModelView->setHeaderData(6,Qt::Horizontal,tr("Номер акта"));
    ActVnedrModelView->setHeaderData(7,Qt::Horizontal,tr("Предприятие"));
    ActVnedrModelView->setHeaderData(8,Qt::Horizontal,tr("Дата"));
    ActVnedrModelView->setHeaderData(9,Qt::Horizontal,tr("Тема на русском"));
    ActVnedrModelView->setHeaderData(10,Qt::Horizontal,tr("Тема на кыргызском"));
    ActVnedrModelView->setHeaderData(11,Qt::Horizontal,tr("Дополнения"));
    return ActVnedrModelView;
}

QSqlQueryModel *Dal_studentsControl::getPracWork(QString stud, QString group, QDate dates, QDate datesEnd, int tip_prac)
{
    QString query="", condition="",filter="";
    QSqlQueryModel  *PracWorkModelView = new QSqlQueryModel(this);
    if(stud!="" || group!="" || !dates.isNull() || tip_prac!=0)
        filter.append(" where ");
    if(stud!="")
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition.append(" PracWorkView.`s_fio` like '%" + stud + "%'  ");
    }

    if(group!="")
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition.append(" PracWorkView.`g_name` like '%" + group + "%'  ");
    }

    if(!dates.isNull())
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition.append(" PracWorkView.`data` between '" + dates.toString("yyyy-MM-dd") + "' and '" + datesEnd.toString("yyyy-MM-dd 23:59:59") + "'");
    }

    if(tip_prac!=0)
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition.append(" PracWorkView.`pr_type` = " + QString::number(tip_prac));
    }
    query = "select * from PracWorkView " + filter + condition;
    PracWorkModelView->setQuery(query);
    PracWorkModelView->setHeaderData(5,Qt::Horizontal,tr("ФИО"));
    PracWorkModelView->setHeaderData(6,Qt::Horizontal,tr("Группа"));
    PracWorkModelView->setHeaderData(7,Qt::Horizontal,tr("Тип практики"));
    PracWorkModelView->setHeaderData(8,Qt::Horizontal,tr("Семестр"));
    PracWorkModelView->setHeaderData(9,Qt::Horizontal,tr("Руководитель от кафедры"));
    PracWorkModelView->setHeaderData(10,Qt::Horizontal,tr("Руководитель от предприятия"));
    PracWorkModelView->setHeaderData(11,Qt::Horizontal,tr("Место отбывания практики"));
    PracWorkModelView->setHeaderData(12,Qt::Horizontal,tr("Оценка"));
    PracWorkModelView->setHeaderData(13,Qt::Horizontal,tr("Отзыв"));
    PracWorkModelView->setHeaderData(14,Qt::Horizontal,tr("Номер приказа"));
    PracWorkModelView->setHeaderData(15,Qt::Horizontal,tr("Дата прохождения"));


    return PracWorkModelView;
}

QSqlQueryModel *Dal_studentsControl::getPracWorkReport(int specialnost_id, int group_id, QDate dateStart, QDate dateEnd, int tip_prac)
{
    QString query = "SELECT  * FROM PracWorkViewReport WHERE 1=1 ";
    QSqlQueryModel  *model = new QSqlQueryModel(this);
    if(specialnost_id!=0)
        query.append(" AND id_specialnosti = " + QString::number(specialnost_id));
    if(group_id!=0)
        query.append(" AND id_group = " + QString::number(group_id));
    if(!dateStart.isNull())
        query.append(" AND pr_year BETWEEN '" + dateStart.toString("yyyy-MM-dd") + "' AND '" + dateEnd.toString("yyyy-MM-dd 23:59:59") + "'");
    if(tip_prac!=0)
        query.append(" AND pr_type = " + QString::number(tip_prac));
    query.append(" ORDER BY id_specialnosti, id_group, s_fio");
    model->setQuery(query);
    model->setHeaderData(6,Qt::Horizontal,tr("Специальность"));
    model->setHeaderData(7,Qt::Horizontal,tr("Группа"));
    model->setHeaderData(8,Qt::Horizontal,tr("ФИО"));
    model->setHeaderData(9,Qt::Horizontal,tr("Тип практики"));
    model->setHeaderData(10,Qt::Horizontal,tr("Семестр"));
    model->setHeaderData(11,Qt::Horizontal,tr("Руков. от кафедры"));
    model->setHeaderData(12,Qt::Horizontal,tr("Руков. от предприятия"));
    model->setHeaderData(13,Qt::Horizontal,tr("Место отбывания практики"));
    model->setHeaderData(14,Qt::Horizontal,tr("Оценка"));
    model->setHeaderData(15,Qt::Horizontal,tr("Отзыв"));
    model->setHeaderData(16,Qt::Horizontal,tr("№ приказа"));
    model->setHeaderData(17,Qt::Horizontal,tr("Дата прохождения"));
    //qDebug()<<query;
    return model;
}

bool Dal_studentsControl::deleteActVned(int id_actvndr)
{
    QSqlQuery query;
    query.prepare("delete from is_akti_vnedreniya where id_akt_vnedreniya = :id_actvndr");
    query.bindValue(":id_actvndr", id_actvndr);
    query.exec();
    if (! query.isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

QSqlQuery *Dal_studentsControl::getCurrentCourseWork(int id_course_work)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select * from course_work_view where id_course_work = :id_course_work");
    query->bindValue(":id_course_work", id_course_work);
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

QSqlQuery *Dal_studentsControl::getCurrentTrudoVipusknik(int id_trudoVip)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select * from TrrudoUstrVipusknikovView where id_trudo_vipusknik = :id_trudoVip");
    query->bindValue(":id_trudoVip", id_trudoVip);
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

bool Dal_studentsControl::addCourseWork(int discipline_id, int student_id, int sotrudnik_id, QString tema, QString course_work_fail, int ocenka, QDate data_sdachi, int kolvo_listov)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("insert into is_course_work (discipline_id, student_id, sotrudnik_id, tema, course_work_fail, ocenka, data_sdachi, kolvo_listov)\
                   values (:discipline_id, :student_id, :sotrudnik_id, :tema, :course_work_fail, :ocenka, :data_sdachi, :kolvo_listov)");
                   query->bindValue(":discipline_id", discipline_id);
            query->bindValue(":student_id", student_id);
    query->bindValue(":sotrudnik_id", sotrudnik_id);
    query->bindValue(":tema", tema);
    query->bindValue(":course_work_fail", course_work_fail);
    query->bindValue(":ocenka", ocenka);
    query->bindValue(":data_sdachi", data_sdachi.toString("yyyy-MM-dd"));
    query->bindValue(":kolvo_listov", kolvo_listov);
    query->exec();
    qDebug()<<query->lastError()<<query->executedQuery();
    if (! query->isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_studentsControl::editCourseWork(int id_course_work, int discipline_id, int student_id, int sotrudnik_id, QString tema, QString course_work_fail, int ocenka, QDate data_sdachi, int kolvo_listov)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("UPDATE is_course_work SET discipline_id = :discipline_id, student_id = :student_id, sotrudnik_id = :sotrudnik_id, tema = :tema, course_work_fail = :course_work_fail, ocenka = :ocenka, data_sdachi = :data_sdachi, kolvo_listov = :kolvo_listov WHERE id_course_work = " + QString::number(id_course_work));
    query->bindValue(":discipline_id", discipline_id);
    query->bindValue(":student_id", student_id);
    query->bindValue(":sotrudnik_id", sotrudnik_id);
    query->bindValue(":tema", tema);
    query->bindValue(":course_work_fail", course_work_fail);
    query->bindValue(":ocenka", ocenka);
    query->bindValue(":data_sdachi", data_sdachi.toString("yyyy-MM-dd"));
    query->bindValue(":kolvo_listov", kolvo_listov);
    query->bindValue(":id_course_work", id_course_work);
    query->exec();
    if (! query->isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_studentsControl::editTrudoVipusknik(int trudo_id, int vip_id, QString AdrRab, QString ZanDolj, int RabZaGran)
{
    qDebug()<<trudo_id<<vip_id<<AdrRab<<RabZaGran<<ZanDolj;
    QSqlQuery *query = new QSqlQuery;
    query->prepare("UPDATE is_trudo_vipusknik SET vipusknik_id = :vip_id, Adres_raboti = :AdrRab, Viehali_zaGranicu = :RabZaGran, ZanimaemayaDoljnost = :ZanDolj WHERE id_trudo_vipusknik = " + QString::number(trudo_id));
    query->bindValue(":vip_id", vip_id);
    query->bindValue(":AdrRab", AdrRab);
    query->bindValue(":ZanDolj", ZanDolj);
    query->bindValue(":RabZaGran", RabZaGran);
    query->exec();
    if (! query->isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_studentsControl::deleteCourseWork(int id_course_work)
{
    QSqlQuery query;
    query.prepare("delete from is_course_work where id_course_work = :id_course_work");
    query.bindValue(":id_course_work", id_course_work);
    query.exec();
    if (! query.isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

QSqlQuery *Dal_studentsControl::getCurrentSrez(int id_srez)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select * from is_srez where id_srez = :id_srez");
    query->bindValue(":id_srez", id_srez);
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

bool Dal_studentsControl::addSrez(int group_id, QDate data_sreza, int discipline_1, int discipline_2, int discipline_3, int discipline_4, int discipline_5)
{
    QSqlQuery *query = new QSqlQuery;
    QString q1 = "insert into is_srez (group_id, data_sreza, discipline1";
    QString q2 = " values (:group_id, :data_sreza, :discipline_1";
    if(discipline_2!=0)
    {
        q1.append(", discipline2");
        q2.append(", :discipline_2");
    }
    if(discipline_3!=0)
    {
        q1.append(", discipline3");
        q2.append(", :discipline_3");
    }
    if(discipline_4!=0)
    {
        q1.append(", discipline4");
        q2.append(", :discipline_4");
    }
    if(discipline_5!=0)
    {
        q1.append(", discipline5");
        q2.append(", :discipline_5");
    }
    query->prepare(q1 + ") " + q2 + ")");
    query->bindValue(":group_id", group_id);
    query->bindValue(":data_sreza", data_sreza.toString("yyyy-MM-dd"));
    query->bindValue(":discipline_1", discipline_1);
    if(discipline_2 != 0)
        query->bindValue(":discipline_2", discipline_2);
    if(discipline_3 != 0)
        query->bindValue(":discipline_3", discipline_3);
    if(discipline_4 != 0)
        query->bindValue(":discipline_4", discipline_4);
    if(discipline_5 != 0)
        query->bindValue(":discipline_5", discipline_5);
    query->exec();
    if (! query->isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_studentsControl::editSrez(int id_srez, int group_id, QDate data_sreza, int discipline_1, int discipline_2, int discipline_3, int discipline_4, int discipline_5)
{
    QSqlQuery *query = new QSqlQuery;
    QString q1 = "UPDATE is_srez SET group_id = :group_id, data_sreza = :data_sreza, discipline1 = :discipline_1";
    //    query->prepare("UPDATE is_srez SET group_id = :group_id, data_sreza = :data_sreza, discipline1 = :discipline_1, discipline2 = :discipline_2, discipline3 = :discipline_3, discipline4 = :discipline_4, discipline5 = :discipline_5 WHERE id_srez = :id_srez");
    if(discipline_2 != 0)
        q1.append(", discipline2 = :discipline_2");
    if(discipline_3 != 0)
        q1.append(", discipline3 = :discipline_3");
    if(discipline_4 != 0)
        q1.append(", discipline4 = :discipline_4");
    if(discipline_5 != 0)
        q1.append(", discipline5 = :discipline_5");
    query->prepare(q1 +  " WHERE id_srez = :id_srez");
    query->bindValue(":group_id", group_id);
    query->bindValue(":data_sreza", data_sreza.toString("yyyy-MM-dd"));
    query->bindValue(":discipline_1", discipline_1);
    if(discipline_2 != 0)
        query->bindValue(":discipline_2", discipline_2);
    if(discipline_3 != 0)
        query->bindValue(":discipline_3", discipline_3);
    if(discipline_4 != 0)
        query->bindValue(":discipline_4", discipline_4);
    if(discipline_5 != 0)
        query->bindValue(":discipline_5", discipline_5);
    query->bindValue(":id_srez", id_srez);
    query->exec();
    qDebug()<<query->lastError()<<query->executedQuery();
    if (! query->isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_studentsControl::deleteSrez(int id_srez)
{
    QSqlQuery query;
    query.prepare("delete from is_srez where id_srez = :id_srez");
    query.bindValue(":id_srez", id_srez);
    query.exec();
    if (! query.isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_studentsControl::addSrezBally(int student_id, int discipline_1, int discipline_2, int discipline_3, int discipline_4, int discipline_5, int srez_id)
{
    QSqlQuery *query = new QSqlQuery;
    QString q1 = "insert into is_srez_bally (student_id, srez_id, ocenka1";
    QString q2 = " values (:student_id, :srez_id, :discipline_1";
    if(discipline_2!=0)
    {
        q1.append(", ocenka2");
        q2.append(", :discipline_2");
    }
    if(discipline_3!=0)
    {
        q1.append(", ocenka3");
        q2.append(", :discipline_3");
    }
    if(discipline_4!=0)
    {
        q1.append(", ocenka4");
        q2.append(", :discipline_4");
    }
    if(discipline_5!=0)
    {
        q1.append(", ocenka5");
        q2.append(", :discipline_5");
    }
    query->prepare(q1 + ") " + q2 + ")");
    query->bindValue(":student_id", student_id);
    query->bindValue(":srez_id", srez_id);
    query->bindValue(":discipline_1", discipline_1);
    if(discipline_2 != 0)
        query->bindValue(":discipline_2", discipline_2);
    if(discipline_3 != 0)
        query->bindValue(":discipline_3", discipline_3);
    if(discipline_4 != 0)
        query->bindValue(":discipline_4", discipline_4);
    if(discipline_5 != 0)
        query->bindValue(":discipline_5", discipline_5);
    query->exec();
    if (! query->isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_studentsControl::addGakBally(int student_id, int discipline_1, int discipline_2, int discipline_3, int discipline_4, bool nedopusk, int gak_id, int group_id)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("insert into is_gak_bally (student_id, gak_id, discip1, discip2, discip3, discip4, nedopusk, group_id) values(:student_id, :gak_id, :discip1, :discip2, :discip3, :discip4, :nedopusk, :group_id)");
    query->bindValue(":student_id", student_id);
    query->bindValue(":gak_id", gak_id);
    query->bindValue(":discip1", discipline_1);
    query->bindValue(":discip2", discipline_2);
    query->bindValue(":discip3", discipline_3);
    query->bindValue(":discip4", discipline_4);
    query->bindValue(":nedopusk", nedopusk);
    query->bindValue(":group_id", group_id);
    query->exec();
    if (! query->isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}


