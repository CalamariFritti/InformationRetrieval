#include <QtWidgets>

#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "document.h"

#include <QDebug>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

/*
 * lets you open files in .txt format only
 */
void MainWindow::on_loadFileButton_clicked()
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open File"),
                                                    "",
                                                    "text file (*.txt)");
    if (!fileName.isEmpty())
        loadFile(fileName);
}

/*
 * if file is open at provided fileName, a new document object is created
 * contents of the document is written from file and is shown on textbrowser
 */
void MainWindow::loadFile(QString &fileName)
{
    QFile file(fileName);
    if (!file.open(QFile::ReadOnly | QFile::Text)) {
        QMessageBox::warning(this,
                             tr("InfRet"),
                             tr("Cannot read file %1:\n%2.")
                             .arg(QDir::toNativeSeparators(fileName), file.errorString()));
        return;
    }


    QTextStream in(&file);

    while(!in.atEnd())
    {
        QString read = in.readAll();
        if(!isStopWord){
            input.setContent(read);
        }else{
            stoppWords.setContent(read);
        }
    }
    ui->textBrowser->clear();
    ui->textBrowser->append("Datei geladen:\n\n" + fileName +"\n\n");
}

void MainWindow::on_createOriginalDocs_clicked()
{
    //split input text on 4 empty lines
    QList<QString> originalDocuments = input.content.split("\n\n\n\n");
    //remove introduction pragraphs
    originalDocuments.removeFirst();
    originalDocuments.removeFirst();
    //iterator object necessary for stepping through elements of list
    QListIterator<QString> origDocIterator(originalDocuments);
    //get working directory
    QString path = QDir::currentPath();
    path += "/originalDocuments";
    //create relative save path
    QDir dir(path);
    if(!dir.exists())
    {
        dir.mkdir(path);
    }
    //create original doc from each parable text and save to file
    while(origDocIterator.hasNext())
    {
        Document doc;
        QList<QString> titleText = origDocIterator.peekNext().split("\n\n\n");

        doc.setTitle(titleText[0]);
        doc.setContent(titleText[1]);

        QString fileName = titleText[0].toLower().trimmed().replace(" ","_");
        QString save = path + "/" + fileName + ".txt";

        QFile file(save);

        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::information(this,
                                     tr("Unable to open file"),
                                     file.errorString());
            return;
        }
        QString out = doc.title + "\n\n" + doc.content;
        file.write(out.toUtf8());
        file.close();
        origDocIterator.next();

        docList.append(doc);
    }
    ui->textBrowser->append("Es wurden\n    "
                            + QString::number(docList.length())
                            + "\nOriginaldokumente erstellt.\n");
    ui->textBrowser->append("Die Originaldokumente wurden hier abgelegt:\n\n"
                            + path
                            + "\n    abgelegt.");
}

void MainWindow::on_loadStoppWordList_clicked()
{
    isStopWord = true;
    on_loadFileButton_clicked();
    isStopWord = false;
}

void MainWindow::on_eliminateStopWords_clicked()
{
    QListIterator<Document> docIter(docList);
    while(docIter.hasNext()){
        Document doc;
        doc = docIter.next();

        doc.stopWordElimination(stoppWords);

        stoppWordsEliminatedList.append(doc);

    }
    ui->textBrowser->append("\nStoppworteliminierung durchgeführt für Dokumente:\n"
                            + QString::number(stoppWordsEliminatedList.length()));
    saveStopWordDocs();
}

void MainWindow::saveStopWordDocs()
{
    QListIterator<Document> stoppWordIterator(stoppWordsEliminatedList);

    QString path = QDir::currentPath();
    path += "/stoppWordDocuments";
    //create relative save path
    QDir dir(path);
    if(!dir.exists())
    {
        dir.mkdir(path);
    }
    while(stoppWordIterator.hasNext())
    {
        Document doc = stoppWordIterator.next();
        QString fileName = doc.title.toLower().trimmed().replace(" ","_");
        QString save = path + "/" + fileName + ".txt";
        QFile file(save);

        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::information(this,
                                     tr("Unable to open file"),
                                     file.errorString());
            return;
        }
        QString out = doc.title + "\n\n" + doc.content;
        file.write(out.toUtf8());
        file.close();
    }

    ui->textBrowser->append(
            "\nDokumente mit eliminierten Stopp-Wörtern gespeichert unter:\n"
            + QString::number(stoppWordsEliminatedList.length())
            + "\n"
            + path);
}

void MainWindow::on_getTerm_clicked()
{

    term = QInputDialog::getText(this,"Bitte Suchbegriff eingeben:","Lineare Suche");
    if(term == ""){
        QMessageBox::warning(this,"Kein Begriff eingegeben!","Die Eingabe darf nicht leer sein!",QMessageBox::Ok);
    }else{
        ui->textBrowser->append("Suchbegriff: \"" + term + "\" eingegeben!");
    }
}

void MainWindow::on_linearSerachOrigDocs_clicked()
{
   QListIterator<Document> docIter(docList);

   while(docIter.hasNext())
   {

   }

}
