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
    stopWords.removeApostrophes();
    QStringList stopWordList = stopWords.makeContentList();
    stopWordList.removeDuplicates();

    this->removeApostrophes();
    this->removeNonLetters();

    //keep a list of
    this->contentList = this->makeContentList();

    this->contentList.removeAll("");
    stopWordList.removeAll("");

    QListIterator<QString> stopWordListIterator(stopWordList);
    QMutableListIterator<QString> contentListIterator(contentList);

    //for every word in stop word list
    while(stopWordListIterator.hasNext())
    {
        //look at every word in content list
        while(contentListIterator.hasNext())
        {
            QString stopWord = stopWordListIterator.peekNext();
            QString contentWord = contentListIterator.peekNext();

            qDebug() << "\nstoppword: " << stopWord;
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

}
//treatment for apostroph, remove apostroph plus following letter(s)
void Document::removeApostrophes()
{
    this->reducedForStopWords = "";
    if(!this->title.isEmpty()){
        this->title = this->title.toLower();
        this->title.remove(QRegExp("(['])([a-z]+)"));
        this->title.replace("\n"," ");
    }
    this->content = this->content.toLower();
    this->content.remove(QRegExp("(['])([a-z]+)"));
    this->content.replace("\n"," ");
}
//transform text for comparability, removes everything that is not a letter nor number
//but keeps the spaces (for splitting)
void Document::removeNonLetters(){
    if(!this->title.isEmpty()){
        this->title.remove(QRegExp("[^A-Za-z0-9 ]+"));
        this->title = this->title.trimmed();
    }
    this->content.remove(QRegExp("[^A-Za-z0-9 ]+"));
    this->content = this->content.trimmed();
}

QStringList Document::makeContentList()
{
    return this->content.split(QRegExp("[ ]+"));
}


