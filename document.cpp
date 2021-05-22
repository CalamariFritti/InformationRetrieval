/*
 * This is a document. It can have a title and a content.
 * There are two actions that can be performed at the moment:
 *  1. elimination of stop words, it is neccessary to provide a list of stop words
 *  2. reduction to base form (NOT IMPLEMENTED YET!)
 */

#include <QWidget>
#include <QDebug>

#include "document.h"
#include "mainwindow.h"

Document::Document()
{

}

QString Document::getContent()
{
    return this->content;
}

QString Document::getTitle()
{
    return this->title;
}

void Document::setContent(QString content)
{
    this->content = content;
}

void Document::setTitle(QString title)
{
    this->title = title;
}


/*
 *  parameter: Document-type, contents is a list of stopwords
 *
 *  Eliminates all words from stopWordList from and creates a new document.
 *  Content of this document is transformed to ignore case, remove special
 *  characters, etc.
 *  Then an iteration over every entry in the stop word list eliminates all
 *  stop words.
 */
void Document::stopWordElimination(Document stopWords)
{
   qDebug() << "original content:\n" << content;

    QList<QString> stopWordList = stopWords.getContent().toLower().split("\n");
    QListIterator<QString> stopWordListIterator(stopWordList);
    //treatment for apostroph, remove apostroph plus following letter
    content = content.toLower();
    content.remove(QRegExp("(['])([a-z])"));
    content.replace("\n"," ");

    //transform text for comparability, removes everything that is not a letter nor number
    //but keeps the spaces (for splitting)
    content.remove(QRegExp("[^A-Za-z0-9 ]+"));

    QList<QString> contentList = this->content.toLower().split(" ");
    QMutableListIterator<QString> contentListIterator(contentList);

    //for every word in stop word list
    while(stopWordListIterator.hasNext())
    {
        //look at every word in content list
        while(contentListIterator.hasNext())
        {
            QString stopWord = stopWordListIterator.peekNext();
            QString contentWord = contentListIterator.peekNext();

            qDebug() << "\n\nstoppword: " << stopWord;
            qDebug() << "contentword: " << contentWord;

            //if match, remove
            if(stopWord == contentWord)
            {
                contentListIterator.next();
                contentListIterator.remove();
            }else{
                contentListIterator.next();
            }
        }
        qDebug() << "--------------------------------------------------------------------------------------------------------------";

        stopWordListIterator.next();
        contentListIterator.toFront();
    }
    QString eliminated = contentList.join(" ");
    this->content = eliminated;

    qDebug() << "eliminated and joined form list:\n\n" << eliminated << "\n\n";
}
