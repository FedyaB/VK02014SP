#include "gsuggestion.h"
#include <QTimer>
#include <QtWidgets/QTreeWidget>
#include <QEvent>
#include <QStringList>
#include <QtNetwork/QNetworkRequest>
#include <QtNetwork/QNetworkReply>
#include <QtWidgets/QLineEdit>
#include <QKeyEvent>
#include <QXmlStreamReader>
#include <QByteArray>
#include <QtWidgets/QHeaderView>
/*
 *   QLineEdit -------- editor;
 *   QTreeWidget ------ popup;
 *   QTimer ----------- timer;
 *   QNetworkAccessManager networkManager;
 */

GSuggestion::GSuggestion(QLineEdit *parent): QObject(parent), editor(parent)
{
    popup = new QTreeWidget;
    popup->setWindowFlags(Qt::Popup);
    popup->header()->hide();
    popup->setFocusPolicy(Qt::NoFocus);
    popup->setFocusProxy(parent);
    popup->setSelectionBehavior(QTreeWidget::SelectRows);

    popup->installEventFilter(this);

    connect(popup,SIGNAL(itemClicked(QTreeWidgetItem*,int)), SLOT(choosingDone()));

    timer = new QTimer(this);
    timer->setSingleShot(true);
    timer->setInterval(500);

    connect(timer, SIGNAL(timeout()), SLOT(autoSuggest()));
    connect(editor, SIGNAL(textEdited(QString)), timer, SLOT( start() ));

    connect(&networkManager, SIGNAL(finished(QNetworkReply*)), this, SLOT(handleNetworkData(QNetworkReply*)));
}

GSuggestion::~GSuggestion()
{
    delete popup;
}

bool GSuggestion::eventFilter(QObject *focusedObject, QEvent *ev)
{
    if (focusedObject != popup)
        return false;

    if (ev -> type() == QEvent::MouseButtonPress)
    {
        popup -> hide();
        editor -> setFocus();
        return true;
    }

    if (ev -> type() == QEvent::KeyPress)
    {
        bool consumed = false;
        int key = static_cast<QKeyEvent*> (ev) -> key();
        switch(key)
        {
            case Qt::Key_Enter:
            case Qt::Key_Return:
                                   choosingDone();

            case Qt::Key_Escape:
                                   editor -> setFocus();
                                   popup -> hide();
                                   consumed = true;

            case Qt::Key_Up:
            case Qt::Key_Down:
            case Qt::Key_Home:
            case Qt::Key_End:
            case Qt::Key_PageUp:
            case Qt::Key_PageDown:
                                   break;

            default:
                                   editor -> setFocus();
                                   editor -> event(ev);
                                   popup -> hide();
                                   break;
        }
        return consumed;
    }

    return false;
}

void GSuggestion::showSuggestions(const QStringList &choices)
{
    if(choices.isEmpty())
        return;


    popup->setUpdatesEnabled(false);
    popup->clear();
    for (int i = 0; i < choices.count(); ++i)
    {
            QTreeWidgetItem *item;
            item = new QTreeWidgetItem(popup);
            item->setText(0, choices[i]);

    }
    popup->setCurrentItem(popup->topLevelItem(0));
    popup->resizeColumnToContents(0);
    popup->adjustSize();
    popup->setUpdatesEnabled(true);

    int h = popup->sizeHintForRow(0) * qMin(7, choices.count()) + 3;
    popup->resize(popup->width(), h);

    popup->move(editor->mapToGlobal(QPoint(0, editor->height())));
    popup->setFocus();
    popup->show();
}

void GSuggestion::choosingDone()
{
    popup -> hide();
    editor -> setFocus();
    timer -> stop();
    QTreeWidgetItem *item = popup->currentItem();
    if (item) {
                editor->setText(item->text(0));
                QMetaObject::invokeMethod(editor, "returnPressed");
              }
}

void GSuggestion::autoSuggest()
{
    QString str = editor->text();
    QString urlGoogleQuery = "http://google.com/complete/search?output=toolbar&q=%1";
    QString url = QString(urlGoogleQuery).arg(str);
    networkManager.get(QNetworkRequest(QString(url)));
}

void GSuggestion::preventSuggest()
{
    timer->stop();
}

void GSuggestion::handleNetworkData(QNetworkReply *networkReply)
{
    if (!networkReply->error())
    {
        QStringList choices;

        QByteArray response(networkReply->readAll());
        QXmlStreamReader xml(response);
        while (!xml.atEnd())
        {
            xml.readNext();
            if (xml.tokenType() == QXmlStreamReader::StartElement)
                if (xml.name() == "suggestion")
                {
                    QStringRef str = xml.attributes().value("data");
                    choices << str.toString();
                }
        }

        showSuggestions(choices);
    }
    networkReply->deleteLater();
}
