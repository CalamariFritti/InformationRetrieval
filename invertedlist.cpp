#include "invertedlist.h"

InvertedList::InvertedList(QList<Document> collection)
{
        makeHugeList(collection, false);
        createInvertedList();
}

InvertedList::InvertedList(QList<Document> collection, bool isStopped)
{
        makeHugeList(collection, isStopped);
        createInvertedList();
}


void InvertedList::makeHugeList(QList<Document> collection, bool isStop){

    QListIterator<Document> documentIterator(collection);
    while(documentIterator.hasNext()){
        Document activeDoc = documentIterator.next();
        getAllTheWords(activeDoc, isStop);
    }
    basicMappedList = listSorter(basicMappedList);
}

QVector<QStringList> InvertedList::getAllTheWords(Document doc, bool isStop){

    if(isStop){
        for(int i = 0; i < doc.eliminatedList.size(); i++){
            QStringList temp = {doc.eliminatedList[i], QString::number(doc.id)};
            if(!basicMappedList.contains(temp)){
                basicMappedList.append(temp);
            }
        }
    }else{
        for(int i = 0; i < doc.cleanContentList.size(); i++){
            QStringList temp = {doc.cleanContentList[i], QString::number(doc.id)};
            if(!basicMappedList.contains(temp)){
                basicMappedList.append(temp);
            }
        }
    }
    return basicMappedList;
}

QVector<QStringList> InvertedList::listSorter(QVector<QStringList> inputList)
{
    std::sort(inputList.begin(),inputList.end());
    return inputList;
}

void InvertedList::createInvertedList()
{
    for(int i = 0; i < basicMappedList.size() - 2; ){

        QStringList temp;
        temp.append(basicMappedList[i].first());
        temp.append(basicMappedList[i].last());

        while(basicMappedList[i].first() == basicMappedList[i + 1].first()
              && i < basicMappedList.size() - 2 )
        {
            temp.append(basicMappedList[i + 1].last());
            i++;
        };

        invertedList.append(temp);
        i++;
    }
}
