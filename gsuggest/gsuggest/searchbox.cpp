#include "searchbox.h"
#include <QWidget>
#include <QDesktopServices>
#include <QUrl>
#include "gsuggestion.h"

SearchBox::SearchBox(QWidget *parent): QLineEdit(parent)
{
    completer = new GSuggestion(this);

    connect(this, SIGNAL(returnPressed()),this, SLOT(doSearch()));

    setFocus();
}

void SearchBox::doSearch()
{
    completer->preventSuggest();
    QString urlGoogleQuery = "http://google.ru/search?q=%1";
    QString url = QString(urlGoogleQuery).arg(text());///????
    QDesktopServices::openUrl(QUrl(url));
}
