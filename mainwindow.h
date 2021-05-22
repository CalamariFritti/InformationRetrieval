#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include "document.h"


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

    void on_linearSearchOrigDocs_clicked();

private:
    Ui::MainWindow *ui;

    Document input;

    bool isStopWord = false;

    Document stoppWords;

    QList<Document> docList;

    QList<Document> stoppWordsEliminatedList;

    QList<Document> resultsLinear;

    QString term = "";

};

#endif // MAINWINDOW_H
