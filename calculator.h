#ifndef CALCULATOR_H
#define CALCULATOR_H

#include "mainwindow.h"

class Calculator
{
public:
    Calculator(QString);
    Calculator(QString, QString, QString);
    double precision(QList<int> results);
    double recall(QList<int> results);
private:
    QList<int> relBeasts = { 4, 10, 11, 12, 15, 18, 24, 34, 43, 52, 59, 73 };
    QList<int> relFox = { 4, 8, 19, 20, 31, 34, 38, 49, 59, 64, 65, 73, 74, 82 };
    QList<int> relAnimal ={ 4, 5, 8, 9, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20, 21,
                      31, 34, 38, 43, 46, 47, 48, 49, 50, 52, 53, 55, 57, 58,
                      59, 62, 64, 65, 66, 67, 68, 70, 73, 74, 77, 78, 80, 82 };
    QList<int> relHunters = { 11, 18, 25, 30, 32, 49, 53, 66, 74 };
    QList<int> relMan = { 6, 11, 12, 17, 18, 27, 35, 41, 42, 43, 44, 45, 53, 54, 56,
                    57, 58, 60, 61, 62, 69, 72, 76, 77, 79, 80, 82 };
    QList<int> relSeeing = { 10, 11, 13, 17, 21, 34, 41, 44, 45, 50, 54, 59, 62, 64,
                        66, 72, 73, 77, 80 };
    QList<int> active;
    QList<int> multi01;
    QList<int> multi02;
    QString term;
    QList<int> activeTerm(QString term);
    QList<int> conjunction(QList<int> one, QList<int> two);
    QList<int> disjunction(QList<int> one, QList<int> two);
};

#endif // CALCULATOR_H
