#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSet>
#include "document.h"
#include "calculator.h"
#include "invertedlist.h"
#include "porterstem.h"

#include <QDebug>


namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

private slots:
    void on_loadFileButton_clicked();
    void loadFile(QString &fileName);
    void on_loadStoppWordList_clicked();
    void on_createOriginalDocs_clicked();
    void on_eliminateStopWords_clicked();
    void saveStopWordDocs();

    void on_getTerm_clicked();
    void on_getMode_clicked();

    void linearSearch(QList<Document> searchList);
    void on_linearSearchOrigDocs_clicked();
    void on_linearSearchStopDocs_clicked();

    void on_recall_clicked();
    void on_precision_clicked();

    void on_invertListStoppDocs_clicked();
    void on_invertListOrigDocs_clicked();

    void on_linearSearchInvertList_clicked();
    void linearSearchInvertList(QVector<QStringList>);
    void regularLinearSearchInvertList(QVector<QStringList>);
    void disjunctLinearSearchInvertList(QVector<QStringList>);
    void conjunctLinearSearchInvertList(QVector<QStringList>);
    void stringListToResultId(QStringList input);
    void on_linearSearchInvertListStop_clicked();

    void on_porterStem_clicked();

    void on_linearSearchStemmDocs_clicked();

private:
    Ui::MainWindow *ui;

    Document input;
    Document stoppWords;

    bool isStopWord = false;
    bool stopDocSearch = false;

    QList<Document> docList;
    QList<Document> stoppWordsEliminatedList;
    QList<Document> stemList;
    QList<Document> resultsLinear;

    QList<int> resultIds;

    QVector<QStringList> origInvertList;
    QVector<QStringList> stopInvertList;
    QVector<QStringList> stemmedInvertList;


    QString term = "";
    QString term1 = "";
    QString term2 = "";
    QString mode = "Regulär";

    int id = 01;

    QString getTerm();
    void regularLinearSearch(QList<Document> searchList);
    void conjunctionLinearSearch(QList<Document> searchList);
    void disjunctionLinearSearch(QList<Document> searchList);
};

#endif // MAINWINDOW_H
