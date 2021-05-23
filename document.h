#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include <QWidget>

class Document
{
public:
    Document();

    QString content;
    QString reducedForStopWords;
    QString title;

    QStringList contentList;

public slots:
    void setContent(QString content);
    void setTitle(QString title);
    void stopWordElimination(Document stopWordList);
    void removeNonLetters();
    void removeApostrophes();
    QStringList makeContentList();
    QString getContent();
    QString getTitle();

};

#endif // DOCUMENT_H
