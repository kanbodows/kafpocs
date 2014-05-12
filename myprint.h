#ifndef MYPRINT_H
#define MYPRINT_H
#include <QtPrintSupport>
#include <QString>
#include <QFile>
#include <QtSql>
#include <QMap>
#include "styles.h"
#define RowList QMap <int, QString>

class MyPrint
{
public:
    MyPrint();
    void print(QString body);
    QTextDocument *print(QSqlQueryModel *model, RowList colHeaders, QString title, QString footer, int groupCols);
    QString printOnlyTable(QSqlQueryModel *model, RowList colHeaders, int groupCols = 0);
    static const int ReplaceBr = 1;
    Styles* css;
};

#endif // MYPRINT_H
