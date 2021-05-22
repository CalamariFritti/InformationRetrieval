#ifndef DOCUMENT_H
#define DOCUMENT_H

#include <QObject>
#include <QWidget>

class Document
{
public:
    Document();

    QString content;
    QString title;

public slots:
    void setContent(QString content);
    void setTitle(QString title);

    QString getContent();
    QString getTitle();

    void stopWordElimination(Document stopWordList);


};

#endif // DOCUMENT_H
