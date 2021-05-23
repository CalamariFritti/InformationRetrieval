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
            ui->textBrowser->clear();
            ui->textBrowser->append("Es wurde eine Datei geladen:\n\n"
                                    + fileName +"\n\n");
            ui->textBrowser->append("Sie können diese datei nun zur Kollektion der "
                                    "Originaldokumente wandeln.");
            ui->textBrowser->append("(\"Erzeuge Originaldokumente\"-Button)");
        }else{
            stoppWords.setContent(read);
            ui->textBrowser->clear();
            ui->textBrowser->append("Es wurde eine Datei geladen:\n\n"
                                    + fileName +"\n\n");
            ui->textBrowser->append("Sie können jetzt die Stoppworteliminierung "
                                    " für die Original-Dokumente durchführen.");
        }
    }
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
    ui->textBrowser->clear();
    ui->textBrowser->append("Es wurden\n    "
                            + QString::number(docList.length())
                            + "\nOriginaldokumente erstellt.\n");
    ui->textBrowser->append("Die Originaldokumente wurden hier abgelegt:\n\n"
                            + path
                            + "\n    abgelegt.\n\n");
    ui->textBrowser->append("Sie können jetzt bereits eine lineare Suche in "
                            "den Original-Dokumenten vornehmen.\n"
                            "Hierzu müssen Sie zunächst einen Suchbegriff "
                            "eingeben."
                            "\n\nAlternativ können die Original-Dokumente "
                            "jetzt um Stopp-Worte bereinigt werden! \n"
                            "Hierfür bitte zunächst eine Stoppwortliste laden.");
}

void MainWindow::on_loadStoppWordList_clicked()
{
    isStopWord = true;
    on_loadFileButton_clicked();
    isStopWord = false;
}

void MainWindow::on_eliminateStopWords_clicked()
{
    ui->textBrowser->clear();
    ui->textBrowser->append("Eliminieren Stoppworte aus den Originaldokumenten...");

    QListIterator<Document> docIter(docList);
    while(docIter.hasNext())
    {
        Document doc;
        doc = docIter.next();
        doc.stopWordElimination(stoppWords);
        stoppWordsEliminatedList.append(doc);
        ui->textBrowser->append("...");
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
        qDebug() << "this filename too long? " << fileName;
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
        qDebug() << "\ncontent?" << doc.content;
        file.write(out.toUtf8());
        file.close();
    }
    ui->textBrowser->clear();
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
        term = term.toLower();
    }
}

void MainWindow::on_linearSearchOrigDocs_clicked()
{
    ui->textBrowser->clear();
    ui->textBrowser->append("Lineare Suche in den Original Dokumenten gestartet:\n\n");
    ui->textBrowser->append("Folgende Dokumente enthalten den gesuchten Begriff:");

    linearSearch(docList);
}

void MainWindow::on_linearSearchStopDocs_clicked()
{
    ui->textBrowser->clear();
    ui->textBrowser->append("Lineare Suche in den Stoppwort-Dokumenten gestartet:\n\n");
    ui->textBrowser->append("Folgende Dokumente enthalten den gesuchten Begriff:");
    linearSearch(stoppWordsEliminatedList);
}

void MainWindow::linearSearch(QList<Document> searchList){

    QListIterator<Document> searchListIter(searchList);
    resultsLinear.clear();

    while(searchListIter.hasNext())
    {
        Document activeDoc = searchListIter.next();
        activeDoc.removeApostrophes();
        activeDoc.removeNonLetters();
        activeDoc.contentList = activeDoc.makeContentList();
        qDebug() << "searched this content: " << activeDoc.contentList.join(",");
        qDebug() << "for this word: " << term;
        qDebug() << "result: " << activeDoc.contentList.contains(term);
        if(activeDoc.contentList.contains(term)){
            resultsLinear.append(activeDoc);
            qDebug() << "found one: " << resultsLinear.last().getTitle();
            ui->textBrowser->append(resultsLinear.last().getTitle());
        }
    }
}
