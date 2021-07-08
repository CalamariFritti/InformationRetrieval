/***
 * Implements a Porter (1980) suffix stripping algorithm.
 *
 * Takes on word as an argument, transforms this to char array.
 * removes suffixes following the rules of Porter stemming.
 *
 * Returns the
 *
 ***/


#include "porterstem.h"

PorterStem::PorterStem()
{

}

// using the six steps of porter stemming algorithm
// returns stemmed word
QString PorterStem::loadWord(QString word)
{
    wordToStem = "";

    // empty or short words do not get stemmed
    if(word.isEmpty() || word.length() <= 2){ return word;}

    // length of word, array from word
    stemIndex = 0;
    endIndex = word.length() - 1;
    wordToStem = word;
    wordToArray();

    // six steps of porter stemming
    stepOne();
    stepTwo();
    stepThree();
    stepFour();
    stepFive();
    stepSix();


    QString ret = QString::fromLatin1(wordAsArray, (endIndex + 1));


    return ret;
}

void PorterStem::wordToArray()
{
    for(int i = 0; i <= endIndex; i++){ wordAsArray[i] = wordToStem[i].toLatin1(); }
}

// endings -ed or -ing and plurals are elminated
/************************************************
 * examples:
 * 			caresses  ->  caress
 *          ponies    ->  poni
 *			ties      ->  ti
 *			caress    ->  caress
 *			cats      ->  cat
 *          agreed    -> agree
 *          disabled  ->  disable
 *          feed      ->  feed
 *
 *          matting   ->  mat
 *          mating    ->  mate
 *			meeting   ->  meet
 *			milling   ->  mill
 *			messing   ->  mess
 *
 *          meetings  ->  meet
 * **********************************************/
void PorterStem::stepOne()
{
    if(wordAsArray[endIndex] == 's'){
        if(ending("sses")){
            endIndex -= 2;
        }else if(ending("ies")){
            setEnd("i");
        }else if(wordAsArray[endIndex - 1] != 's'){
            endIndex--;
        }
    }
    if(ending("eed")){
        if(countM() > 0) { endIndex--; }
    }else if(ending("ed") || (ending("ing") && vowelInStem())){
        endIndex = stemIndex;
        if (ending("at")){
            setEnd("ate");
        }else if (ending("bl")){
            setEnd("ble");
        }else if (ending("iz")){
            setEnd("ize");
        }else if (isDoubleConsonant(endIndex)){
            endIndex--;
            int ch = wordAsArray[endIndex];
            if (ch == 'l' || ch == 's' || ch == 'z')
                endIndex++;
        }else if (countM() == 1 && isCVC(endIndex)){
            setEnd("e");
        }
    }
}

// if stem has another vowel, transform y to i on last position
void PorterStem::stepTwo(){
    if(ending("y") && vowelInStem()){
        wordAsArray[endIndex] = 'i';
    }
}

//
void PorterStem::stepThree(){
    if(endIndex == 0){ return; }

    switch(wordAsArray[endIndex - 1]) {
    case 'a':
        if(ending("ational")){ replaceEnd("ate"); break;}
        if(ending("tional")){ replaceEnd("tion"); }
        break;
    case 'c':
        if(ending("enci")){ replaceEnd("ence"); break;}
        if(ending("anci")){ replaceEnd("ance"); }
        break;
    case 'e':
        if(ending("izer")){ replaceEnd("ize"); }
        break;
    case 'l':
        if(ending("bli")){ replaceEnd("ble"); break; }
        if(ending("alli")){ replaceEnd("al"); break; }
        if(ending("entli")) { replaceEnd("ent"); break; }
        if(ending("eli")) { replaceEnd("e"); break; }
        if(ending("ousli")) { replaceEnd("ous");  }
        break;
    case 'o':
        if(ending("ization")) { replaceEnd("ize"); break; }
        if(ending("ation")) { replaceEnd("ate"); break; }
        if(ending("ator")) { replaceEnd("ate");  }
        break;
    case 's':
        if(ending("alism")) { replaceEnd("al"); break; }
        if(ending("iveness")) { replaceEnd("ive"); break; }
        if(ending("fulness")) { replaceEnd("ful"); break; }
        if(ending("ousness")) { replaceEnd("ous");  }
        break;
    case 't':
        if(ending("aliti")) { replaceEnd("al"); break; }
        if(ending("iviti")) { replaceEnd("ive"); break; }
        if(ending("biliti")) { replaceEnd("ble");  }
        break;
    case 'g':
        if(ending("logi")) { replaceEnd("log");
        }
        break;
    }
}

void PorterStem::stepFour(){
    switch (wordAsArray[endIndex]) {
    case 'e':
        if (ending("icate")) { replaceEnd("ic"); break; }
        if (ending("ative")) { replaceEnd(""); break; }
        if (ending("alize")) { replaceEnd("al"); }
        break;
    case 'i':
        if (ending("iciti")) { replaceEnd("ic"); }
        break;
    case 'l':
        if (ending("ical")) { replaceEnd("ic"); break; }
        if (ending("ful")) { replaceEnd(""); }
        break;
    case 's':
        if (ending("ness")) { replaceEnd(""); }
        break;
                }
}

void PorterStem::stepFive(){
    if (endIndex == 0) return;

    switch (wordAsArray[endIndex - 1]) {
    case 'a':
        if (ending("al")){ break; return; }
    case 'c':
        if (ending("ance")) break;
        if (ending("ence")) break; return;
    case 'e':
        if (ending("er")) break; return;
    case 'i':
        if (ending("ic")) break; return;
    case 'l':
        if (ending("able")) break;
        if (ending("ible")) break; return;
    case 'n':
        if (ending("ant")) break;
        if (ending("ement")) break;
        if (ending("ment")) break;
        /* element etc. not stripped before the m */
        if (ending("ent")) break; return;
    case 'o':
        if (ending("ion") && stemIndex >= 0 && (wordAsArray[stemIndex] == 's' || wordAsArray[stemIndex] == 't')) break;
        /* j >= 0 fixes Bug 2 */
        if (ending("ou")) break; return;
        /* takes care of -ous */
    case 's':
        if (ending("ism")) break; return;
    case 't':
        if (ending("ate")) break;
        if (ending("iti")) break; return;
    case 'u':
        if (ending("ous")) break; return;
    case 'v':
        if (ending("ive")) break; return;
    case 'z':
        if (ending("ize")) break; return;
    default:
        return;
    }
    if (countM() > 1) {
        endIndex = stemIndex;
    }
}

void PorterStem::stepSix(){
    stemIndex = endIndex;

    if (wordAsArray[endIndex] == 'e') {
        int a = countM();
        if (a > 1 || a == 1 && !isCVC(endIndex - 1))
            endIndex--;
    }
    if (wordAsArray[endIndex] == 'l' && isDoubleConsonant(endIndex) && countM() > 1)
        endIndex--;
}

// check if ending of the current word matches the end-string
bool PorterStem::ending(QString end)
{
    int len = end.length();
    int index = endIndex - len + 1;
    if(index < 0){ return false; }

    QByteArray array = end.toLatin1();
    char  * buffer = array.data();
    for (int i = 0; i < len; i++){
        if(wordAsArray[index + i] != buffer[i]){ return false; }
    }
    stemIndex = endIndex - len;
    return true;
}

// provides a new ending for the current word
void PorterStem::setEnd(QString end)
{
    int len = end.length();
    int index = stemIndex + 1;
    //conversion necessary for qt types
    QByteArray array = end.toLatin1();
    char  * buffer = array.data();
    for (int i = 0; i < len; i++)
    {
        wordAsArray[index + i] = buffer[i];
    }
    endIndex = stemIndex +len;
}

// returns true if character at specified index in word is a consonant
bool PorterStem::isConsonant(int i)
{
    switch (wordAsArray[i]){
        case 'a': case 'e': case 'i': case 'o': case 'u': return false;
        case 'y': return (i==0) ? true : !this->isConsonant(i-1);
        default: return true;
    }
}

int PorterStem::countM()
{
    int n = 0;
    int index = 0;
    while(true){
        if(index > stemIndex) return n;
        if(!isConsonant(index)) break;
        index++;
    }
    index++;
    while(true){
        while(true) {
            if (index > stemIndex) return n;
            if (isConsonant(index)) break;
            index++;
        }
        index++;
        n++;
        while(true){
            if(index > stemIndex) return n;
            if(!isConsonant(index)) break;
            index++;
        }
        index++;
    }

}

// is there a vowel in current stem?
bool PorterStem::vowelInStem()
{
    for(int i = 0; 0 < stemIndex; i++){
        if(!isConsonant(i)) return true;
    }
    return false;
}

// for restoring 'e' at the end of short words, e.g.:
// cav(e), crim(e), lov(e), but not snow, box, tray
bool PorterStem::isCVC(int index)
{
    bool ret = false;
    if(index < 2
        || !isConsonant(index)
        || isConsonant(index - 1)
        || !isConsonant(index - 2)){
            return ret;
    }
    char c = wordAsArray[index];
    return c != 'w' && c != 'x' && c != 'y';
}

bool PorterStem::isDoubleConsonant(int index){
    if(index < 1) return false;
    return wordAsArray[index] == wordAsArray[index - 1] && isConsonant(index);
}

void PorterStem::replaceEnd(QString end){
    if(countM() > 0){
        setEnd(end);
    }
}
