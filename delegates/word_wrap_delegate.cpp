#include <QPlainTextEdit>

#include "word_wrap_delegate.h"
#include <qpainter.h>
#include <QStyleOptionViewItem>
#include <qdebug.h>

TextEditDelegate::TextEditDelegate(QObject *parent)
    : QItemDelegate(parent)
{
}

QWidget *TextEditDelegate::createEditor(QWidget *parent,
    const QStyleOptionViewItem &/* option */,
    const QModelIndex &/* index */) const
{
    QPlainTextEdit *editor = new QPlainTextEdit(parent);

    return editor;
}

void TextEditDelegate::drawDisplay(QPainter *painter, const QStyleOptionViewItem &option,
                                   const QRect &rect, const QString &text) const
{
    QPalette::ColorGroup cg = option.state & QStyle::State_Enabled
                              ? QPalette::Normal : QPalette::Disabled;
    if (cg == QPalette::Normal && !(option.state & QStyle::State_Active))
        cg = QPalette::Inactive;
    if (option.state & QStyle::State_Selected) {
            painter->setPen(option.palette.color(cg, QPalette::HighlightedText));
        }
        else {
            painter->setPen(option.palette.color(cg, QPalette::Text));
        }

    if (text.isEmpty())
        return;

    if (option.state & QStyle::State_Editing) {
        painter->save();
        painter->setPen(option.palette.color(cg, QPalette::Text));
        painter->drawRect(rect.adjusted(0, 0, -1, -1));
        painter->restore();
    }
    QTextOption opt(option.displayAlignment);
    opt.setWrapMode(QTextOption::WordWrap);
    painter->save();
    painter->setPen(option.palette.color(cg, QPalette::Text));
    painter->drawText(rect.adjusted(1, 0, 0, 0), text, opt);
    painter->restore();
}

QSize TextEditDelegate::sizeHint(const QStyleOptionViewItem &option,
                                 const QModelIndex &index) const
{
    int width = option.rect.width() -1;
    int spW = option.fontMetrics.width(QChar(' '));
    QStringList words = index.data().toString().split(QChar(' '));
    int currRow = 0;
    int currCol = 0;
    QString clipString;
    int count = words.size();
    int wcount = 0;
    for (int i = 0; i < count; ++i)
    {
        int currW = option.fontMetrics.width(words[i]);
        if (currCol + currW < width)
        {
            currCol += currW + spW;
            clipString += words[i] + QString(" ");
            ++wcount;
        }
        else
        {
            if (wcount)
            {
               clipString += QString("\n");
                currCol = currW + spW;
                wcount = 1;
                clipString += words[i] + QString(" ");
                ++currRow;
            }
            else
            {
                clipString += words[i];
                if(i%2!=0)
                clipString += QString("\n");
                currCol = 0;
                wcount = 0;
                ++currRow;
            }
        }
    }
    return option.fontMetrics.size(Qt::TextDontClip, clipString);
}
