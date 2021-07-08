#ifndef PORTERSTEM_H
#define PORTERSTEM_H

#include "mainwindow.h"

class PorterStem
{

private:
    QString wordToStem;
    int endIndex;
    int stemIndex;
    char wordAsArray[];

public:
    PorterStem();
    QString loadWord(QString word = "");
private:
    bool isConsonant(int i);
    void wordToArray();
    int countM();
    void stepOne();
    void stepTwo();
    void stepThree();
    void stepFour();
    void stepFive();
    void stepSix();
    bool ending(QString end);
    void setEnd(QString end);
    bool vowelInStem();
    bool isCVC(int index);
    bool isDoubleConsonant(int index);

    void replaceEnd(QString end);
};

#endif // PORTERSTEM_H
