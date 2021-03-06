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
        doc.setId(id);
        doc.setContentList();
        doc.setCleanContentList();
        id++;

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
    ui->textBrowser->append("Eliminiere Stoppworte aus den Originaldokumenten...");

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
        QString save = path + "/" + fileName + ".txt";
        QFile file(save);

        if (!file.open(QIODevice::WriteOnly))
        {
            QMessageBox::information(this,
                                     tr("Unable to open file"),
                                     file.errorString());
            return;
        }
        QString out = doc.title + "\n\n" + doc.reducedForStopWords;
        file.write(out.toUtf8());
        file.close();
    }
    ui->textBrowser->clear();
    ui->textBrowser->append(
                QString::number(stoppWordsEliminatedList.length())
            + " Dokumente mit eliminierten Stopp-Wörtern gespeichert unter:\n"
            + "\n"
            + path);
}

void MainWindow::on_getMode_clicked()
{
    QStringList items = { "Regulär", "Konjunktion", "Disjunktion"};
    mode = QInputDialog::getItem(this, "Modus wählen", "Modus wählen", items, 0);
    ui->modeBrowser->setText(mode);
}

void MainWindow::on_getTerm_clicked()
{
    term = getTerm();
    ui->termBrowser->setText(term);
    if(term.split(" ").length() > 1){
        term1 = term.split(" ")[0];
        term2 = term.split(" ")[1];
        ui->termBrowser->setText("Term 1: " + term1);
        ui->termBrowser->append("Term 2: " + term2);
    }else{
        term1 = term;
    }
}

QString MainWindow::getTerm()
{
    QString ret = "";
    ret = QInputDialog::getText(this,"Bitte Suchbegriff(e) eingeben:","Lineare Suche");
    if(ret == ""){
        QMessageBox::warning(this,"Kein Begriff eingegeben!","Die Eingabe darf nicht leer sein!",QMessageBox::Ok);
    }else if(ret.split(" ").length() > 2){
        QMessageBox::warning(this,"Maximum überschritten!","Bitte maximal zwei Suchbegriffe eingeben!",QMessageBox::Ok);
    }else{
        ui->termBrowser->setText(ret);
        ret = ret.toLower().trimmed();
    }

    return ret;
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
    stopDocSearch = true;
    linearSearch(stoppWordsEliminatedList);
    stopDocSearch = false;
}

void MainWindow::linearSearch(QList<Document> searchList)
{
    if(mode == "Disjunktion"){
        disjunctionLinearSearch(searchList);
    }else if(mode == "Konjunktion"){
        conjunctionLinearSearch(searchList);
    }else{
        regularLinearSearch(searchList);
    }
}

void MainWindow::regularLinearSearch(QList<Document> searchList)
{
    QListIterator<Document> searchListIter(searchList);
    resultsLinear.clear();
    resultIds.clear();

    while(searchListIter.hasNext())
    {
        Document activeDoc = searchListIter.next();
        QString prepare = "";
        if(!stopDocSearch)
        {
            prepare = activeDoc.removeApostrophes(activeDoc.content);
            prepare = activeDoc.removeNonLetters(prepare);
        }else{
            prepare = activeDoc.reducedForStopWords;
        }

        QStringList cleanList = activeDoc.makeContentList(prepare);

        if(cleanList.contains(term1)){
            resultsLinear.append(activeDoc);
            resultIds.append(activeDoc.getId());
            ui->textBrowser->append(resultsLinear.last().getTitle() +
                                    "id: " + QString::number(activeDoc.getId()));
        }
    }
}

void MainWindow::conjunctionLinearSearch(QList<Document> searchList)
{
    QListIterator<Document> searchListIter(searchList);
    resultsLinear.clear();
    resultIds.clear();

    while(searchListIter.hasNext())
    {
        Document activeDoc = searchListIter.next();
        QString prepare = "";
        if(!stopDocSearch)
        {
            prepare = activeDoc.removeApostrophes(activeDoc.content);
            prepare = activeDoc.removeNonLetters(prepare);
        }else{
            prepare = activeDoc.reducedForStopWords;
        }

        QStringList cleanList = activeDoc.makeContentList(prepare);

        if(cleanList.contains(term1) && cleanList.contains(term2)){
            resultsLinear.append(activeDoc);
            resultIds.append(activeDoc.getId());
            ui->textBrowser->append(resultsLinear.last().getTitle() +
                                    "id: " + QString::number(activeDoc.getId()));
        }
    }
}

void MainWindow::disjunctionLinearSearch(QList<Document> searchList)
{
    QListIterator<Document> searchListIter(searchList);
    resultsLinear.clear();
    resultIds.clear();

    while(searchListIter.hasNext())
    {
        Document activeDoc = searchListIter.next();
        QString prepare = "";
        if(!stopDocSearch)
        {
            prepare = activeDoc.removeApostrophes(activeDoc.content);
            prepare = activeDoc.removeNonLetters(prepare);
        }else{
            prepare = activeDoc.reducedForStopWords;
        }

        QStringList cleanList = activeDoc.makeContentList(prepare);

        if(cleanList.contains(term1) || cleanList.contains(term2)){
            resultsLinear.append(activeDoc);
            resultIds.append(activeDoc.getId());
            ui->textBrowser->append(resultsLinear.last().getTitle() +
                                    "id: " + QString::number(activeDoc.getId()));
        }
    }
}
/*******************+
 * This creates an calcutor object and automatically shows the resuts for
 * precision and recall for the last active search!
 * Only terms that are provided in calculator class can be calculated for,
 * otherwise the results will always be 0.
 * *********************/

void MainWindow::on_recall_clicked()
{
    Calculator * calc;
    QString recallTerm = getTerm();                        //ask for term

    //split term to multiple terms
    QStringList multipleTerms;
    if(recallTerm.contains(" ")){ multipleTerms = recallTerm.split(' '); }

    //
    if(multipleTerms.length() > 1){
        calc = new Calculator(multipleTerms[0], multipleTerms[1], mode);
    }else{
        calc =new Calculator(recallTerm);
    }

    double precision = 0;
    double recall = 0;

    precision = calc->precision(resultIds);
    recall = calc->recall(resultIds);

    ui->textBrowser->clear();
    ui->textBrowser->append("Es wurde eine Suche mit folgenden Parametern durchgeführt:");
    ui->textBrowser->append("Suchbegriff: " + term);

    ui->textBrowser->append("Anzahl der gefundenen Dokumente:\n " + QString::number(resultsLinear.length()));

    if(multipleTerms.length()> 1){
        ui->textBrowser->append("Zu den Recall/Precision-Begriffen: \n"
                                + multipleTerms[0] + multipleTerms[1] +
                                "\nwurde mittels \n" + mode +
                                "\nfolgende Werte ermittelt:");
    }else{
        ui->textBrowser->append("Zum Recall/Precision-Begriff: \n"
                            + recallTerm +
                            "\nwurden folgende Werte ermittelt:");
    }
    ui->textBrowser->append("Recall-Wert: " + QString::number(recall));
    ui->textBrowser->append("Precision-Wert: " + QString::number(precision));
}

void MainWindow::on_invertListOrigDocs_clicked()
{
    InvertedList * invList = new InvertedList(docList);
    origInvertList = invList->invertedList;

    ui->textBrowser->setText("Invertierte Liste aus den Original-Dokumenten erstellt:");
    ui->textBrowser->append("\nDie Liste enthält: " +
                            QString::number(origInvertList.size()) +
                            " Begriffe!\n");
    ui->textBrowser->append("Das erste Element der Liste ist:\n\n" +
                            origInvertList[0].join(" "));
}

void MainWindow::on_invertListStoppDocs_clicked()
{

    InvertedList * invList = new InvertedList(stoppWordsEliminatedList, true);
    stopInvertList = invList->invertedList;

    ui->textBrowser->setText("Invertierte Liste aus den StopWort-Dokumenten erstellt:");
    ui->textBrowser->append("\nDie Liste enthält: " +
                            QString::number(stopInvertList.size()) +
                            " Begriffe!\n");
    ui->textBrowser->append("Das erste Element der Liste ist:\n\n" +
                            stopInvertList[0].join(" "));
}


void MainWindow::on_precision_clicked()
{



}

void MainWindow::on_linearSearchInvertList_clicked()
{
    ui->textBrowser->clear();
    ui->textBrowser->append("Suche in invertierter Liste der Original Dokumenten gestartet:\n\n");
    ui->textBrowser->append("Folgende Dokumente enthalten den gesuchten Begriff:");

    linearSearchInvertList(origInvertList);
}


void MainWindow::on_linearSearchInvertListStop_clicked()
{
    ui->textBrowser->clear();
    ui->textBrowser->append("Suche in invertierter Liste der StoppWort Dokumente gestartet:\n\n");
    ui->textBrowser->append("Folgende Dokumente enthalten den gesuchten Begriff:");

    linearSearchInvertList(stopInvertList);
}

void MainWindow::linearSearchInvertList(QVector<QStringList> input)
{
    if(mode == "Disjunktion"){
        disjunctLinearSearchInvertList(input);
    }else if(mode == "Konjunktion"){
        conjunctLinearSearchInvertList(input);
    }else{
        regularLinearSearchInvertList(input);
    }

}

void MainWindow::regularLinearSearchInvertList(QVector<QStringList> input)
{
    QStringList result;
    for(int i = 0; i < input.size(); i++){
        if(term1 == input[i].first()){
            result = input[i];
            result.removeFirst();
            break;
        }
    }

    stringListToResultId(result);

    ui->textBrowser->append(result.join(" "));
    qDebug() << "gesucht und gefunden in invertierter Liste: " << result;

}

void MainWindow::disjunctLinearSearchInvertList(QVector<QStringList> input)
{
    QStringList result;
    QStringList result1;
    QStringList result2;

    for(int i = 0; i < input.size() - 1; i++){
        if(term1 == input[i].first()){
            result1 = input[i];
            result1.removeFirst();
            break;
        }
    }

    for(int i = 0; i < input.size() - 1; i++){
        if(term2 == input[i].first()){
            result2 = input[i];
            result2.removeFirst();
            break;
        }
    }
    for(QString x : result1){
        result.append(x);
        resultIds.append(x.toInt());
    }

    for(QString x : result2){
        if(!result.contains(x)){
            result.append(x);
            resultIds.append(x.toInt());
        }
    }

    ui->textBrowser->append(result.join(" "));
}

void MainWindow::conjunctLinearSearchInvertList(QVector<QStringList> input)
{
    QStringList result;
    QStringList result1;
    QStringList result2;

    for(int i = 0; i < input.size() - 1; i++){
        if(term1 == input[i].first()){
            result1 = input[i];
            result1.removeFirst();
            break;
        }
    }

    for(int j = 0; j < input.size() - 1; j++){
        if(term2 == input[j].first()){
            result2 = input[j];
            result2.removeFirst();
            break;
        }
    }

    for(QString x : result1){
        if(result2.contains(x)){
            result.append(x);
            resultIds.append(x.toInt());
        }
    }


    qDebug() << result1;
    qDebug() << result2;
    ui->textBrowser->append(result.join(" "));

}

void MainWindow::stringListToResultId(QStringList input)
{
    resultIds.clear();
    for(int i = 0; i < input.size() - 1; i++)
    {
        resultIds.append(input[i].toInt());
    }
}

void MainWindow::on_porterStem_clicked()
{
    ui->textBrowser->clear();
    ui->textBrowser->setText("Stemming nach Porter durchgeführt: ");

    for(Document doc : this->docList){
        qDebug() << "eli,List" << doc.contentList;
        doc.porterStemming();
    }

/*
    QStringList list = {"relational", "appetizer", "realization",
                       "procurement", "defendant", "racism","objective"};
*/
    QStringList list = {"caresses", "ponies", "ties", "cats", "ponies", "ties",
                       "caress", "feed", "agreed", "disabled", "matting",
                        "mating", "meeting", "milling", "messing", "meetings",
                       "directly", "relational", "appetizer", "realization",
                       "procurement", "defendant", "racism","objective",
                       "cave", "love", "snow","box"};


    PorterStem *Stemmer = new PorterStem();

     ui->textBrowser->setText("porterstemmer: ");
    for(QString word : list){
         QString result = Stemmer->loadWord(word);
         ui->textBrowser->append(result);

     }
/*

    ui->textBrowser->setText("Stemming nach Porter durchgeführt für:");
    ui->textBrowser->append("\nDie Liste enthält: " +
                            QString::number(stopInvertList.size()) +
                            " Begriffe!\n");
    ui->textBrowser->append("Das erste Element der Liste ist:\n\n" +
                            stopInvertList[0].join(" "));
*/
}

void MainWindow::on_linearSearchStemmDocs_clicked()
{
    ui->textBrowser->clear();

    QListIterator<Document> searchListIter(docList);
    resultsLinear.clear();
    resultIds.clear();

    while(searchListIter.hasNext())
    {
        Document activeDoc = searchListIter.next();
        qDebug() << activeDoc.title;
        QString prepare = activeDoc.porterList.join(" ");
        qDebug() << term1;
        qDebug() << prepare;

        if(prepare.contains(term1)){
            qDebug() << true;
            resultsLinear.append(activeDoc);
            resultIds.append(activeDoc.getId());
            ui->textBrowser->append(resultsLinear.last().getTitle() +
                                    "id: " + QString::number(activeDoc.getId()));
        }
    }
}
