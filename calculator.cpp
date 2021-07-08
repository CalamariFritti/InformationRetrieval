#include "calculator.h"


Calculator::Calculator(QString term)
{
    this->term = term;          //fox, man, seeing, etc.
    active = activeTerm(term);  //loads predefined relevant list
}
Calculator::Calculator(QString term01, QString term02, QString mode)
{
    multi01 = activeTerm(term01);
    multi02 = activeTerm(term02);
    if(mode == "Konjunktion"){ active = conjunction(multi01, multi02); }
    else{ active = disjunction(multi01, multi02); }
}

QList<int> Calculator::activeTerm(QString term)
{
    if(term == "beast"){
        return relBeasts;
    }else if(term == "fox"){
        return relFox;
    }else if(term == "animal"){
        return relAnimal;
    }else if(term == "hunters"){
        return relHunters;
    }else if(term == "man"){
        return relMan;
    }else if(term == "seeing"){
        return relSeeing;
    }else{
        QList<int> defVal = {0};    // other terms always produce 0
        return defVal;              // for recall and precision
    }
}
/***
 * returns elements that are in list one AND list two
 ***/
QList<int> Calculator::conjunction(QList<int> one, QList<int> two)
{
    QList<int> ret;
    qDebug() << "konjunktion";
    for(int i = 0; i < one.length(); i++){
        if(two.contains(one[i])){
            ret.append(one[i]);
        }
    }
    qDebug() << "ret length" << ret.length();
    return ret;
}
/***
 * returns all elements that are in list one OR list two
 ***/
QList<int> Calculator::disjunction(QList<int> one, QList<int> two)
{
    for(int i = 0; i < one.length(); i++){
        if(!two.contains(one[i])){
            two.append(one[i]);
        }
    }
    return two;
}

double Calculator::recall(QList<int> results)
{
    double ret = 0;
    double intersection = 0;            //intersects relevants results

    QListIterator<int> resIter(results);
    while(resIter.hasNext()){
        if(active.contains(resIter.next()))
        {
            intersection++;
        }
    }
    ret = (intersection/double(active.length()));
    return ret;

}

double Calculator::precision(QList<int> results)
{
    double ret = 0;
    double intersection = 0;            //intersects relevants results

    QListIterator<int> resIter(results);
    while(resIter.hasNext()){
        if(active.contains(resIter.next())){ intersection++; }
    }

    ret = (intersection/double(results.length()));
    return ret;
}
