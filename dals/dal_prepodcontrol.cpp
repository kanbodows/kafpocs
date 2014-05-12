#include "dal_prepodcontrol.h"
#include <QMessageBox>
#include <QDebug>
#include <QDialog>
Dal_prepodcontrol::Dal_prepodcontrol(QObject *parent)
{    
}

QSqlQueryModel *Dal_prepodcontrol::getStatia(QString avtor, QString soavtor, QString tema)
{
    QString query="", condition="",filter="";
    QSqlQueryModel static *StatiaModelView = new QSqlQueryModel(this);
    if(avtor!="" || soavtor!="" || tema!="")
        filter.append(" where ");
    if(avtor!="")
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition.append(" Statia_view.`Avtor` like '%" + avtor + "%'  ");
    }

    if(soavtor!="")
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition.append(" Statia_view.`SoAvtor` like '%" + soavtor + "%'  ");
    }

    if(tema!="")
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition.append(" Statia_view.`statya_tema` like '%" + tema + "%'  ");
    }

    query = "select * from Statia_view " + filter + condition;
    StatiaModelView->setQuery(query);
    return StatiaModelView;
}

bool Dal_prepodcontrol::deleteStatia(int id_statia)
{
    QSqlQuery query;
    query.prepare("delete from is_statya where statya_id = :id_statia");
    query.bindValue(":id_statia", id_statia);
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


QSqlQueryModel *Dal_prepodcontrol::getComboAvtor()
{
    QSqlQueryModel static *comboAvtorModel = new QSqlQueryModel(this);
    comboAvtorModel->setQuery("select * from is_sotrudniki");
    return comboAvtorModel;
}

QSqlQueryModel *Dal_prepodcontrol::getcomboPrepod()
{
    QSqlQueryModel *comboPrepodModel = new QSqlQueryModel(this);
    comboPrepodModel->setQuery("select * from sotr_kafedri");
    return comboPrepodModel;
}

QSqlQueryModel *Dal_prepodcontrol::getcomboPredm()
{
    QSqlQueryModel *comboPredmModel = new QSqlQueryModel(this);
    comboPredmModel->setQuery("select * from is_discipline  group by is_discipline.d_name order by is_discipline.d_name asc");
    return comboPredmModel;
}

QSqlQueryModel *Dal_prepodcontrol::getcomboSemmestr()
{
    QSqlQueryModel *comboSemmestrModel = new QSqlQueryModel(this);
    comboSemmestrModel->setQuery("select * from is_semmestr  order by is_semmestr.id_semestr asc ;");
    return comboSemmestrModel;
}

QSqlQueryModel *Dal_prepodcontrol::getcomboKafedry()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("SELECT * FROM is_kafedry");
    return model;
}

QSqlQueryModel *Dal_prepodcontrol::getPlanUmr(QString goda)
{
    QString query = "";
    QSqlQueryModel *PlanUmrmodel = new QSqlQueryModel(this);
    query = "SELECT * FROM planumr_view where 1=1";
    if(goda!="")
        query.append(" and god = " + goda);
    PlanUmrmodel->setQuery(query);
    PlanUmrmodel->setHeaderData(1,Qt::Horizontal,tr("ФИО авторов"));
    PlanUmrmodel->setHeaderData(2,Qt::Horizontal,tr("Наименование\n учебно-методических\n работ с указанием направления"));
    PlanUmrmodel->setHeaderData(3,Qt::Horizontal,tr("Аннотация"));
    PlanUmrmodel->setHeaderData(4,Qt::Horizontal,tr("Объем в учеб.\n-изд. листах"));
    PlanUmrmodel->setHeaderData(5,Qt::Horizontal,tr("Тираж"));
    PlanUmrmodel->setHeaderData(6,Qt::Horizontal,tr("Срок пред-ния\n в ОП ИЦ «Техник»"));
    return PlanUmrmodel;
}

QSqlQueryModel *Dal_prepodcontrol::getComboSoAvtor(int id_avt)
{
    QSqlQueryModel static *comboAvtorModel = new QSqlQueryModel(this);
    comboAvtorModel->setQuery("select * from is_sotrudniki where id_sotr <> " + QString::number(id_avt));
    return comboAvtorModel;
}

bool Dal_prepodcontrol::getStatiaAdd(int avtors_id, int soAvtors_id, QString temas,QDate dates)
{
    QSqlQuery *query = new QSqlQuery;
    if(soAvtors_id!=0)
    {
        query->prepare("insert into is_statya (statya_avtor, statya_soavtor, statya_tema,statya_date) values (:avtors_id, :soAvtors_id, :temas, :dates)");
        query->bindValue(":avtors_id", avtors_id);
        query->bindValue(":soAvtors_id", soAvtors_id);
        query->bindValue(":temas", temas);
        query->bindValue(":dates", dates.toString("yyyy-MM-dd"));
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
    else
        query->prepare("insert into is_statya (statya_avtor, statya_tema,statya_date) values (:avtors_id, :temas, :dates)");
    query->bindValue(":avtors_id", avtors_id);
    query->bindValue(":temas", temas);
    query->bindValue(":dates", dates.toString("yyyy-MM-dd"));
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

bool Dal_prepodcontrol::addPlanUMR(QString ispoln, QString UMR, QString anotac, double kolvoStr, int tiraj, QString srokIspoln, QString gods)
{
    qDebug()<<"ispol"<<ispoln<<"UMR"<<UMR<<anotac<<kolvoStr<<tiraj<<srokIspoln<<gods;
    QSqlQuery *query = new QSqlQuery;

    query->prepare("insert into is_planumr (naimMetRabISpec, anotacia, kolvoListov, tiraj, srokPredostavlOPiC, god, sotrudFio)\
                                               values (:UMR, :anotac, :kolvoStr, :tiraj, :srokIspoln, :gods , :ispoln)");
                   query->bindValue(":UMR", UMR);
            query->bindValue(":anotac", anotac);
    query->bindValue(":kolvoStr", kolvoStr);
    query->bindValue(":tiraj", tiraj);
    query->bindValue(":srokIspoln", srokIspoln);
    query->bindValue(":gods", gods);
    query->bindValue(":ispoln", ispoln);
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

bool Dal_prepodcontrol::PublicationNirAdd(int pub_id, QString nameF, QString Urlf, QString MejStandartNomer, QString NazvaniePublikacii, QString NazvanieJurnala, QString NomerVipuskaJurnala, QDate GodVipuska)
{
    QSqlQuery *query = new QSqlQuery;

    query->prepare("insert into is_publications (pub_nir, pub_file, pub_url, mejdunarodStandartNomer, nazvaniePublikacii, nazvanieJurnala, NomerVipuskaJurnala, GodVipuska)\
                   values (:pub_id, :nameF, :Urlf, :MejStandartNomer, :NazvaniePublikacii, :NazvanieJurnala, :NomerVipuskaJurnala, :GodVipuska)");
                   query->bindValue(":pub_id", pub_id);
            query->bindValue(":nameF", nameF);
    query->bindValue(":Urlf", Urlf);
    query->bindValue(":MejStandartNomer", MejStandartNomer);
    query->bindValue(":NazvaniePublikacii", NazvaniePublikacii);
    query->bindValue(":NazvanieJurnala", NazvanieJurnala);
    query->bindValue(":NomerVipuskaJurnala", NomerVipuskaJurnala);
    query->bindValue(":GodVipuska", GodVipuska.toString("yyyy-MM-dd"));
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

bool Dal_prepodcontrol::sostavKomAdd(int pred, int secr, int ch1, int ch2, int ch3, int ch4, int ch5, QDate dates, int nomerRaporta)
{
    QSqlQuery *query = new QSqlQuery;

    query->prepare("insert into is_sostav_gak (predsed, secretar, chlen1, chlen2, chlen3, chlen4, chlen5, date, n_raport)\
                   values (:pred, :secr, :ch1, :ch2, :ch3, :ch4, :ch5, :dates, :nomerRaporta)");
                   query->bindValue(":pred", pred);
            query->bindValue(":secr", secr);
    query->bindValue(":ch1", ch1);
    query->bindValue(":ch2", ch2);
    query->bindValue(":ch3", ch3);
    query->bindValue(":ch4", ch4);
    query->bindValue(":ch5", ch5);
    query->bindValue(":dates", dates.toString("yyyy-MM-dd"));
    query->bindValue(":nomerRaporta", nomerRaporta);
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

bool Dal_prepodcontrol::SotrudnikAdd(QString FIO, QString telephon, QString mails, QString obrazovanie, int staj_obsh, int stajVKGTU, int ped_staj, QString uchZvan, int dolj, double stavka, QString logins, QString passw, QDate dateRojd, QDate dateReg, int status, QString MestoRojd, QString mestoRaboti, QString nacion, QString SeriaPasport, QString addres, QString semPoloj, QString socZash, int OnlyGak, QString sostav)
{
    //qDebug()<<FIO<<telephon<<mails<<obrazovanie<<staj_obsh<<stajVKGTU<<ped_staj<<uchZvan<<dolj<<stavka<<logins<<passw<<dateRojd<<dateReg<<status<<MestoRojd<<mestoRaboti<<nacion<<SeriaPasport<<addres<<semPoloj<<socZash<<OnlyGak;
    QSqlQuery *query = new QSqlQuery;
    query->prepare("insert into is_sotrudniki (st_fio, st_mob_tel, st_email, st_obrazovanie, st_staj_obchiy, st_staj_v_kgtu, st_staj_ped, st_uch_zvanie, st_doljnost, st_stavka, st_login, st_passwd, st_date_birth, st_date_reg, st_status, st_mesto_rojdenia, st_mesto_raboty, st_nacionalnost, st_pasport_seria__, st_adress, st_semeinoe_poloj, st___soc_zashity, st_only_gak, st_sostav)\
                   values (:FIO, :telephon, :mails, :obrazovanie, :staj_obsh, :stajVKGTU, :ped_staj, :uchZvan, :dolj, :stavka, :logins, :passw, :dateRojd, :dateReg, :status, :MestoRojd , :mestoRaboti, :nacion , :SeriaPasport, :addres, :semPoloj, :socZash, :OnlyGak, :sostav)");
                   query->bindValue(":FIO", FIO);
            query->bindValue(":telephon", telephon);
    query->bindValue(":mails", mails);
    query->bindValue(":obrazovanie", obrazovanie);
    query->bindValue(":staj_obsh", staj_obsh);
    query->bindValue(":stajVKGTU", stajVKGTU);
    query->bindValue(":ped_staj", ped_staj);
    query->bindValue(":uchZvan", uchZvan);
    query->bindValue(":dolj", dolj);
    query->bindValue(":stavka", stavka);
    query->bindValue(":logins", logins);
    query->bindValue(":passw", passw);
    query->bindValue(":dateRojd", dateRojd.toString("yyyy-MM-dd"));
    query->bindValue(":dateReg", dateReg.toString("yyyy-MM-dd"));
    query->bindValue(":status", status);
    query->bindValue(":MestoRojd", MestoRojd);
    query->bindValue(":mestoRaboti", mestoRaboti);
    query->bindValue(":nacion", nacion);
    query->bindValue(":SeriaPasport", SeriaPasport);
    query->bindValue(":addres", addres);
    query->bindValue(":semPoloj", semPoloj);
    query->bindValue(":socZash", socZash);
    query->bindValue(":OnlyGak", OnlyGak);
    query->bindValue(":sostav", sostav);
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

bool Dal_prepodcontrol::getSubWorkAdd(int prepod, int predm, int semes, QDate dates)
{
    QSqlQuery *query = new QSqlQuery;

    query->prepare("insert into is_subj_work (sbj_discipline, sbj_semestr, sbj_year, sbj_prepod) values (:predm, :semes, :dates, :prepod )");
    query->bindValue(":predm", predm);
    query->bindValue(":semes", semes);
    query->bindValue(":dates", dates.toString("yyyy-MM-dd"));
    query->bindValue(":prepod", prepod);
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

QSqlQuery *Dal_prepodcontrol::getCurrentStatia(int id_stati)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select Statia_view.statya_id, Statia_view.statya_avtor,Statia_view.statya_soavtor, Statia_view.statya_tema, Statia_view.statya_date from Statia_view where Statia_view.statya_id = :id_stati");
    query->bindValue(":id_stati", id_stati);
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

QSqlQuery *Dal_prepodcontrol::getCurrentSubWork(int id_sub_work)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select * from subj_workView where subj_workView.id_subj_work = :id_sub_work");
    query->bindValue(":id_sub_work", id_sub_work);
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

QSqlQuery *Dal_prepodcontrol::getCurrentSotrudnik(int rec_id)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select * from Sotr_view where Sotr_view.id_sotr = :rec_id");
    query->bindValue(":rec_id", rec_id);
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

QSqlQuery *Dal_prepodcontrol::getCurrentDiscipline(int id_discipline)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select * from is_discipline where id_discipline = :id_discipline");
    query->bindValue(":id_discipline", id_discipline);
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

QSqlQuery *Dal_prepodcontrol::getCurrentPublication(int id_publ)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select * from is_publications where id_public = :id_publ");
    query->bindValue(":id_publ", id_publ);
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

QSqlQuery *Dal_prepodcontrol::getCurrentplanUmr(int zap_id)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select * from planumr_view where id_planUMR = :zap_id");
    query->bindValue(":zap_id", zap_id);
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

QSqlQuery *Dal_prepodcontrol::getCurrentMetodichka(int id_metodichka)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select * from is_metodichki where id_met = :id_met");
    query->bindValue(":id_met", id_metodichka);
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

QSqlQuery *Dal_prepodcontrol::getCurrentplanzasKaf(int zap)
{
    QSqlQuery *query2 = new QSqlQuery;
    query2->prepare("select * from is_prosmotrPlana where id_prosmotrPlana= :zap");
    query2->bindValue(":zap", zap);
    query2->exec();
    if (! query2->isActive())
    {
        return NULL;
    }
    query2->first();
    return query2;
}

QSqlQuery *Dal_prepodcontrol::updateGalochkaInProsmotrplana(int ids_prosmPlana)
{
    QSqlQuery *q = new QSqlQuery;;
    q->prepare("UPDATE is_prosmotrPlana SET OtmetkaOVipoln = 1 WHERE id_prosmotrPlana = " + QString::number(ids_prosmPlana));
    q->bindValue(":ids_prosmPlana", ids_prosmPlana);
    q->exec();
    if (! q->isActive())
    {
        return NULL;
    }
    q->first();
    return q;
}

bool Dal_prepodcontrol::editStat(int id_stat, int statAvtor_id, int statSoAvtor_id, QString stat_tema, QDate stat_date)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("call updStatya(:id_stat,:statAvtor_id, :statSoAvtor_id, :stat_tema, :stat_date, @result)");
    query->bindValue(":id_stat", id_stat);
    query->bindValue(":statAvtor_id", statAvtor_id);
    query->bindValue(":statSoAvtor_id", statSoAvtor_id);
    query->bindValue(":stat_tema", stat_tema);
    query->bindValue(":stat_date", stat_date.toString("yyyy-MM-dd"));
    query->exec();
    query->exec(" select @result");
    query->first();
    int result = query->value(0).toInt();
    if(result == 1)
    {
        return false;

    }
    else
        return true;

}

bool Dal_prepodcontrol::EditPlanUMR(int id_zap, QString ispoln, QString UMR, QString anotac, double kolvoStr, int tiraj, QString srokIspoln, QString gods)
{
    QSqlQuery q;
    if(ispoln!="")
    {
    q.prepare("UPDATE is_planumr SET naimMetRabISpec = :UMR, anotacia = :anotac,kolvoListov = :kolvoStr, tiraj = :tiraj, srokPredostavlOPiC = :srokIspoln, god = :gods, sotrudFio = :ispoln WHERE id_planUMR = " + QString::number(id_zap));
    q.bindValue(":UMR", UMR);
    q.bindValue(":anotac", anotac);
    q.bindValue(":kolvoStr", kolvoStr);
    q.bindValue(":tiraj", tiraj);
    q.bindValue(":srokIspoln", srokIspoln);
    q.bindValue(":gods", gods);
    q.bindValue(":ispoln", ispoln);
    return q.exec();
    }
    else
    {
        q.prepare("UPDATE is_planumr SET naimMetRabISpec = :UMR, anotacia = :anotac,kolvoListov = :kolvoStr, tiraj = :tiraj, srokPredostavlOPiC = :srokIspoln, god = :gods WHERE id_planUMR = " + QString::number(id_zap));
        q.bindValue(":UMR", UMR);
        q.bindValue(":anotac", anotac);
        q.bindValue(":kolvoStr", kolvoStr);
        q.bindValue(":tiraj", tiraj);
        q.bindValue(":srokIspoln", srokIspoln);
        q.bindValue(":gods", gods);
        return q.exec();
    }

}

bool Dal_prepodcontrol::editSubWork(int subwork_id, int predm, int semes, QDate dates, int prepod)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("call updSubWork(:subwork_id, :predm, :semes, :dates, :prepod, @result)");
    query->bindValue(":subwork_id", subwork_id);
    query->bindValue(":predm", predm);
    query->bindValue(":semes", semes);
    query->bindValue(":dates", dates.toString("yyyy-MM-dd"));
    query->bindValue(":prepod", prepod);
    query->exec();
    query->exec(" select @result");
    query->first();
    int result = query->value(0).toInt();
    if(result == 1)
    {
        return false;

    }
    else
        return true;
}

bool Dal_prepodcontrol::editParolSotr(int recor_id, QString newParol)
{
    QSqlQuery q;
    q.prepare("UPDATE is_sotrudniki SET st_passwd = :newParol WHERE id_sotr = " + QString::number(recor_id));
    q.bindValue(":newParol", newParol);
    return q.exec();
}

QSqlQueryModel *Dal_prepodcontrol::getSubjWork(QString FIO, QString discipl)
{
    QString query="", condition="",filter="";
    QSqlQueryModel  *SubWorkModelView = new QSqlQueryModel(this);
    if(FIO!="" || discipl!="")
        filter.append(" where ");
    if(FIO!="")
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition.append(" subj_workView.`s_fio` like '%" + FIO + "%'  ");
    }

    if(discipl!="")
    {
        if (condition != "")
        {
            condition.append(" and ");
        }
        condition.append(" subj_workView.`d_name` like '%" + discipl + "%'  ");
    }

    query = "select * from subj_workView " + filter + condition;
    SubWorkModelView->setQuery(query);
    SubWorkModelView->setHeaderData(4,Qt::Horizontal,tr("ФИО"));
    SubWorkModelView->setHeaderData(5,Qt::Horizontal,tr("Дисциплина"));
    SubWorkModelView->setHeaderData(6,Qt::Horizontal,tr("Семестр"));
    SubWorkModelView->setHeaderData(7,Qt::Horizontal,tr("Дата"));

    return SubWorkModelView;
}

bool Dal_prepodcontrol::deleteSubWork(int subWork_id)
{
    QSqlQuery query;
    query.prepare("delete from is_subj_work where id_subj_work = :subWork_id");
    query.bindValue(":subWork_id", subWork_id);
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

bool Dal_prepodcontrol::deleteSotr(int sotr_id)
{
    QSqlQuery query;
    query.prepare("delete from is_sotrudniki where id_sotr = :sotr_id");
    query.bindValue(":sotr_id", sotr_id);
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

bool Dal_prepodcontrol::deleteZapPlanUmr(int id_zap)
{
    QSqlQuery query;
    query.prepare("delete from is_planumr where id_planumr = :id_zap");
    query.bindValue(":id_zap", id_zap);
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

QSqlQueryModel *Dal_prepodcontrol::getMetodichka(int discipl, int type_met)
{
    qDebug()<<discipl<<type_met;
    QString query="";
    QSqlQueryModel *MetodichModel = new QSqlQueryModel(this);
    query.append("select * from MetodickViews where 1=1");
    if(discipl!=0)
        query.append(" and id_discipline = " + QString::number(discipl));
    if(type_met!=0)
        query.append(" and id_met_type = " + QString::number(type_met));
    qDebug()<<query;
    MetodichModel->setQuery(query);
    MetodichModel->setHeaderData(4,Qt::Horizontal,tr("Дисциплина"));
    MetodichModel->setHeaderData(5,Qt::Horizontal,tr("Название методички"));
    MetodichModel->setHeaderData(6,Qt::Horizontal,tr("Автор с кафедры"));
    MetodichModel->setHeaderData(7,Qt::Horizontal,tr("Автор"));
    MetodichModel->setHeaderData(8,Qt::Horizontal,tr("Год"));
    MetodichModel->setHeaderData(9,Qt::Horizontal,tr("Тип методички"));
    return MetodichModel;
}

QSqlQueryModel *Dal_prepodcontrol::getMetodichki(int id_discipline)
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("select * from metodichki_view  where met_subject = " + QString::number(id_discipline));
    model->setHeaderData(1,Qt::Horizontal,tr("Название"));
    model->setHeaderData(2,Qt::Horizontal,tr("Автор"));
    model->setHeaderData(3,Qt::Horizontal,tr("Файл"));
    model->setHeaderData(4,Qt::Horizontal,tr("Год"));
    model->setHeaderData(5,Qt::Horizontal,tr("Тип"));
    return model;
}

QSqlQueryModel *Dal_prepodcontrol::getKnigi(int id_discipline)
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("select * from is_knigi  where discipline_id = " + QString::number(id_discipline));
    model->setHeaderData(1,Qt::Horizontal,tr("Название"));
    model->setHeaderData(2,Qt::Horizontal,tr("Автор"));
    model->setHeaderData(3,Qt::Horizontal,tr("Год"));
    model->setHeaderData(4,Qt::Horizontal,tr("Издательство"));
    model->setHeaderData(5,Qt::Horizontal,tr("Файл"));
    return model;
}

QSqlQueryModel *Dal_prepodcontrol::getMetTypes()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("select * from is_met_types");
    return model;
}

QSqlQueryModel *Dal_prepodcontrol::getDoljnost()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("select * from is_doljnost");
    return model;
}

QSqlQueryModel *Dal_prepodcontrol::getComboPubNir()
{
    QSqlQueryModel *ComboPubNirmodel = new QSqlQueryModel(this);
    ComboPubNirmodel->setQuery("select * from is_nir_sotr");
    return ComboPubNirmodel;
}

QSqlQueryModel *Dal_prepodcontrol::getKniga(QString namess, int discip)
{
    QString query = "";
    QSqlQueryModel *KnigaModel = new QSqlQueryModel(this);
    query.append("select * from book_view where 1 = 1");
    if(namess!="")
        query.append(" and nazvanie like '%" + namess + "%'");
    if(discip!=0)
        query.append(" and id_discipline = " + QString::number(discip));
    KnigaModel->setQuery(query);
    KnigaModel->setHeaderData(2,Qt::Horizontal,tr("Дисциплина"));
    KnigaModel->setHeaderData(4,Qt::Horizontal,tr("Автор с кафедры"));
    KnigaModel->setHeaderData(5,Qt::Horizontal,tr("Назвние книги"));
    KnigaModel->setHeaderData(6,Qt::Horizontal,tr("Автор"));
    KnigaModel->setHeaderData(7,Qt::Horizontal,tr("Год"));
    KnigaModel->setHeaderData(8,Qt::Horizontal,tr("Издательство"));
    KnigaModel->setHeaderData(9,Qt::Horizontal,tr("Файл"));
    return KnigaModel;
}

QSqlQueryModel *Dal_prepodcontrol::getDisciplines()
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("select id_discipline, d_name, d_semestr from is_discipline");
    return model;
}

QSqlQueryModel *Dal_prepodcontrol::getDisciplines(QString name, int semestr_id)
{
    QString query = "SELECT * FROM discipline_view WHERE 1=1 ";
    if(!name.isEmpty())
        query.append(" AND d_name like '%" + name + "%'");
    if(semestr_id != 0)
        query.append(" AND d_semestr = " + QString::number(semestr_id));
    query.append(" ORDER BY semestr_name");
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery(query);
    model->setHeaderData(1,Qt::Horizontal,tr("Дисциплина"));
    model->setHeaderData(3,Qt::Horizontal,tr("Семестр"));
    model->setHeaderData(4,Qt::Horizontal,tr("Конт-е точки"));
    model->setHeaderData(5,Qt::Horizontal,tr("Варианты"));
    model->setHeaderData(6,Qt::Horizontal,tr("Машинная"));
    model->setHeaderData(7,Qt::Horizontal,tr("Бланочная"));
    model->setHeaderData(8,Qt::Horizontal,tr("Устная"));
    model->setHeaderData(9,Qt::Horizontal,tr("Письменная"));
    model->setHeaderData(10,Qt::Horizontal,tr("Автор тестов"));
    return model;
}

QSqlQueryModel *Dal_prepodcontrol::getPublication(QString numberNir, QString naimFaila)
{
    QString query = "";
    QSqlQueryModel *PublicationModel = new QSqlQueryModel(this);
    query = "select * from publication_view where 1 = 1";
    if(numberNir!="")
        query.append(" and nir_sotr_nomer_proekta = " + numberNir);
    if(naimFaila!="")
        query.append(" and pub_file like '%" + naimFaila + "%'");

    PublicationModel->setQuery(query);
    PublicationModel->setHeaderData(2,Qt::Horizontal,tr("Номер проекта НИР"));
    PublicationModel->setHeaderData(3,Qt::Horizontal,tr("Наименование файла"));
    PublicationModel->setHeaderData(5,Qt::Horizontal,tr("Международный\n стандартный номер"));
    PublicationModel->setHeaderData(6,Qt::Horizontal,tr("Название публикации"));
    PublicationModel->setHeaderData(7,Qt::Horizontal,tr("Название журнала"));
    PublicationModel->setHeaderData(8,Qt::Horizontal,tr("Номер выпуска журнала"));
    PublicationModel->setHeaderData(9,Qt::Horizontal,tr("Год выпуска"));

    return PublicationModel;
}

int Dal_prepodcontrol::addDiscipline(QString nazv, int semestr_id, int tochki, QString varianty, int mashin, int blanoch, int ust, int pism, int sotr_id, int kafedra_id, int specialnost_id, QString tip)
{
    QSqlQuery q;
    q.prepare("INSERT INTO is_discipline (d_name, d_semestr, tochki, varianty, mashin, blanoch, ust, pism, sotr_id, kafedra_id, specialnost_id, tip) \
              VALUES (:nazv, :semestr_id, :tochki, :varianty, :mashin, :blanoch, :ust, :pism, :sotr_id, :kafedra_id, :specialnost_id, :tip)");
              q.bindValue(":nazv", nazv);
            q.bindValue(":semestr_id", semestr_id);
    q.bindValue(":tochki", tochki);
    q.bindValue(":varianty", varianty);
    q.bindValue(":mashin", mashin);
    q.bindValue(":blanoch", blanoch);
    q.bindValue(":ust", ust);
    q.bindValue(":pism", pism);
    q.bindValue(":sotr_id", sotr_id);
    q.bindValue(":kafedra_id", kafedra_id);
    q.bindValue(":specialnost_id", specialnost_id);
    q.bindValue(":tip", tip);
    if(q.exec())
        return q.lastInsertId().toInt();
    else
        return -1;
}

bool Dal_prepodcontrol::editDiscipline(int discipline_id, QString nazv, int semestr_id, int tochki, QString varianty, int mashin, int blanoch, int ust, int pism, int sotr_id, int kafedra_id, int specialnost_id, QString tip)
{
    QSqlQuery q;
    q.prepare("UPDATE is_discipline SET d_name = :nazv, d_semestr = :semestr_id, tochki = :tochki, varianty = :varianty, mashin = :mashin, blanoch = :blanoch, ust = :ust, pism = :pism, sotr_id = :sotr_id, kafedra_id = :kafedra_id, specialnost_id = :specialnost_id, tip = :tip"
              " WHERE id_discipline = " + QString::number(discipline_id));
    q.bindValue(":nazv", nazv);
    q.bindValue(":semestr_id", semestr_id);
    q.bindValue(":discipline_id", discipline_id);
    q.bindValue(":tochki", tochki);
    q.bindValue(":varianty", varianty);
    q.bindValue(":mashin", mashin);
    q.bindValue(":blanoch", blanoch);
    q.bindValue(":ust", ust);
    q.bindValue(":pism", pism);
    q.bindValue(":sotr_id", sotr_id);
    q.bindValue(":kafedra_id", kafedra_id);
    q.bindValue(":specialnost_id", specialnost_id);
    q.bindValue(":tip", tip);
    return q.exec();//qDebug()<<q.lastError();
}

bool Dal_prepodcontrol::addPlanZasKaf(QString Vopros, QString Ispolnit, QString SrokiIspoln, QString GodaS, QString GodaPo)
{
    QSqlQuery *qu = new QSqlQuery;
    qu->prepare("insert into is_prosmotrPlana (VoprosiZased, ispolnit, SrokiIspoln,God_s, God_po) \
                VALUES (:Vopros,:Ispolnit, :SrokiIspoln, :GodaS, :GodaPo)");
                qu->bindValue(":Vopros", Vopros); //.toString("yyyy-MM-dd")
            qu->bindValue(":Ispolnit", Ispolnit);
    qu->bindValue(":SrokiIspoln", SrokiIspoln);
    qu->bindValue(":GodaS", GodaS);
    qu->bindValue(":GodaPo", GodaPo);
    qu->exec();
    if (!qu->isActive())
    {
        return false;
    }
    else
    {
        return true;
    }

    //    QString Gods = "";
    //    Gods.append(GodaS + GodaPo);
    //    qDebug()<<Gods;
    //    QSqlQuery *qu2 = new QSqlQuery;
    //    qu2->prepare("insert into is_GodSozdPlanaKaf (God, God_s, God_po) \
    //                VALUES (:Gods,:GodaS, :GodaPo)");
    //    qu2->bindValue(":Gods", Gods); //.toString("yyyy-MM-dd")
    //    qu2->bindValue(":GodaS", GodaS);
    //    qu2->bindValue(":GodaPo", GodaPo);
    //    qu2->exec();
    //    if (!qu2->isActive())
    //    {
    //        return false;
    //    }
    //    else
    //    {
    //        return true;
    //    }
}

bool Dal_prepodcontrol::addSozdPlanKaf(QString gods1, QString godsS, QString godsPo)
{
    QSqlQuery *qu2 = new QSqlQuery;
    qu2->prepare("insert into is_GodSozdPlanaKaf (God, God_s, God_po) \
                 VALUES (:gods1,:godsS, :godsPo)");
                 qu2->bindValue(":gods1", gods1); //.toString("yyyy-MM-dd")
            qu2->bindValue(":godsS", godsS);
    qu2->bindValue(":godsPo", godsPo);
    qu2->exec();
    if (!qu2->isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_prepodcontrol::addSozdGrafKaf(QString gods1, QString godsS, QString godsPo)
{
    QSqlQuery *qu2 = new QSqlQuery;
    qu2->prepare("insert into is_GodSozdGrafika (God, God_s, God_po) \
                 VALUES (:gods1,:godsS, :godsPo)");
                 qu2->bindValue(":gods1", gods1); //.toString("yyyy-MM-dd")
            qu2->bindValue(":godsS", godsS);
    qu2->bindValue(":godsPo", godsPo);
    qu2->exec();
    if (!qu2->isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_prepodcontrol::addGrafikVzaimopoa(int FioPrepod, QString datapos, int FIOProv, QString NachGod, QString KonecGoda)
{
    QSqlQuery *qu2 = new QSqlQuery;
    qu2->prepare("insert into is_GrafikVzaimoposesh (FIO_prepod, DataPosesh, FIO_prover, NachaloGoda, KoneccGoda) \
                 VALUES (:FioPrepod,:datapos, :FIOProv, :NachGod, :KonecGoda)");
                 qu2->bindValue(":FioPrepod", FioPrepod); //.toString("yyyy-MM-dd")
            qu2->bindValue(":datapos", datapos);
    qu2->bindValue(":FIOProv", FIOProv);
    qu2->bindValue(":NachGod", NachGod);
    qu2->bindValue(":KonecGoda", KonecGoda);
    qu2->exec();
    if (!qu2->isActive())
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_prepodcontrol::deleteDiscipline(int id_discipline)
{
    QSqlQuery query;
    query.prepare("DELETE FROM is_discipline WHERE id_discipline = :id_discipline");
    query.bindValue(":id_discipline", id_discipline);
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

bool Dal_prepodcontrol::addMetodichka(int discipline_id, QString nazv, QString avtor, QString file, QDate god, int metType_id, int sotr_id, QString URLfiles)
{
    qDebug()<<discipline_id<<nazv<<avtor<<file<<god<<metType_id<<sotr_id<<URLfiles;
    QSqlQuery *q = new QSqlQuery;
    q->prepare("call AddMetod(:discipline_id, :nazv, :avtor, :file, :god, :metType_id, :sotr_id, :URLfiles, @result)");
    q->bindValue(":discipline_id", discipline_id);
    q->bindValue(":nazv", nazv);
    q->bindValue(":avtor", avtor);
    q->bindValue(":file", file);
    q->bindValue(":god", god.toString("yyyy-MM-dd"));
    q->bindValue(":metType_id", metType_id);
    q->bindValue(":sotr_id", sotr_id);
    q->bindValue(":URLfiles", URLfiles);
    q->exec();
    q->exec(" select @result");
    q->first();
    int result = q->value(0).toInt();
    if(result == 1)
    {
        return false;

    }
    else
        return true;

}

bool Dal_prepodcontrol::editMetodichka(int id_metodichka, int discipline_id, QString nazv, QString avtor, QString file, QDate god, int metType_id, int sotr_id, QString URLfiles)
{
    QString query = "UPDATE is_metodichki SET met_subject = :discipline_id, met_nazvanie = :nazv, met_autor = :avtor, met_file = :file, met_year = :god, met_type = :metType_id, met_urlFiles = :URLfiles ";
    if(sotr_id>0)
        query.append(" , met_sotr_avtor = :sotr_id");
    query.append(" WHERE id_met = :id_metodichka");
    QSqlQuery *q = new QSqlQuery;
    q->prepare(query);
    q->bindValue(":id_metodichka", id_metodichka);
    q->bindValue(":discipline_id", discipline_id);
    q->bindValue(":nazv", nazv);
    q->bindValue(":avtor", avtor);
    q->bindValue(":file", file);
    q->bindValue(":god", god);
    q->bindValue(":metType_id", metType_id);
    if(sotr_id>0)
        q->bindValue(":sotr_id", sotr_id);
    q->bindValue(":URLfiles", URLfiles);
    if(q->exec() == 1)
    {
        return true;

    }
    else
        return false;
}

bool Dal_prepodcontrol::editSotrudnik(int id_sotr, QString FIO, QString telephon, QString mails, QString obrazovanie, int staj_obsh, int stajVKGTU, int ped_staj, QString uchZvan, int dolj, double stavka, QString logins, QString passw, QDate dateRojd, QDate dateReg, int status, QString MestoRojd, QString mestoRaboti, QString nacion, QString SeriaPasport, QString addres, QString semPoloj, QString socZash, int OnlyGak, QString sostav)
{
    QSqlQuery q;
    q.prepare("UPDATE is_sotrudniki SET st_fio = :FIO, st_mob_tel = :telephon, st_email = :mails, st_obrazovanie = :obrazovanie, st_staj_obchiy = :staj_obsh, st_staj_v_kgtu = :stajVKGTU, st_staj_ped =:ped_staj, st_uch_zvanie =:uchZvan, st_doljnost =:dolj, st_stavka =:stavka, st_login =:logins, st_passwd =:passw, st_date_birth =:dateRojd, st_date_reg =:dateReg, st_status =:status, st_mesto_rojdenia =:MestoRojd, st_mesto_raboty =:mestoRaboti, st_nacionalnost =:nacion, st_pasport_seria__ =:SeriaPasport, st_adress =:addres, st_semeinoe_poloj =:semPoloj, st___soc_zashity =:socZash, st_only_gak =:OnlyGak, st_sostav = :sostav WHERE id_sotr = " + QString::number(id_sotr));
    q.bindValue(":FIO", FIO);
    q.bindValue(":telephon", telephon);
    q.bindValue(":mails", mails);
    q.bindValue(":obrazovanie", obrazovanie);
    q.bindValue(":staj_obsh", staj_obsh);
    q.bindValue(":stajVKGTU", stajVKGTU);
    q.bindValue(":ped_staj", ped_staj);
    q.bindValue(":uchZvan", uchZvan);
    q.bindValue(":dolj", dolj);
    q.bindValue(":stavka", stavka);
    q.bindValue(":logins", logins);
    q.bindValue(":passw", passw);
    q.bindValue(":dateRojd", dateRojd.toString("yyyy-MM-dd"));
    q.bindValue(":dateReg", dateReg.toString("yyyy-MM-dd"));
    q.bindValue(":status", status);
    q.bindValue(":MestoRojd", MestoRojd);
    q.bindValue(":mestoRaboti", mestoRaboti);
    q.bindValue(":nacion", nacion);
    q.bindValue(":SeriaPasport", SeriaPasport);
    q.bindValue(":addres", addres);
    q.bindValue(":semPoloj", semPoloj);
    q.bindValue(":socZash", socZash);
    q.bindValue(":OnlyGak", OnlyGak);
    q.bindValue(":sostav", sostav);
    return q.exec();
}

bool Dal_prepodcontrol::deleteMetodichka(int id_metodichka)
{
    QSqlQuery query;
    query.prepare("delete from is_metodichki where id_met = :id_metodichka");
    query.bindValue(":id_metodichka", id_metodichka);
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

bool Dal_prepodcontrol::ProverkaGoda(QString godS, QString GodPo)
{
    QSqlQuery query;
    query.prepare("select * from is_GodSozdGrafika where God_s = :godS and God_po = :GodPo");
    query.bindValue(":godS", godS);
    query.bindValue(":GodPo", GodPo);
    query.exec();
    if (query.size()==0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

QSqlQuery *Dal_prepodcontrol::getCurrentKniga(int id_knigi)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select * from is_knigi where id_knigi = :id_knigi");
    query->bindValue(":id_knigi", id_knigi);
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

bool Dal_prepodcontrol::addKniga(int discipline_id, QString nazv, QString avtor, QString file, QDate god, QString izdatel, int sotr)
{
    QSqlQuery *q = new QSqlQuery;
    q->prepare("call AddKniga(:discipline_id,:nazv, :avtor, :file, :god,:izdatel, :sotr, @result)");
    q->bindValue(":discipline_id", discipline_id);
    q->bindValue(":nazv", nazv);
    q->bindValue(":avtor", avtor);
    q->bindValue(":file", file);
    q->bindValue(":god", god.toString("yyyy-MM-dd"));
    q->bindValue(":izdatel", izdatel);
    q->bindValue(":sotr", sotr);
    q->exec();
    q->exec(" select @result");
    q->first();
    int result = q->value(0).toInt();
    if(result == 1)
    {
        return false;

    }
    else
        return true;
}

bool Dal_prepodcontrol::editKniga(int id_knigi, int discipline_id, QString nazv, QString avtor, QString file, QDate god, QString izdatel, int sotr)
{
    QSqlQuery *q = new QSqlQuery;
    q->prepare("call updKniga(:id_knigi,:discipline_id,:nazv, :avtor, :file, :god,:izdatel, :sotr, @result)");
    q->bindValue(":id_knigi", id_knigi);
    q->bindValue(":discipline_id", discipline_id);
    q->bindValue(":nazv", nazv);
    q->bindValue(":avtor", avtor);
    q->bindValue(":file", file);
    q->bindValue(":god", god);
    q->bindValue(":izdatel", izdatel);
    q->bindValue(":sotr", sotr);
    q->exec();
    q->exec(" select @result");
    q->first();
    int result = q->value(0).toInt();
    if(result == 1)
    {
        return false;

    }
    else
        return true;
}

bool Dal_prepodcontrol::editPublNir(int id_publ, int pub_nimer_proecta, QString namefil, QString UrlFil, QString MejStandartNomer, QString NazvaniePublikacii, QString NazvanieJurnala, QString NomerVipuskaJurnala, QDate GodVipuska)
{
    QSqlQuery q;
    q.prepare("UPDATE is_publications SET pub_nir = :pub_nimer_proecta, pub_file = :namefil, pub_url = :UrlFil, mejdunarodStandartNomer = :MejStandartNomer, nazvaniePublikacii = :NazvaniePublikacii, nazvanieJurnala = :NazvanieJurnala,  NomerVipuskaJurnala = :NomerVipuskaJurnala, GodVipuska = :GodVipuska WHERE id_public = " + QString::number(id_publ));
    q.bindValue(":pub_nimer_proecta", pub_nimer_proecta);
    q.bindValue(":namefil", namefil);
    q.bindValue(":UrlFil", UrlFil);
    q.bindValue(":MejStandartNomer", MejStandartNomer);
    q.bindValue(":NazvaniePublikacii", NazvaniePublikacii);
    q.bindValue(":NazvanieJurnala", NazvanieJurnala);
    q.bindValue(":NomerVipuskaJurnala", NomerVipuskaJurnala);
    q.bindValue(":GodVipuska", GodVipuska.toString("yyyy-MM-dd"));
    return q.exec();
}

bool Dal_prepodcontrol::deleteKniga(int id_knigi)
{
    QSqlQuery query;
    query.prepare("delete from is_knigi where id_knigi = :id_knigi");
    query.bindValue(":id_knigi", id_knigi);
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

bool Dal_prepodcontrol::ProverkaPrepod(int prepod_id, QDateTime Dataprov)
{
    QString dl;
    qDebug()<<prepod_id<<Dataprov;
    dl = Dataprov.toString("yyyy-MM-dd hh:mm");
    qDebug()<<Dataprov;
    QSqlQuery query;
    //  query.prepare("SELECT * FROM is_GrafikVzaimoposesh WHERE  TIMESTAMPDIFF(HOUR, DataPosesh, :Dataprov) < 1  and FIO_prepod = :prepod_id");
    query.prepare("SELECT * FROM is_GrafikVzaimoposesh WHERE   DataPosesh = :dl  and FIO_prepod = :prepod_id");
    //  query.prepare("SELECT * FROM is_GrafikVzaimoposesh WHERE   DataPosesh between DataPosesh  and :Dataprov   and FIO_prepod = :prepod_id");
    query.bindValue(":dl", dl);
    query.bindValue(":prepod_id", prepod_id);

    query.exec();
    if (query.size()==0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_prepodcontrol::ProverProveraush(int prov_id, QDateTime Dataprov)
{
    QString df = Dataprov.toString("yyyy-MM-dd hh:mm");
    qDebug()<<df;
    QSqlQuery query;
    Dataprov.toString("yyyy-MM-dd hh:mm");
    // query.prepare("SELECT * FROM is_GrafikVzaimoposesh WHERE  TIMESTAMPDIFF(HOUR, DataPosesh, :Dataprov) < 1  and FIO_prover = :prov_id");
    query.prepare("SELECT * FROM is_GrafikVzaimoposesh WHERE  DataPosesh = :df and FIO_prover = :prov_id");
    query.bindValue(":df", df);
    query.bindValue(":prov_id", prov_id);

    query.exec();
    if (query.size()==0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_prepodcontrol::deletePublicationNir(int id_pub)
{
    QSqlQuery query;
    query.prepare("delete from is_publications where id_public = :id_pub");
    query.bindValue(":id_pub", id_pub);
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

bool Dal_prepodcontrol::deleteObrazKursi(int id_obrazKurs)
{
    QSqlQuery query;
    query.prepare("delete from is_platnieobrazovatkurs where id_PlatnieObrazovatKurs = :id_obrazKurs");
    query.bindValue(":id_obrazKurs", id_obrazKurs);
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

bool Dal_prepodcontrol::deleteSostavKom(int id_sost_kom)
{
    QSqlQuery query;
    query.prepare("delete from is_sostav_gak where `gak-id` = :id_sost_kom");
    query.bindValue(":id_sost_kom", id_sost_kom);
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

QSqlQueryModel *Dal_prepodcontrol::getSotrModels(QString sotr, int doljnost, QString sostav)
{
    QString query="";
    QSqlQueryModel *Sotrudnikmodel = new QSqlQueryModel(this);
    query = "select * from Sotr_view where st_only_gak = 0 and st_status=0";
    if(sotr!="")
        query.append(" and st_fio  like '%" + sotr + "%'  ");
    if(doljnost!=0)
        query.append(" and st_doljnost = " + QString::number(doljnost));
    if(!sostav.isEmpty())
        query.append(" and st_sostav  = '" + sostav + "'");
    query.append(" order by  st_doljnost, st_fio");
    Sotrudnikmodel->setQuery(query);
    Sotrudnikmodel->setHeaderData(1,Qt::Horizontal,tr("ФИО"));
    Sotrudnikmodel->setHeaderData(2,Qt::Horizontal,tr("Телефон"));
    Sotrudnikmodel->setHeaderData(3,Qt::Horizontal,tr("E-mail"));
    Sotrudnikmodel->setHeaderData(4,Qt::Horizontal,tr("Образование"));
    Sotrudnikmodel->setHeaderData(9,Qt::Horizontal,tr("Должность"));
    Sotrudnikmodel->setHeaderData(11,Qt::Horizontal,tr("Ставка"));
    Sotrudnikmodel->setHeaderData(17,Qt::Horizontal,tr("Уволен"));
    Sotrudnikmodel->setHeaderData(28,Qt::Horizontal,tr("Состав"));
    return Sotrudnikmodel;
}

QSqlQueryModel *Dal_prepodcontrol::getSotrReportModels(int doljnost, QString sostav)
{
    QString query = "";
    QSqlQueryModel *Sotrudnikmodel = new QSqlQueryModel(this);
    query = "select * from sotr_viewReport where 1 = 1 ";
    if(doljnost!=0)
        query.append(" and st_doljnost = " + QString::number(doljnost));
    if(!sostav.isEmpty())
        query.append(" and st_sostav  = '" + sostav + "'");
    query.append(" order by  st_sostav, st_fio, st_doljnost");
    Sotrudnikmodel->setQuery(query);
    //    Sotrudnikmodel->setHeaderData(1,Qt::Horizontal,tr("Состав"));
    //    Sotrudnikmodel->setHeaderData(2,Qt::Horizontal,tr("ФИО"));
    //    Sotrudnikmodel->setHeaderData(3,Qt::Horizontal,tr("Телефон"));
    //    Sotrudnikmodel->setHeaderData(4,Qt::Horizontal,tr("E-mail"));
    //    Sotrudnikmodel->setHeaderData(5,Qt::Horizontal,tr("Образование"));
    //    Sotrudnikmodel->setHeaderData(6,Qt::Horizontal,tr("Ученое звание"));
    //    Sotrudnikmodel->setHeaderData(7,Qt::Horizontal,tr("Должность"));
    //    Sotrudnikmodel->setHeaderData(8,Qt::Horizontal,tr("Ставка"));
    //    Sotrudnikmodel->setHeaderData(9,Qt::Horizontal,tr("Дата рождения"));
    //    Sotrudnikmodel->setHeaderData(10,Qt::Horizontal,tr("Место рождения"));
    //    Sotrudnikmodel->setHeaderData(11,Qt::Horizontal,tr("Национальность"));
    //    Sotrudnikmodel->setHeaderData(12,Qt::Horizontal,tr("Серия паспорта"));
    //    Sotrudnikmodel->setHeaderData(13,Qt::Horizontal,tr("Адрес проживания"));
    //    Sotrudnikmodel->setHeaderData(14,Qt::Horizontal,tr("Семейное положение"));
    //    Sotrudnikmodel->setHeaderData(15,Qt::Horizontal,tr("№ Соц. защиты"));
    return Sotrudnikmodel;
}

QSqlQueryModel *Dal_prepodcontrol::getUchastnikiKom(int pred, int secr, int ch1, int ch2, int ch3, int ch4, int ch5)
{
    QString query="";
    QSqlQueryModel *UchastModel = new QSqlQueryModel(this);
    query = "select * from is_sotrudniki where 1 = 1";
    if(pred!=0)
        query.append(" and id_sotr  <> " + QString::number(pred));
    if(secr!=0)
        query.append(" and id_sotr  <> " + QString::number(secr));
    if(ch1!=0)
        query.append(" and id_sotr  <> " + QString::number(ch1));
    if(ch2!=0)
        query.append(" and id_sotr  <> " + QString::number(ch2));
    if(ch3!=0)
        query.append(" and id_sotr  <> " + QString::number(ch3));
    if(ch4!=0)
        query.append(" and id_sotr  <> " + QString::number(ch4));
    if(ch5!=0)
        query.append(" and id_sotr  <> " + QString::number(ch5));
    UchastModel->setQuery(query);
    return UchastModel;
}

QSqlQueryModel *Dal_prepodcontrol::getPlatnieObrazUslugi(QString nazvanKur, QString VidKur)
{
    QString query="";
    QSqlQueryModel *ObrazKurModel = new QSqlQueryModel(this);
    query = "select * from planmieobrazuslugiview where 1 = 1";
    if(nazvanKur!="")
        query.append(" and NazvanieKursa like '%" + nazvanKur + "%'");
    if(VidKur!="")
        query.append(" and tipKursa like '%" + VidKur + "%'");
    ObrazKurModel->setQuery(query);
    ObrazKurModel->setHeaderData(2,Qt::Horizontal,tr("Название курса"));
    ObrazKurModel->setHeaderData(3,Qt::Horizontal,tr("Преподаватель"));
    ObrazKurModel->setHeaderData(4,Qt::Horizontal,tr("Количество часов"));
    ObrazKurModel->setHeaderData(5,Qt::Horizontal,tr("Вид занятия"));
    ObrazKurModel->setHeaderData(6,Qt::Horizontal,tr("Период обучения"));
    ObrazKurModel->setHeaderData(7,Qt::Horizontal,tr("Оплата"));
    ObrazKurModel->setHeaderData(8,Qt::Horizontal,tr("Тип курса"));
    return ObrazKurModel;
}

bool Dal_prepodcontrol::addPrepodDiscipline(int sotrudnik_id, int group_id, int discipline_id, int lk, int pr, int lb)
{
    QSqlQuery q;
    qDebug()<<sotrudnik_id<<group_id<<discipline_id<<lk<<pr<<lb;

    q.prepare("INSERT INTO is_predmetPrepod (prepod_id, group_id, predmet_id, lektor, praktik, labor) VALUES (:sotrudnik_id, :group_id, :discipline_id, :lk, :pr, :lb)");
    q.bindValue(":sotrudnik_id", sotrudnik_id);
    q.bindValue(":group_id", group_id);
    q.bindValue(":discipline_id", discipline_id);
    q.bindValue(":lk", lk);
    q.bindValue(":pr", pr);
    q.bindValue(":lb", lb);
    return q.exec();
}

QSqlQueryModel *Dal_prepodcontrol::getPredmetPrepod(int id_discipline)
{
    QSqlQueryModel *model = new QSqlQueryModel(this);
    model->setQuery("select * from predmetPrepod_view  where predmet_id = " + QString::number(id_discipline));
    model->setHeaderData(0,Qt::Horizontal,tr("Дисциплина"));
    model->setHeaderData(1,Qt::Horizontal,tr("Группа"));
    model->setHeaderData(2,Qt::Horizontal,tr("Сотрудник"));
    model->setHeaderData(3,Qt::Horizontal,tr("лк"));
    model->setHeaderData(4,Qt::Horizontal,tr("пр"));
    model->setHeaderData(5,Qt::Horizontal,tr("лб"));
    return model;
}

QSqlQueryModel *Dal_prepodcontrol::getGroups(int id_discipline)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT is_course.course FROM is_discipline AS is_discipline, kafedraDB.is_semmestr AS is_semmestr, kafedraDB.is_course AS is_course WHERE is_discipline.d_semestr = is_semmestr.id_semestr AND is_semmestr.course_id = is_course.id_course AND is_discipline.id_discipline = :id_discipline");
    query->bindValue(":id_discipline", id_discipline);
    query->exec();
    query->first();
    QSqlQueryModel *model = new QSqlQueryModel(this);
    if(QDate::currentDate().month() >= 9)
        model->setQuery("select * from is_group  where g_godPostupleniya - " + QString::number(QDate::currentDate().year()) + " = " + query->value(0).toString());
    else
        model->setQuery("select * from is_group  where g_godPostupleniya - " + QString::number(QDate::currentDate().year()) + " = " + QString::number(query->value(0).toInt()-1));
    return model;
}

bool Dal_prepodcontrol::existLektor(int discipline_id)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT id_predmetPrepod FROM is_predmetPrepod WHERE predmet_id = :discipline_id AND lektor = 1");
    query->bindValue(":discipline_id", discipline_id);
    query->exec();
    query->next();
    //    qDebug()<<discipline_id<<query->value(0).isValid();
    return query->isValid();
}

bool Dal_prepodcontrol::existPraktik(int group_id, int discipline_id)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT id_predmetPrepod FROM is_predmetPrepod WHERE group_id = :group_id AND predmet_id = :discipline_id AND praktik = 1");
    query->bindValue(":discipline_id", discipline_id);
    query->bindValue(":group_id", group_id);
    query->exec();
    query->next();
    qDebug()<<query->isValid();
    return query->isValid();
}

bool Dal_prepodcontrol::existLabor(int group_id, int discipline_id)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT id_predmetPrepod FROM is_predmetPrepod WHERE group_id = :group_id AND predmet_id = :discipline_id AND labor = 1");
    query->bindValue(":discipline_id", discipline_id);
    query->bindValue(":group_id", group_id);
    query->exec();
    query->next();
    qDebug()<<query->isValid();
    return query->isValid();
}

QSqlQueryModel *Dal_prepodcontrol::getPovyshKval(QString sotr, QString tema, QString kurs)
{
    QString query = "";
    QSqlQueryModel *model = new QSqlQueryModel(this);
    query = "select * from povyshkval_view where 1 = 1 ";
    if(!sotr.isEmpty())
        query.append(" and st_fio like '%" + sotr + "%'");
    if(!tema.isEmpty())
        query.append(" and lvlup_theme like '%" + tema + "%'");
    if(!kurs.isEmpty())
        query.append(" and lvlup_kurs like '%" + kurs + "%'");
    model->setQuery(query);
    model->setHeaderData(2,Qt::Horizontal,tr("Сотрудник"));
    model->setHeaderData(3,Qt::Horizontal,tr("Дата"));
    model->setHeaderData(4,Qt::Horizontal,tr("Тема"));
    model->setHeaderData(5,Qt::Horizontal,tr("Курс"));
    model->setHeaderData(6,Qt::Horizontal,tr("Место"));
    model->setHeaderData(7,Qt::Horizontal,tr("Сертификат"));
    model->setHeaderData(8,Qt::Horizontal,tr("Файл"));
    model->setHeaderData(9,Qt::Horizontal,tr("Оценка"));
    return model;
}

QSqlQueryModel *Dal_prepodcontrol::getPovyshKvalReport(int doljnost, QDate beginDate, QDate endDate)
{
    QString query = "";
    QSqlQueryModel *model = new QSqlQueryModel(this);
    query = "SELECT * FROM povish_kval_viewReport WHERE 1 = 1 ";
    if(doljnost != 0)
        query.append(" AND id_doljnost = " + QString::number(doljnost));
    if(!beginDate.isNull())
        query.append(" AND lvlup_date BETWEEN '" + beginDate.toString("yyyy-MM-dd") + "' AND '" + endDate.toString("yyyy-MM-dd 23:59:59") + "'");
    query.append(" ORDER BY id_doljnost ");
    model->setQuery(query);
    model->setHeaderData(3,Qt::Horizontal,tr("Должность"));
    model->setHeaderData(4,Qt::Horizontal,tr("Сотрудник"));
    model->setHeaderData(5,Qt::Horizontal,tr("Дата"));
    model->setHeaderData(6,Qt::Horizontal,tr("Тема"));
    model->setHeaderData(7,Qt::Horizontal,tr("Курс"));
    model->setHeaderData(8,Qt::Horizontal,tr("Место"));
    model->setHeaderData(9,Qt::Horizontal,tr("Оценка"));
    qDebug()<<query;
    return model;
}

QSqlQuery *Dal_prepodcontrol::getCurrentPovyskKval(int id_povyskKval)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select * from is_povish_kval where id_lvlup = :id_povyskKval");
    query->bindValue(":id_povyskKval", id_povyskKval);
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

bool Dal_prepodcontrol::addPovyskKval(int sotr_id, QDate date, QString tema, QString kurs, QString mesto, QString sert, QString sertFile, double ocenka, QString URLfile)
{
    QSqlQuery q;
    q.prepare("INSERT INTO is_povish_kval (lvlup_sotrudnik, lvlup_date, lvlup_theme, lvlup_kurs, lvlup_mesto, lvlup_sert, lvlup_sert_file, lvlup_ocenka, lvlup_URLFail) VALUES (:sotr_id, :date, :tema, :kurs, :mesto, :sert, :sertFile, :ocenka, :URLfile)");
    q.bindValue(":sotr_id", sotr_id);
    q.bindValue(":date", date);
    q.bindValue(":tema", tema);
    q.bindValue(":kurs", kurs);
    q.bindValue(":mesto", mesto);
    q.bindValue(":sert", sert);
    q.bindValue(":sertFile", sertFile);
    q.bindValue(":ocenka", ocenka);
    q.bindValue(":URLfile", URLfile);
    return q.exec();
}

bool Dal_prepodcontrol::editPovyskKval(int id_povyskKval, int sotr_id, QDate date, QString tema, QString kurs, QString mesto, QString sert, QString sertFile, double ocenka, QString URLfile)
{
    QSqlQuery q;
    q.prepare("UPDATE is_povish_kval SET lvlup_sotrudnik = :sotr_id, lvlup_date = :date, lvlup_theme = :tema, lvlup_kurs = :kurs, lvlup_mesto = :mesto, lvlup_sert = :sert, lvlup_sert_file =  :sertFile, lvlup_ocenka = :ocenka, lvlup_URLFail = :URLfile WHERE id_lvlup = " + QString::number(id_povyskKval));
    q.bindValue(":sotr_id", sotr_id);
    q.bindValue(":date", date);
    q.bindValue(":tema", tema);
    q.bindValue(":kurs", kurs);
    q.bindValue(":mesto", mesto);
    q.bindValue(":sert", sert);
    q.bindValue(":sertFile", sertFile);
    q.bindValue(":ocenka", ocenka);
    q.bindValue(":id_povyskKval", id_povyskKval);
    q.bindValue(":URLfile", URLfile);
    return q.exec();
}

bool Dal_prepodcontrol::editGrafkik(int zap_id, int prepod, int prov, QDateTime dataprov, QString godS, QString godPo)
{
    QSqlQuery q;
    q.prepare("UPDATE is_GrafikVzaimoposesh SET FIO_prepod = :prepod, FIO_prover = :prov, DataPosesh = :dataprov, NachaloGoda = :godS, KoneccGoda = :godPo WHERE id_GrafikVzaimoposesh = " + QString::number(zap_id));
    q.bindValue(":prepod", prepod);
    q.bindValue(":prov", prov);
    q.bindValue(":dataprov", dataprov);
    q.bindValue(":godS", godS);
    q.bindValue(":godPo", godPo);
    return q.exec();
}

bool Dal_prepodcontrol::deletePovyskKval(int id_povyskKval)
{
    QSqlQuery query;
    query.prepare("delete from is_povish_kval where id_lvlup = :id_povyskKval");
    query.bindValue(":id_povyskKval", id_povyskKval);
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

QSqlQueryModel* Dal_prepodcontrol::getRaiting(int doljnost, QDate beginDate, QDate endDate)
{
    QString query = "";
    QSqlQueryModel *model = new QSqlQueryModel(this);
    query = "SELECT `is_sotrudniki`.`id_sotr`, `is_sotrudniki`.`st_fio`, \
            COUNT( DISTINCT `is_nagradi`.`id_nagradi` ) AS `kolvoNagradi`,\
            COUNT( DISTINCT `is_kur_rab`.`id_kur_rab` ) AS `kolvoKurRab`, \
            COUNT( DISTINCT `is_metodichki`.`id_met` ) AS `kolvoMet`, \
            COUNT( DISTINCT `is_povish_kval`.`id_lvlup` ) AS `kolvoPovyshKval`, \
            COUNT( DISTINCT `is_publications`.`id_public` ) AS `kolvoPublic`, \
            COUNT( DISTINCT `is_knigi`.`id_knigi` ) AS `kolvoKnigi`, \
            COUNT( DISTINCT `is_proekty`.`id_proekta` ) AS `kolvoProektov`, \
            (COUNT( DISTINCT `is_nagradi`.`id_nagradi` ) + COUNT( DISTINCT `is_kur_rab`.`id_kur_rab` ) + COUNT( DISTINCT `is_metodichki`.`id_met` )  + COUNT( DISTINCT `is_povish_kval`.`id_lvlup` ) + COUNT( DISTINCT `is_publications`.`id_public` ) + COUNT( DISTINCT `is_knigi`.`id_knigi` ) + COUNT( DISTINCT `is_proekty`.`id_proekta` )) AS `Total`\
            FROM { OJ `is_sotrudniki` AS `is_sotrudniki` \
                LEFT OUTER JOIN `is_metodichki` AS `is_metodichki` ON `is_sotrudniki`.`id_sotr` = `is_metodichki`.`met_sotr_avtor` AND `is_metodichki`.`met_year` BETWEEN :beginDate AND :endDate \
                LEFT OUTER JOIN `is_nir_sotr` AS `is_nir_sotr` ON `is_sotrudniki`.`id_sotr` = `is_nir_sotr`.`nir_sotr_prepodavatel` \
                LEFT OUTER JOIN `is_publications` AS `is_publications` ON `is_nir_sotr`.`nir_sotr_id` = `is_publications`.`pub_nir` AND `is_publications`.`GodVipuska` BETWEEN :beginDate AND :endDate \
                LEFT OUTER JOIN `is_nagradi` AS `is_nagradi` ON `is_sotrudniki`.`id_sotr` = `is_nagradi`.`sotr_id` AND `is_nagradi`.`nagr_date` BETWEEN :beginDate AND :endDate \
                LEFT OUTER JOIN `is_povish_kval` AS `is_povish_kval` ON `is_sotrudniki`.`id_sotr` = `is_povish_kval`.`lvlup_sotrudnik` AND `is_povish_kval`.`lvlup_date` BETWEEN :beginDate AND :endDate \
                LEFT OUTER JOIN `is_kur_rab` AS `is_kur_rab` ON `is_sotrudniki`.`id_sotr` = `is_kur_rab`.`sotrudnik` AND `is_kur_rab`.`date` BETWEEN :beginDate AND :endDate \
                LEFT OUTER JOIN `is_knigi` AS `is_knigi` ON `is_knigi`.`sotr_avtor` = `is_sotrudniki`.`id_sotr` AND `is_knigi`.`god` BETWEEN :beginDate AND :endDate  \
                LEFT OUTER JOIN `is_proekty` AS `is_proekty` ON `is_proekty`.`sotrudnik_id` = `is_sotrudniki`.`id_sotr` AND `is_proekty`.`proekt_data` BETWEEN :beginDate AND :endDate } \
    WHERE `is_sotrudniki`.`st_only_gak` = 0 AND `is_sotrudniki`.`st_status` = 0 AND `is_sotrudniki`.`st_doljnost` = :st_doljnost GROUP BY `is_sotrudniki`.`id_sotr` ORDER BY `Total` desc ";
            /*if(!sotr.isEmpty())
                        query.append(" and st_fio like '%" + sotr + "%'");*/
            query.replace(":beginDate","\"" + beginDate.toString("yyyy-MM-dd") + "\"");
    query.replace(":endDate", "\"" + endDate.toString("yyyy-MM-dd") + "\"");
    query.replace(":st_doljnost", QString::number(doljnost));
    model->setQuery(query);
    model->setHeaderData(1,Qt::Horizontal,tr("Сотрудник"));
    model->setHeaderData(2,Qt::Horizontal,tr("Награды"));
    model->setHeaderData(3,Qt::Horizontal,tr("Круторские работы"));
    model->setHeaderData(4,Qt::Horizontal,tr("Методички"));
    model->setHeaderData(5,Qt::Horizontal,tr("Повышения квалификации"));
    model->setHeaderData(6,Qt::Horizontal,tr("Публикации"));
    model->setHeaderData(7,Qt::Horizontal,tr("Книги"));
    model->setHeaderData(8,Qt::Horizontal,tr("Проекты"));
    model->setHeaderData(9,Qt::Horizontal,tr("Итого"));
    qDebug()<<query;
    return model;
}

QSqlQueryModel *Dal_prepodcontrol::getSostavKom(int nomRaport)
{
    QString query = "";
    QSqlQueryModel *SostavKomismodel = new QSqlQueryModel(this);
    query = "select * from SostavKomissii where 1 = 1 ";
    if(nomRaport!=0)
        query.append(" and n_raport = " + QString::number(nomRaport));
    SostavKomismodel->setQuery(query);
//    SostavKomismodel->setHeaderData(8,Qt::Horizontal,tr("Председатель"));
//    SostavKomismodel->setHeaderData(9,Qt::Horizontal,tr("Секретарь"));
//    SostavKomismodel->setHeaderData(10,Qt::Horizontal,tr("Член комиссии №1"));
//    SostavKomismodel->setHeaderData(11, Qt::Horizontal,tr("Член комиссии №2"));
//    SostavKomismodel->setHeaderData(12,Qt::Horizontal,tr("Член комиссии №3"));
//    SostavKomismodel->setHeaderData(13,Qt::Horizontal,tr("Член комиссии №4"));
//    SostavKomismodel->setHeaderData(14,Qt::Horizontal,tr("Член комиссии №5"));
    SostavKomismodel->setHeaderData(15,Qt::Horizontal,tr("Год"));
//    SostavKomismodel->setHeaderData(16,Qt::Horizontal,tr("Номер рапорта"));
    return SostavKomismodel;
}

QSqlQueryModel *Dal_prepodcontrol::getNagradi(QString sotr)
{
    QString query = "";
    QSqlQueryModel *model = new QSqlQueryModel(this);
    query = "select * from nagradi_view where 1 = 1 ";
    if(!sotr.isEmpty())
        query.append(" and st_fio like '%" + sotr + "%'");
    model->setQuery(query);
    model->setHeaderData(2,Qt::Horizontal,tr("Сотрудник"));
    model->setHeaderData(3,Qt::Horizontal,tr("Награда"));
    model->setHeaderData(4,Qt::Horizontal,tr("Дата"));
    //model->setHeaderData(5,Qt::Horizontal,tr("Файл"));
    return model;
}

QSqlQuery *Dal_prepodcontrol::getCurrentNagradi(int id_nagradi)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select * from is_nagradi where id_nagradi = :id_nagradi");
    query->bindValue(":id_nagradi", id_nagradi);
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

bool Dal_prepodcontrol::addNagradi(int sotr_id, QString nagrada, QDate date, QString file)
{
    QSqlQuery q;
    q.prepare("INSERT INTO is_nagradi (sotr_id, nagrada, nagr_date, file) VALUES (:sotr_id, :nagrada, :date, :file)");
    q.bindValue(":sotr_id", sotr_id);
    q.bindValue(":nagrada", nagrada);
    q.bindValue(":date", date.toString("yyyy-MM-dd"));
    q.bindValue(":file", file);
    return q.exec();
}

bool Dal_prepodcontrol::editNagradi(int id_nagradi, int sotr_id, QString nagrada, QDate date, QString file)
{
    QSqlQuery q;
    qDebug()<<sotr_id<<nagrada<<date<<file;
    q.prepare("UPDATE is_nagradi SET sotr_id = :sotr_id, nagrada = :nagrada, nagr_date = :date, file = :file WHERE id_nagradi = " + QString::number(id_nagradi));
    q.bindValue(":sotr_id", sotr_id);
    q.bindValue(":date", date.toString("yyyy-MM-dd"));
    q.bindValue(":nagrada", nagrada);
    q.bindValue(":file", file);
    q.bindValue(":id_nagradi", id_nagradi);
    return q.exec();
}

bool Dal_prepodcontrol::deleteNagradi(int id_nagradi)
{
    QSqlQuery query;
    query.prepare("delete from is_nagradi where id_nagradi = :id_nagradi");
    query.bindValue(":id_nagradi", id_nagradi);
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

QSqlQueryModel *Dal_prepodcontrol::getPorucheniya(int status_id, QString title)
{
    QString query = "";
    QSqlQueryModel *model = new QSqlQueryModel(this);
    query = "select * from Poruchenias_view where (poruchitel = " + QString::number(dal_main->getCurrentEmployee()) + " OR ispolnitel = " + QString::number(dal_main->getCurrentEmployee()) + ") ";
    if(status_id!=0)
        query.append(" and status_id =  " + QString::number(status_id));
    if(!title.isEmpty())
        query.append(" and title like '%" + title + "%'");
    model->setQuery(query);
    model->setHeaderData(5,Qt::Horizontal,tr("Заголовок поручения"));
    model->setHeaderData(6,Qt::Horizontal,tr("Текст поручения"));
    //model->setHeaderData(7,Qt::Horizontal,tr("Файлы"));
    model->setHeaderData(8,Qt::Horizontal,tr("Менеджер"));
    model->setHeaderData(9,Qt::Horizontal,tr("Исполнитель"));
    model->setHeaderData(10,Qt::Horizontal,tr("Статус"));
    model->setHeaderData(11,Qt::Horizontal,tr("Дата"));
    return model;
}

QSqlQuery *Dal_prepodcontrol::getCurrentPoruchenie(int id_poruch)
{
    QSqlQuery *query = new QSqlQuery;
    qDebug()<<id_poruch;
    query->prepare("select * from Poruchenias_view where id_poruchenie = :id_poruch");
    query->bindValue(":id_poruch", id_poruch);
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

QSqlQuery *Dal_prepodcontrol::getCurrentSostavGAK(int id_gaks)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("select * from SostavKomissii where `gak-id` = :id_gaks");
    query->bindValue(":id_gaks", id_gaks);
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

bool Dal_prepodcontrol::addPoruchenie(QString title, QString text, QString files, int ispolnitel, QDate date)
{
    QSqlQuery q;
    q.prepare("INSERT INTO is_porucheniya (title, text, files, poruchitel, ispolnitel, status_id, check_date, checkPoruch, checkIspoln) VALUES (:title, :text, :files, :poruchitel, :ispolnitel, :status_id, :date, :checkPoruch, :checkIspoln)");
    q.bindValue(":title", title);
    q.bindValue(":text", text);
    q.bindValue(":files", files);
    q.bindValue(":poruchitel", dal_main->getCurrentEmployee());
    q.bindValue(":ispolnitel", ispolnitel);
    q.bindValue(":status_id", 1); // по умолчанию выставить статус на "Не прочитано"
    q.bindValue(":date", date.toString("yyyy-MM-dd"));
    q.bindValue(":checkPoruch", true);
    q.bindValue(":checkIspoln", false);
    return q.exec();
}

bool Dal_prepodcontrol::PlatnieObrazovatslugiAdd(int id_sotrud, QString nazvanKursa, QString kolvoChasov, QString vidZanyatia, QString periodObuch, QString oplata, QString tipKursa)
{
    QSqlQuery q;
    q.prepare("INSERT INTO is_platnieobrazovatkurs (NazvanieKursa, VedetKurs, KolvoChasov, vidZanyatii, periodObuch, Oplata, tipKursa)\
              VALUES (:nazvanKursa, :id_sotrud, :kolvoChasov, :vidZanyatia, :periodObuch, :oplata, :tipKursa)");
              q.bindValue(":nazvanKursa", nazvanKursa);
            q.bindValue(":id_sotrud", id_sotrud);
    q.bindValue(":kolvoChasov", kolvoChasov);
    q.bindValue(":vidZanyatia",vidZanyatia);
    q.bindValue(":periodObuch", periodObuch);
    q.bindValue(":oplata", oplata);
    q.bindValue(":tipKursa", tipKursa);
    return q.exec();
}

bool Dal_prepodcontrol::editPoruchenie(int id_poruch, QString title, QString text, QString files, int ispolnitel, QDate date)
{
    QSqlQuery q;
    q.prepare("UPDATE is_porucheniya SET title = :title, text = :text, files = :files, ispolnitel = :ispolnitel, check_date = :date WHERE id_poruchenie = " + QString::number(id_poruch));
    q.bindValue(":title", title);
    q.bindValue(":text", text);
    q.bindValue(":files", files);
    q.bindValue(":ispolnitel", ispolnitel);
    //    q.bindValue(":status_id", 1); // по умолчанию выставить статус на "Не прочитано"
    q.bindValue(":date", date);
    q.bindValue(":id_poruch", id_poruch);
    return q.exec();
}

bool Dal_prepodcontrol::PlatnieObrazovatslugiEdit(int zap, int id_sotrud, QString nazvanKursa, QString kolvoChasov, QString vidZanyatia, QString periodObuch, QString oplata, QString tipKursa)
{
    QSqlQuery q;
    q.prepare("UPDATE is_platnieobrazovatkurs SET NazvanieKursa = :nazvanKursa, VedetKurs = :id_sotrud, KolvoChasov = :kolvoChasov, vidZanyatii = :vidZanyatia, periodObuch = :periodObuch, Oplata = :oplata, tipKursa = :tipKursa WHERE id_PlatnieObrazovatKurs = " + QString::number(zap));
    q.bindValue(":nazvanKursa", nazvanKursa);
    q.bindValue(":id_sotrud", id_sotrud);
    q.bindValue(":kolvoChasov", kolvoChasov);
    q.bindValue(":vidZanyatia",vidZanyatia);
    q.bindValue(":periodObuch", periodObuch);
    q.bindValue(":oplata", oplata);
    q.bindValue(":tipKursa", tipKursa);
    return q.exec();
}

bool Dal_prepodcontrol::editPlanZasKaf(int zap, QString GODS, QString GODSPO, QString SrokVipol, QString VoprosZas, QString ispolnit)
{
    QSqlQuery q;
    q.prepare("UPDATE is_prosmotrPlana SET VoprosiZased = :VoprosZas, ispolnit = :ispolnit, SrokiIspoln = :SrokVipol, God_s = :GODS, God_po = :GODSPO WHERE id_prosmotrPlana = " + QString::number(zap));
    q.bindValue(":VoprosZas", VoprosZas);
    q.bindValue(":ispolnit", ispolnit);
    q.bindValue(":SrokVipol", SrokVipol);
    q.bindValue(":GODS", GODS);
    q.bindValue(":GODSPO", GODSPO);
    return q.exec();
}

bool Dal_prepodcontrol::sostavKomEdit(int zap, int preds, int secr, int ch1, int ch2, int ch3, int ch4, int ch5, QDate dates, int chislo)
{
    qDebug()<<zap<<preds<<secr<<ch1<<ch2<<ch3<<ch4<<ch5<<dates<<chislo;
    QSqlQuery q;
    q.prepare("UPDATE is_sostav_gak SET predsed = :preds, secretar = :secr, chlen1 = :ch1, chlen2 = :ch2, chlen3 = :ch3 , chlen4 = :ch4 , chlen5 = :ch5 , date = :dates, n_raport=:chislo WHERE `gak-id` = " + QString::number(zap));
    q.bindValue(":preds", preds);
    q.bindValue(":secr", secr);
    q.bindValue(":ch1", ch1);
    q.bindValue(":ch2", ch2);
    q.bindValue(":ch3", ch3);
    q.bindValue(":ch4", ch4);
    q.bindValue(":ch5", ch5);
    q.bindValue(":dates", dates.toString("yyyy-MM-dd"));
    q.bindValue(":chislo", chislo);
    return q.exec();
}

bool Dal_prepodcontrol::deletePoruchenie(int id_poruch)
{
    QSqlQuery query;
    query.prepare("delete from is_porucheniya where id_poruchenie = :id_poruch");
    query.bindValue(":id_poruch", id_poruch);
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

bool Dal_prepodcontrol::setStatusPorucheniya(int id_poruch, int status_id, bool checkPoruch, bool checkIspoln)
{
    QSqlQuery q;
    q.prepare("UPDATE is_porucheniya SET status_id = :status_id, checkPoruch = :checkPoruch, checkIspoln = :checkIspoln WHERE id_poruchenie = " + QString::number(id_poruch));
    q.bindValue(":status_id", status_id);
    q.bindValue(":checkPoruch", checkPoruch);
    q.bindValue(":checkIspoln", checkIspoln);
    q.bindValue(":id_poruch", id_poruch);
    return q.exec();
}

bool Dal_prepodcontrol::ProverNaIzmenenia(int proveryaush, QDateTime datapos)
{
    qDebug()<<proveryaush<<datapos;
    QSqlQuery query;
    query.prepare("SELECT * FROM is_GrafikVzaimoposesh WHERE DataPosesh >= :datapos  - INTERVAL 1 HOUR and FIO_prover=:proveryaush");
    query.bindValue(":datapos", datapos.toString("yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":proveryaush", proveryaush);

    query.exec();
    if (query.size()==0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

bool Dal_prepodcontrol::ProverNaIzmForPrepod(int prepod, QDateTime datapos)
{
    QSqlQuery query;
    query.prepare("SELECT * FROM is_GrafikVzaimoposesh WHERE DataPosesh >= :datapos  - INTERVAL 1 HOUR and FIO_prepod=:prepod");
    query.bindValue(":datapos", datapos.toString("yyyy-MM-dd hh:mm:ss"));
    query.bindValue(":prepod", prepod);

    query.exec();
    if (query.size()==0)
    {
        return false;
    }
    else
    {
        return true;
    }
}

QSqlQuery *Dal_prepodcontrol::getCurrentRaspisanie(int id_sotr)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT * FROM is_raspisanie WHERE sotr_id = :id_sotr");
    query->bindValue(":id_sotr", id_sotr);
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

QSqlQuery *Dal_prepodcontrol::getCurrentPlatnuyObrazovatUslugu(int id_platnieobrazUslugi)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT * FROM planmieobrazuslugiview WHERE id_PlatnieObrazovatKurs = :id_platnieobrazUslugi");
    query->bindValue(":id_platnieobrazUslugi", id_platnieobrazUslugi);
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

QSqlQuery *Dal_prepodcontrol::getCurrentGrafVzaimoPos(int id_graf)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT * FROM is_GrafikVzaimoposesh WHERE id_GrafikVzaimoposesh = :id_graf");
    query->bindValue(":id_graf", id_graf);
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

QSqlQuery *Dal_prepodcontrol::deletePlanKaf(QString Gods, QString GodPo)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("delete FROM is_GodSozdPlanaKaf WHERE God_s = :Gods and God_po = :GodPo");
    query->bindValue(":Gods", Gods);
    query->bindValue(":GodPo", GodPo);
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

QSqlQuery *Dal_prepodcontrol::deleteProsmotrPlanKaf(QString Gods, QString GodPo)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("delete FROM is_prosmotrPlana WHERE God_s = :Gods and God_po = :GodPo");
    query->bindValue(":Gods", Gods);
    query->bindValue(":GodPo", GodPo);
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();
    return query;
}

QSqlQueryModel *Dal_prepodcontrol::getSpravkiPochas(int sotr_id, QDate dateBegin, QDate dateEnd)
{
    QString query = "SELECT * FROM spravki_pochas_view WHERE 1=1 ";
    QSqlQueryModel  *model = new QSqlQueryModel(this);
    if(sotr_id!=0)
        query.append(" AND sotr_id = " + QString::number(sotr_id));
    if(!dateBegin.isNull())
        query.append(" AND data_za BETWEEN '" + dateBegin.toString("yyyy-MM-dd") + "' AND '" + dateEnd.toString("yyyy-MM-dd 23:59:59") + "'");
    model->setQuery(query);
    model->setHeaderData(3,Qt::Horizontal,tr("Факультет"));
    model->setHeaderData(4,Qt::Horizontal,tr("Кафедра"));
    model->setHeaderData(5,Qt::Horizontal,tr("ФИО сотрудника"));
    model->setHeaderData(6,Qt::Horizontal,tr("Номер протокола"));
    model->setHeaderData(7,Qt::Horizontal,tr("Дата за"));
    model->setHeaderData(8,Qt::Horizontal,tr("Лекций"));
    model->setHeaderData(9,Qt::Horizontal,tr("Лабор."));
    model->setHeaderData(10,Qt::Horizontal,tr("Практик"));
    model->setHeaderData(11,Qt::Horizontal,tr("On-line"));
    model->setHeaderData(12,Qt::Horizontal,tr("Off-line"));
    model->setHeaderData(13,Qt::Horizontal,tr("Контр. раб."));
    model->setHeaderData(14,Qt::Horizontal,tr("Экзам."));
    model->setHeaderData(15,Qt::Horizontal,tr("Курс. раб."));
    model->setHeaderData(16,Qt::Horizontal,tr("Язык"));
    model->setHeaderData(17,Qt::Horizontal,tr("Номер справки"));
    model->setHeaderData(18,Qt::Horizontal,tr("Всего"));
    return model;
}

QSqlQueryModel *Dal_prepodcontrol::getSpravkiPochasReport(int sotr_id, QDate date)
{
    QString query = "SELECT * FROM spravki_pochas_viewReport WHERE 1=1 ";
    QSqlQueryModel  *model = new QSqlQueryModel(this);
    if(sotr_id!=0)
        query.append(" AND sotr_id = " + QString::number(sotr_id));
    if(!date.isNull())
        query.append(" AND data_za = '" + date.toString("yyyy-MM") + "'");
    model->setQuery(query);
    model->setHeaderData(1,Qt::Horizontal,tr("ФИО сотрудника"));
    model->setHeaderData(2,Qt::Horizontal,tr("Номер протокола"));
    model->setHeaderData(3,Qt::Horizontal,tr("Дата за"));
    model->setHeaderData(4,Qt::Horizontal,tr("Лекций"));
    model->setHeaderData(5,Qt::Horizontal,tr("Лабор."));
    model->setHeaderData(6,Qt::Horizontal,tr("Практик"));
    model->setHeaderData(7,Qt::Horizontal,tr("On-line"));
    model->setHeaderData(8,Qt::Horizontal,tr("Off-line"));
    model->setHeaderData(9,Qt::Horizontal,tr("Контр. раб."));
    model->setHeaderData(10,Qt::Horizontal,tr("Экзам."));
    model->setHeaderData(11,Qt::Horizontal,tr("Курс. раб."));
    model->setHeaderData(12,Qt::Horizontal,tr("Язык"));
    model->setHeaderData(13,Qt::Horizontal,tr("Номер справки"));
    model->setHeaderData(14,Qt::Horizontal,tr("Всего"));
    qDebug()<<query;
    return model;
}

QSqlQuery *Dal_prepodcontrol::getCurrentSpravkaPochas(int id_sparvki_pochas)
{
    QSqlQuery *query = new QSqlQuery;
    query->prepare("SELECT * FROM spravki_pochas_view  WHERE id_spravki_pochas = " + QString::number(id_sparvki_pochas));
    query->exec();
    if (! query->isActive())
    {
        return NULL;
    }
    query->first();

    return query;
}

int Dal_prepodcontrol::addSpravkaPochas(int nomer_spravki, QString facultet, int kafedra_id, int sotr_id, int nomer_protocola, QDate data_za, double vsego_lek, double vsego_labor, double vsego_prak, double vsego_on_line, double vsego_off_line, double vsego_kontr_rab, double vsego_ekzamen, double vsego_kurs_rab, QString yazyk, double vsego)
{
    QSqlQuery q;
    q.prepare("INSERT INTO is_spravki_pochas "
              "(nomer_spravki, facultet, kafedra_id, sotr_id, nomer_protocola, data_za, vsego_lek, vsego_labor, vsego_prak, vsego_on_line, vsego_off_line, vsego_kontr_rab, vsego_ekzamen, vsego_kurs_rab, yazyk, vsego) VALUES "
              "(:nomer_spravki, :facultet, :kafedra_id, :sotr_id, :nomer_protocola, :data_za, :vsego_lek, :vsego_labor, :vsego_prak, :vsego_on_line, :vsego_off_line, :vsego_kontr_rab, :vsego_ekzamen, :vsego_kurs_rab, :yazyk, :vsego)");
    q.bindValue(":nomer_spravki", nomer_spravki);
    q.bindValue(":facultet", facultet);
    q.bindValue(":kafedra_id", kafedra_id);
    q.bindValue(":sotr_id", sotr_id);
    q.bindValue(":nomer_protocola", nomer_protocola);
    q.bindValue(":data_za", data_za);
    q.bindValue(":vsego_lek", vsego_lek);
    q.bindValue(":vsego_labor", vsego_labor);
    q.bindValue(":vsego_prak", vsego_prak);
    q.bindValue(":vsego_on_line", vsego_on_line);
    q.bindValue(":vsego_off_line", vsego_off_line);
    q.bindValue(":vsego_kontr_rab", vsego_kontr_rab);
    q.bindValue(":vsego_ekzamen", vsego_ekzamen);
    q.bindValue(":vsego_kurs_rab", vsego_kurs_rab);
    q.bindValue(":yazyk", yazyk);
    q.bindValue(":vsego", vsego);
    q.exec();
    qDebug()<<q.lastError();
    return q.lastInsertId().toInt();
}

bool Dal_prepodcontrol::editSpravkaPochas(int id_sparvki_pochas, int nomer_spravki, QString facultet, int kafedra_id, int sotr_id, int nomer_protocola, QDate data_za, double vsego_lek, double vsego_labor, double vsego_prak, double vsego_on_line, double vsego_off_line, double vsego_kontr_rab, double vsego_ekzamen, double vsego_kurs_rab, QString yazyk, double vsego)
{
    QSqlQuery q;
    q.prepare("UPDATE is_spravki_pochas SET id_sparvki_pochas = :id_sparvki_pochas, nomer_spravki = :nomer_spravki, facultet = :facultet, kafedra_id = :kafedra_id,"
              " sotr_id = :sotr_id, nomer_protocola = :nomer_protocola, data_za = :data_za, vsego_lek = :vsego_lek, vsego_labor = :vsego_labor, vsego_prak = :vsego_prak, "
              " vsego_on_line = :vsego_on_line, vsego_off_line = :vsego_off_line, vsego_kontr_rab = :vsego_kontr_rab, vsego_ekzamen = :vsego_ekzamen, vsego_kurs_rab = :vsego_kurs_rab, yazyk = :yazyk, vsego = :vsego WHERE id_sparvki_pochas = :id_sparvki_pochas");
    q.bindValue(":nomer_spravki", nomer_spravki);
    q.bindValue(":facultet", facultet);
    q.bindValue(":kafedra_id", kafedra_id);
    q.bindValue(":sotr_id", sotr_id);
    q.bindValue(":nomer_protocola", nomer_protocola);
    q.bindValue(":data_za", data_za);
    q.bindValue(":vsego_lek", vsego_lek);
    q.bindValue(":vsego_labor", vsego_labor);
    q.bindValue(":vsego_prak", vsego_prak);
    q.bindValue(":vsego_on_line", vsego_on_line);
    q.bindValue(":vsego_off_line", vsego_off_line);
    q.bindValue(":vsego_kontr_rab", vsego_kontr_rab);
    q.bindValue(":vsego_ekzamen", vsego_ekzamen);
    q.bindValue(":vsego_kurs_rab", vsego_kurs_rab);
    q.bindValue(":yazyk", yazyk);
    q.bindValue(":vsego", vsego);
    q.bindValue(":id_sparvki_pochas", id_sparvki_pochas);
    qDebug()<<q.lastError();
    return q.exec();
}

bool Dal_prepodcontrol::addSpravkiDetails(QString daty, int gruppa_id, int discipline_id, double lekcii, double labor, double prak, double on_line, double off_line, double kontr_rab, double ekzamen, double kurs_rab, int sparvki_pochas_id)
{
    QSqlQuery q;
    q.prepare("INSERT INTO is_spravki_details "
              "(daty, gruppa_id, discipline_id, lekcii, labor, prak, on_line, off_line, kontr_rab, ekzamen, kurs_rab, sparvki_pochas_id) VALUES"
              "(:daty, :gruppa_id, :discipline_id, :lekcii, :labor, :prak, :on_line, :off_line, :kontr_rab, :ekzamen, :kurs_rab, :sparvki_pochas_id)");
    q.bindValue(":daty", daty);
    q.bindValue(":gruppa_id", gruppa_id);
    q.bindValue(":discipline_id", discipline_id);
    q.bindValue(":lekcii", lekcii);
    q.bindValue(":labor", labor);
    q.bindValue(":prak", prak);
    q.bindValue(":on_line", on_line);
    q.bindValue(":off_line", off_line);
    q.bindValue(":kontr_rab", kontr_rab);
    q.bindValue(":ekzamen", ekzamen);
    q.bindValue(":kurs_rab", kurs_rab);
    q.bindValue(":sparvki_pochas_id", sparvki_pochas_id);
    qDebug()<<q.lastError();
    return q.exec();
}

bool Dal_prepodcontrol::deleteSpravkiDetails(int id_sparvki_pochas)
{
    QSqlQuery q;
    q.prepare("DELETE FROM is_spravki_details WHERE sparvki_pochas_id = :sparvki_pochas_id)");
    q.bindValue(":sparvki_pochas_id", id_sparvki_pochas);
    return q.exec();
}
