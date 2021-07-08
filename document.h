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
    QString porterContent;

    QStringList contentList;
    QStringList cleanContentList;
    QStringList eliminatedList;
    QStringList porterList;

    qint32 id;
public slots:
    void setContent(QString content);
    void setTitle(QString title);
    void setId(int id);
    void setContentList();
    void setCleanContentList();
    void stopWordElimination(Document stopWordList);
    QString removeNonLetters(QString);
    QString removeApostrophes(QString);
    QStringList makeContentList(QString);
    QString getContent();
    QString getTitle();
    int getId();
    void porterStemming();

};

#endif // DOCUMENT_H
