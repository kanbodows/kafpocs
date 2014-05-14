#-------------------------------------------------
#
# Project created by QtCreator 2014-02-09T00:30:37
#
#-------------------------------------------------

QT       += core gui sql printsupport network
greaterThan(QT_MAJOR_VERSION, 4): QT += widgets
TARGET = poks
TEMPLATE = app
CONFIG -= static
 CONFIG += shared
SOURCES += main.cpp\
   delegates/qlightboxwidget.cpp \
        mainwindow.cpp \
    accessdb.cpp \
    dals/dal_main.cpp \
    checker.cpp \
    styles.cpp \
    spravochniki/course.cpp \
    spravochniki/semestr.cpp \
    delegates/textnndelegate.cpp \
    spravochniki/discipline.cpp \
    groups.cpp \
    spravochniki/formaobucheniya.cpp \
    spravochniki/formoplaty.cpp \
    themediplomproekta.cpp \
    spravochniki/doljnost.cpp \
    konf_sem.cpp \
    delegates/datedelegate.cpp \
    kur_rab.cpp \
    spravochniki/lic_doc_types.cpp \
    spravochniki/lic_types.cpp \
    spravochniki/poruch_status.cpp \
    spravochniki/zvaniya.cpp \
    spravochniki/voenkomat.cpp \
    spravochniki/tip_voprosa.cpp \
    spravochniki/prize_types.cpp \
    spravochniki/stip_type.cpp \
    spravochniki/prac_type.cpp \
    lic_docs.cpp \
    spravochniki/met_types.cpp \
    issport.cpp \
    main_journal.cpp \
    stipendiya.cpp\
    metodichki.cpp \
    student.cpp \
    delegates/checkboxdelegate.cpp \
    dals/dal_studentscontrol.cpp \
    statiya.cpp \
    add_edit_forms/addoreditstat.cpp \
    dals/dal_prepodcontrol.cpp \
    poruchenia.cpp \
    olimpiada.cpp \
    add_edit_forms/addoredit_konf_sem.cpp\
    add_edit_forms/addoreditolimp.cpp \
    poseshenia.cpp \
    obchejitie.cpp \
    add_edit_forms/addoredit_obchejitie.cpp \
    add_edit_forms/addoredit_student.cpp \
    add_edit_forms/addoreditvipusknik.cpp \
    partners.cpp \
    actvnedrenia.cpp \
    add_edit_forms/addoreditactvnedrenia.cpp \
    pracwork.cpp \
    add_edit_forms/addoreditpracwork.cpp \
    vakansii.cpp \
    add_edit_forms/addoreditsubwork.cpp \
    smtp.cpp \
    add_edit_forms/addoredit_discipline.cpp \
    otpravkasoobshvipusk.cpp \
    authorization.cpp \
    dals/dal_acl.cpp \
    add_edit_forms/addoredit_metodichki.cpp \
    sotrudniki.cpp \
    add_edit_forms/addoredit_knigi.cpp\
    add_edit_forms/addoreditsotrudnik.cpp \
    knigi.cpp \
    smenaparrolya.cpp \
    nirsotrudnika.cpp \
    publicationnir.cpp \
    add_edit_forms/addoreditpublicationnir.cpp \
    boleznisotrudnikov.cpp\
    add_edit_forms/addoredit_prepoddiscipline.cpp \
    sponsors.cpp \
    delegates/word_wrap_delegate.cpp \
    povyshkval.cpp \
    add_edit_forms/addoredit_povyshkval.cpp \
    reports/raiting.cpp \
    nagradi.cpp \
    add_edit_forms/addoredit_nagradi.cpp \
    add_edit_forms/addoredit_poruchenie.cpp \
    prosmotrporuchstatus.cpp \
    delegates/readbutton.cpp \
    add_edit_forms/edit_poruchstatus.cpp \
    gak.cpp \
    sostavkomissii.cpp \
    add_edit_forms/addoreditsostavkom.cpp \
    add_edit_forms/addoreditgak.cpp\
    uvedomlenie.cpp \
    raspisanie.cpp \
    add_edit_forms/addoredir_raspisanie.cpp \
    jurnalvzaimoposesheniya.cpp \
    add_edit_forms/addoreditjurnvzaimopos.cpp \
    reports/otchetstarostagruppi.cpp \
    myprint.cpp \
    course_work.cpp \
    add_edit_forms/addoredit_course_work.cpp \
    add_edit_forms/addoredit_srez.cpp \
    add_edit_forms/addoredit_srez_bally.cpp \
    diploms.cpp \
    add_edit_forms/addoreditdiplom.cpp \
    prostavitocenki.cpp \
    delegates/readonlydelegate.cpp \
    reports/vipusk_ki.cpp \
    delegates/transactiondatedelegate.cpp \
    srezy.cpp \
    trudoustroistvovipusknikov.cpp \
    add_edit_forms/addoredittrudovip.cpp \
    reports/otchettrudoustrvip.cpp \
    delegates/doublespinboxdelegate.cpp \
    boleznistudentov.cpp \
    add_edit_forms/addoreditboleznistud.cpp \
    otpravkasoobsheniasotrudnikam.cpp\
    reports/otchetsrezbally.cpp \
    otpravkasoobshstudentam.cpp \
    godasozdplanakaf.cpp \
    add_edit_forms/addoreditplanzaskaf.cpp\
    delegates/qcustomplot.cpp \
    plots.cpp \
    prosmotrplanazaskaf.cpp\
    reports/otchetformacontrolya.cpp \
    reports/otchetpraktiki.cpp \
    reports/otchetstudenty.cpp \
    reports/svodotchetpraktiki.cpp \
    proekty.cpp \
    godasozdaniagrafikavzaiomopos.cpp \
    prosmotrgrafvzaimopos.cpp \
    reports/otchetpovyshkval.cpp \
    add_edit_forms/addgrafvzaimopos.cpp \
    add_edit_forms/addoredit_vedomostikursovye.cpp \
    vedomostikursovye.cpp \
    spravochniki/kafedry.cpp \
    spravochniki/specialnosti.cpp \
    delegates/transactiondatedelegatewithousec.cpp\
    reports/otchetsotrudniki.cpp \
    add_edit_forms/addoredit_aktykursovye.cpp \
    add_edit_forms/addoredit_spravkapochas.cpp \
    spravkipochas.cpp \
    platnieobrazovatuslgi.cpp \
    add_edit_forms/add_or_edit_platnieobrazovatkursi.cpp \
    delegates/comboboxitemdelegate.cpp \
    reports/otchetspravkipochas.cpp \
    add_edit_forms/addoredit_gak_bally.cpp \
    delegates/spinboxdelegate.cpp \
    reports/svodotchetgakbally.cpp \
    planumr.cpp \
    add_edit_forms/addoreditplanumr.cpp \
    procentovka.cpp \
    add_edit_forms/add_or_edit_procentovka.cpp \
    prosmotr_redaktirovanie_procentovok.cpp

HEADERS  += mainwindow.h \
    delegates/qlightboxwidget.h \
    accessdb.h \
    dals/dal_main.h \
    checker.h \
    styles.h \
    spravochniki/course.h \
    spravochniki/semestr.h \
    spravochniki/discipline.h \
    groups.h \
    spravochniki/formaobucheniya.h \
    spravochniki/formoplaty.h \
    themediplomproekta.h \
    spravochniki/doljnost.h \
    konf_sem.h \
    delegates/datedelegate.h \
    kur_rab.h \
    spravochniki/lic_doc_types.h \
    spravochniki/lic_types.h \
    spravochniki/poruch_status.h \
    spravochniki/zvaniya.h \
    spravochniki/voenkomat.h \
    spravochniki/tip_voprosa.h \
    spravochniki/prize_types.h \
    spravochniki/stip_type.h \
    spravochniki/prac_type.h \
    lic_docs.h \
    spravochniki/met_types.h \
    issport.h \
    main_journal.h \
    stipendiya.h\
    metodichki.h \
    student.h \
    delegates/checkboxdelegate.h \
    dals/dal_studentscontrol.h \
    statiya.h \
    add_edit_forms/addoreditstat.h \
    dals/dal_prepodcontrol.h \
    poruchenia.h \
    olimpiada.h \
    add_edit_forms/addoredit_konf_sem.h\
    add_edit_forms/addoreditolimp.h \
    poseshenia.h \
    obchejitie.h \
    add_edit_forms/addoredit_obchejitie.h \
    add_edit_forms/addoredit_student.h \
    add_edit_forms/addoreditvipusknik.h \
    partners.h \
    actvnedrenia.h \
    add_edit_forms/addoreditactvnedrenia.h \
    pracwork.h \
    add_edit_forms/addoreditpracwork.h \
    vakansii.h \
    add_edit_forms/addoreditsubwork.h \
    smtp.h \
    add_edit_forms/addoredit_discipline.h \
    otpravkasoobshvipusk.h \
    authorization.h \
    dals/dal_acl.h \
    add_edit_forms/addoredit_metodichki.h \
    add_edit_forms/addoredit_knigi.h \
    sotrudniki.h \
    knigi.h \
    add_edit_forms/addoreditsotrudnik.h \
    smenaparrolya.h \
    nirsotrudnika.h \
    publicationnir.h \
    add_edit_forms/addoreditpublicationnir.h \
    boleznisotrudnikov.h\
    add_edit_forms/addoredit_prepoddiscipline.h \
    sponsors.h \
    delegates/textnndelegate \
    delegates/word_wrap_delegate.h \
    povyshkval.h \
    add_edit_forms/addoredit_povyshkval.h \
    reports/raiting.h \
    nagradi.h \
    add_edit_forms/addoredit_nagradi.h \
    add_edit_forms/addoredit_poruchenie.h \
    prosmotrporuchstatus.h \
    delegates/readbutton.h \
    add_edit_forms/edit_poruchstatus.h \
    gak.h \
    sostavkomissii.h \
    add_edit_forms/addoreditsostavkom.h \
    add_edit_forms/addoreditgak.h\
    uvedomlenie.h \
    raspisanie.h \
    add_edit_forms/addoredir_raspisanie.h \
    jurnalvzaimoposesheniya.h \
    add_edit_forms/addoreditjurnvzaimopos.h \
    reports/otchetstarostagruppi.h \
    myprint.h \
    course_work.h \
    add_edit_forms/addoredit_course_work.h \
    add_edit_forms/addoredit_srez.h \
    diploms.h \
    add_edit_forms/addoredit_srez_bally.h \
    add_edit_forms/addoreditdiplom.h \
    prostavitocenki.h \
    delegates/readonlydelegate.h \
    reports/vipusk_ki.h \
    delegates/transactiondatedelegate.h \
    srezy.h \
    trudoustroistvovipusknikov.h \
    add_edit_forms/addoredittrudovip.h \
    reports/otchettrudoustrvip.h \
    delegates/doublespinboxdelegate.h \
    boleznistudentov.h \
    add_edit_forms/addoreditboleznistud.h \
    otpravkasoobsheniasotrudnikam.h\
    reports/otchetsrezbally.h \
    otpravkasoobshstudentam.h \
    godasozdplanakaf.h \
    add_edit_forms/addoreditplanzaskaf.h\
    delegates/qcustomplot.h \
    plots.h \
    prosmotrplanazaskaf.h\
    reports/otchetformacontrolya.h \
    reports/otchetpraktiki.h \
    reports/otchetstudenty.h \
    godasozdaniagrafikavzaiomopos.h \
    prosmotrgrafvzaimopos.h \
    add_edit_forms/addgrafvzaimopos.h\
    reports/svodotchetpraktiki.h \
    proekty.h \
    reports/otchetpovyshkval.h \
    add_edit_forms/addoredit_vedomostikursovye.h \
    vedomostikursovye.h \
    spravochniki/kafedry.h \
    spravochniki/specialnosti.h \
    delegates/transactiondatedelegatewithousec.h\
    reports/otchetsotrudniki.h \
    add_edit_forms/addoredit_aktykursovye.h \
    add_edit_forms/addoredit_spravkapochas.h \
    spravkipochas.h \
    delegates/comboboxitemdelegate.h \
    platnieobrazovatuslgi.h \
    add_edit_forms/add_or_edit_platnieobrazovatkursi.h \
    reports/otchetspravkipochas.h \
    add_edit_forms/addoredit_gak_bally.h \
    delegates/spinboxdelegate.h \
    reports/svodotchetgakbally.h \
    planumr.h \
    add_edit_forms/addoreditplanumr.h \
    procentovka.h \
    add_edit_forms/add_or_edit_procentovka.h \
    prosmotr_redaktirovanie_procentovok.h

FORMS    += mainwindow.ui \
    accessdb.ui \
    spravochniki/course.ui \
    spravochniki/semestr.ui \
    spravochniki/discipline.ui \
    groups.ui \
    spravochniki/formaobucheniya.ui \
    spravochniki/formoplaty.ui \
    themediplomproekta.ui \
    spravochniki/doljnost.ui \
    konf_sem.ui \
    kur_rab.ui \
    spravochniki/lic_doc_types.ui \
    spravochniki/lic_types.ui \
    spravochniki/poruch_status.ui \
    spravochniki/zvaniya.ui \
    spravochniki/voenkomat.ui \
    spravochniki/tip_voprosa.ui \
    spravochniki/prize_types.ui \
    spravochniki/stip_type.ui \
    spravochniki/prac_type.ui \
    lic_docs.ui \
    spravochniki/met_types.ui \
    issport.ui \
    main_journal.ui \
    stipendiya.ui\
    metodichki.ui \
    student.ui \
    statiya.ui \
    add_edit_forms/addoreditstat.ui \
    poruchenia.ui \
    olimpiada.ui \
    add_edit_forms/addoredit_konf_sem.ui\
    add_edit_forms/addoreditolimp.ui \
    poseshenia.ui \
    obchejitie.ui \
    add_edit_forms/addoredit_obchejitie.ui \
    add_edit_forms/addoredit_student.ui \
    add_edit_forms/addoreditvipusknik.ui \
    partners.ui \
    actvnedrenia.ui \
    add_edit_forms/addoreditactvnedrenia.ui \
    pracwork.ui \
    add_edit_forms/addoreditpracwork.ui \
    vakansii.ui \
    add_edit_forms/addoreditsubwork.ui \
    add_edit_forms/addoredit_discipline.ui \
    otpravkasoobshvipusk.ui \
    authorization.ui \
    add_edit_forms/addoredit_metodichki.ui \
    add_edit_forms/addoredit_knigi.ui\
    sotrudniki.ui \
    knigi.ui\
    add_edit_forms/addoreditsotrudnik.ui\
    smenaparrolya.ui \
    nirsotrudnika.ui \
    publicationnir.ui \
    add_edit_forms/addoreditpublicationnir.ui \
    boleznisotrudnikov.ui\
    add_edit_forms/addoredit_prepoddiscipline.ui \
    sponsors.ui \
    povyshkval.ui \
    add_edit_forms/addoredit_povyshkval.ui \
    reports/raiting.ui \
    nagradi.ui \
    add_edit_forms/addoredit_nagradi.ui \
    add_edit_forms/addoredit_poruchenie.ui \
    prosmotrporuchstatus.ui \
    add_edit_forms/edit_poruchstatus.ui \
    gak.ui \
    sostavkomissii.ui \
    add_edit_forms/addoreditsostavkom.ui \
    add_edit_forms/addoreditgak.ui\
    uvedomlenie.ui \
    raspisanie.ui \
    add_edit_forms/addoredir_raspisanie.ui \
    jurnalvzaimoposesheniya.ui \
    add_edit_forms/addoreditjurnvzaimopos.ui \
    reports/otchetstarostagruppi.ui \
    course_work.ui \
    add_edit_forms/addoredit_course_work.ui \
    diploms.ui \
    add_edit_forms/addoreditdiplom.ui \
    prostavitocenki.ui \
    reports/vipusk_ki.ui \
    srezy.ui \
    add_edit_forms/addoredit_srez.ui \
    add_edit_forms/addoredit_srez_bally.ui \
    trudoustroistvovipusknikov.ui \
    add_edit_forms/addoredittrudovip.ui \
    reports/otchettrudoustrvip.ui \
    boleznistudentov.ui \
    add_edit_forms/addoreditboleznistud.ui \
    otpravkasoobsheniasotrudnikam.ui\
    reports/otchetsrezbally.ui \
    otpravkasoobshstudentam.ui \
    godasozdplanakaf.ui \
    add_edit_forms/addoreditplanzaskaf.ui \
    prosmotrplanazaskaf.ui\
    reports/otchetformacontrolya.ui \
    reports/otchetpraktiki.ui \
    reports/otchetstudenty.ui \
    reports/svodotchetpraktiki.ui \
    proekty.ui \
    godasozdaniagrafikavzaiomopos.ui \
    prosmotrgrafvzaimopos.ui \
    reports/otchetpovyshkval.ui\
    add_edit_forms/addgrafvzaimopos.ui \
    add_edit_forms/addoredit_vedomostikursovye.ui \
    vedomostikursovye.ui \
    spravochniki/kafedry.ui \
    spravochniki/specialnosti.ui \
    reports/otchetsotrudniki.ui \
    add_edit_forms/addoredit_aktykursovye.ui \
    add_edit_forms/addoredit_spravkapochas.ui \
    spravkipochas.ui \
    platnieobrazovatuslgi.ui \
    add_edit_forms/add_or_edit_platnieobrazovatkursi.ui \
    reports/otchetspravkipochas.ui \
    add_edit_forms/addoredit_gak_bally.ui \
    reports/svodotchetgakbally.ui \
    planumr.ui \
    add_edit_forms/addoreditplanumr.ui \
    procentovka.ui \
    add_edit_forms/add_or_edit_procentovka.ui \
    prosmotr_redaktirovanie_procentovok.ui


RESOURCES += \
    resourses.qrc

OTHER_FILES += \
    img/loader.gif \
    img/1downarrow.png \
    img/3.png \
    img/570536.png \
    img/581222.png \
    img/581223.png \
    img/581231.png \
    img/581237.png \
    img/581266.png \
    img/581282.png \
    img/581287.png \
    img/581288.png \
    img/581292.png \
    img/581300.png \
    img/581301.png \
    img/581302.png \
    img/581303.png \
    img/1074754.png \
    img/1115541.png \
    img/1119942.png \
    img/about.png \
    img/anemo.png \
    img/AnemoLogo.png \
    img/ExitN.png \
    img/file.png \
    img/filter_list.png \
    img/ok.png \
    img/order.png \
    img/order1.png \
    img/printing.png \
    src/src.pri \
    src/src.pro.user

