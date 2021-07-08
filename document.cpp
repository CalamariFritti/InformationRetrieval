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

int Document::getId()
{
    return this->id;
}

void Document::setId(int id)
{
    this->id = id;
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

void Document::setContentList()
{
    this->contentList = makeContentList(this->content);
}

void Document::setCleanContentList()
{
    QString cleaned = this->content;
    cleaned = this->removeApostrophes(cleaned);
    cleaned = this->removeNonLetters(cleaned);
    this->cleanContentList = makeContentList(cleaned);
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
    stopWords.setContent(stopWords.removeApostrophes(stopWords.content));
    stopWords.setContent(stopWords.removeNonLetters(stopWords.content));
    QStringList stopWordList = stopWords.makeContentList(stopWords.content);
    stopWordList.removeDuplicates();

    //keep a list of
    this->reducedForStopWords = removeApostrophes(this->content);
    this->reducedForStopWords = removeNonLetters(this->reducedForStopWords);
    this->eliminatedList = this->reducedForStopWords.split(QRegExp("[ ]+"));

    this->eliminatedList.removeAll("");
    stopWordList.removeAll("");

    QListIterator<QString> stopWordListIterator(stopWordList);
    QMutableListIterator<QString> contentListIterator(eliminatedList);

    //for every word in stop word list
    while(stopWordListIterator.hasNext())
    {
        //look at every word in content list
        while(contentListIterator.hasNext())
        {
            QString stopWord = stopWordListIterator.peekNext();
            QString contentWord = contentListIterator.peekNext();

           // qDebug() << "contentWord: " << contentWord;
           // qDebug() << "stopWord: " << stopWord;

            //if match, remove
            if(stopWord == contentWord)
            {
                contentListIterator.next();
                contentListIterator.remove();
            }else{
                contentListIterator.next();
            }
        }

        stopWordListIterator.next();
        contentListIterator.toFront();
    }
    QString eliminated = eliminatedList.join(" ");
    this->reducedForStopWords = eliminated;

}
//treatment for apostroph, remove apostroph plus following letter(s)
QString Document::removeApostrophes(QString input)
{
        input = input.toLower();
        input.remove(QRegExp("(['])([a-z]+)"));
        input.replace("\n"," ");
        return input;
}
//transform text for comparability, removes everything that is not a letter nor number
//but keeps the spaces (for splitting)
QString Document::removeNonLetters(QString input)
{
        input.remove(QRegExp("[^A-Za-z0-9 ]+"));
        input = input.trimmed();
        return input;
}

QStringList Document::makeContentList(QString input)
{
    return input.split(QRegExp("[ ]+"));
}

/*********************************************************************
 *
 * Stammform- Reduktion (Stemming) nach Porter-Algorithmus.
 *
*********************************************************************/

void Document::porterStemming(){
    PorterStem *Stemmer = new PorterStem();

    for(QString word : this->contentList){
         QString result = Stemmer->loadWord(word);
         qDebug() << "result : " << result;
         this->porterList.append(result);
     }

}
