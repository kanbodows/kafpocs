#include "myprint.h"
#include <QTextEdit>
MyPrint::MyPrint()
{
    css = new Styles;
}
void MyPrint::print(QString body)
{
    QPrinter printer(QPrinter::HighResolution);
    printer.setPageSize(QPrinter::A4);
    //printer.setFullPage(true);
    QPrintDialog dlg(&printer);
    dlg.exec();
    QTextEdit toPrint;
    toPrint.insertHtml("<html><head><meta charset=utf-8><head><body>");
    toPrint.insertHtml(body);
    toPrint.insertHtml("</body><html>");
    toPrint.print(&printer);
}

QTextDocument *MyPrint::print(QSqlQueryModel *model, RowList colHeaders, QString title, QString footer, int groupCols = 0)
{
    QString body;
    QStringList colls;
    body.append("<html><head>"
                "<link rel='stylesheet' type='text/css' href='format.css'>"
                "</head><body>");
    if (title!="")
        body.append("<center><H2>" + title + "</H2></center><br>");
    body.append("<center><table border='1' class = 'mine'><tr>");
    RowList::const_iterator i = colHeaders.constBegin();
    while (i != colHeaders.constEnd()) {
        colls.append(QString::number(i.key()));
        body.append("<th>" + i.value() + "</th>\n");
        ++i;
    }
    body.append("</tr>\n");
    while (model->canFetchMore())
        model->fetchMore();
    QMap<int,QString> groupTitles; // заголовки групп groupTitles(номер колонки, название группы)
    QMap<int,int> groupCounts; // количество элементов в группе groupCounts(номер колонки, кол-во)
    for (int index = 0; index < model->rowCount(); index++)
    {
        body.append("<tr>\n");
        for (int listId = 0; listId < colls.count(); listId++)
        {
            if(listId < groupCols) // если колонки нужно сгруппировать(если это групируемая колонка)
            {
                if(groupTitles.value(listId) != model->record(index).value(colls[listId].toInt()).toString())
                {
                    groupTitles[listId] = model->record(index).value(colls[listId].toInt()).toString();
                    body.append("<td class='grHead' colspan = '"+QString::number(colls.count()-listId)+"'><b>");
                    body.append(groupTitles[listId]);
                    body.append("</b></td>\n</tr>\n<tr>\n");
                    if(listId == groupCols-1)
                        for(int i=0; i < listId + 1; i++)
                            body.append("<td>1</td>\n");
                    else
                        for(int i=0; i < listId + 1; i++)
                            body.append("<td></td>\n");
                    groupCounts[listId] = 1;
                }
                else
                {
                    groupCounts[listId] = groupCounts[listId] + 1;
                    body.append("<td>" + QString::number(groupCounts[listId]) + "</td>\n");
                }
            }
            else
            {
                body.append("<td>");
                body.append(model->record(index).value(colls[listId].toInt()).toString());
                body.append("</td>\n");
            }
        }
        body.append("</tr>\n");
    }
    body.append("</table></center>");
    if (footer!="")

        body.append("<center>" + footer + "</center>");

    body.append("</body></html>");
//    qDebug()<<body;
    QTextDocument *toPrint = new QTextDocument;
    //    toPrint->addResource(QTextDocument::ImageResource, QUrl( "bg.png" ), QPixmap(":/img/img/institution_icon.png") );
    toPrint->addResource(QTextDocument::StyleSheetResource, QUrl( "format.css" ), css->reportCss);
    toPrint->setHtml(body);
    return toPrint;
}


QString MyPrint::printOnlyTable(QSqlQueryModel *model, RowList colHeaders, int groupCols)
{
    QString body;
    QStringList colls;
    body.append("<table border='1' class = 'mine'><tr>");
    RowList::const_iterator i = colHeaders.constBegin();
    while (i != colHeaders.constEnd()) {
        colls.append(QString::number(i.key()));
        body.append("<th>" + i.value() + "</th>\n");
        ++i;
    }
    body.append("</tr>\n");
    while (model->canFetchMore())
        model->fetchMore();
    QMap<int,QString> groupTitles; // заголовки групп groupTitles(номер колонки, название группы)
    QMap<int,int> groupCounts; // количество элементов в группе groupCounts(номер колонки, кол-во)
    for (int index = 0; index < model->rowCount(); index++)
    {
        body.append("<tr>\n");
        for (int listId = 0; listId < colls.count(); listId++)
        {
            if(listId < groupCols) // если колонки нужно сгруппировать(если это групируемая колонка)
            {
                if(groupTitles.value(listId) != model->record(index).value(colls[listId].toInt()).toString())
                {
                    groupTitles[listId] = model->record(index).value(colls[listId].toInt()).toString();
                    body.append("<td class='grHead' colspan = '"+QString::number(colls.count()-listId)+"'><b>");
                    body.append(groupTitles[listId]);
                    body.append("</b></td>\n</tr>\n<tr>\n");
                    if(listId == groupCols-1)
                        for(int i=0; i < listId + 1; i++)
                            body.append("<td>1</td>\n");
                    else
                        for(int i=0; i < listId + 1; i++)
                            body.append("<td></td>\n");
                    groupCounts[listId] = 1;
                }
                else
                {
                    groupCounts[listId] = groupCounts[listId] + 1;
                    body.append("<td>" + QString::number(groupCounts[listId]) + "</td>\n");
                }
            }
            else
            {
                body.append("<td>");
                body.append(model->record(index).value(colls[listId].toInt()).toString());
                body.append("</td>\n");
            }
        }
        body.append("</tr>\n");
    }
    body.append("</table>");
    return body;
}
