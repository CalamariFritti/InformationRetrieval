#ifndef INVERTEDLIST_H
#define INVERTEDLIST_H

#include "mainwindow.h"

class InvertedList
{
public:
    InvertedList(QList<Document>);
    InvertedList(QList<Document>, bool);
    QStringList allTheWords;
    QVector<QStringList> basicMappedList;
    QVector<QStringList> invertedList;

    QVector<QStringList> getAllTheWords(Document doc, bool);
    QVector<QStringList> createInvertedList(QVector<QStringList>);
    QVector<QStringList> listSorter(QVector<QStringList> inputList);
    void createInvertedList();
private:
    void createList();
    void makeHugeList(QList<Document> collection, bool);
};

#endif // INVERTEDLIST_H
