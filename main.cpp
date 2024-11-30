
#include <iostream>
#include <iomanip>
#include <fstream>
#include <string>
#include <cctype>
#include <algorithm>
#include <stack>
#include <cmath>
using namespace std;


const unsigned int AV_SENTENCE_LENGTH=10;
const unsigned int AV_WORD_LENGTH=5;

int totalNrOfWords= 0;
int totalNrOfUniqueWords=0;
int uniqueMoreThanThreeCharacters=0;
int nrWordsMoreThanThreeCharacters=0;
int totalNrCharacters=0;
int nrSentences=0;
int averageWordLength=0;
int frequencyCounter;
int averageSentenceLength=1;
int usedMostOftenCounter=0;
string theWordUsedMostOften;


// FUNCTION PROTOTYPES
void readFile(ifstream&);
string removeSpecialCharacters(string&);
int countNrSentences(string&);
void displayStats();
void displayStyleWarnings();



// THIS IS HOW A NODE LOOKS LIKE
struct bstNode{
    string info;
    int count;
    int length;
    bstNode *left= nullptr;
    bstNode *right= nullptr;
};

bstNode *root = NULL;


// CREATE A NEW NODE
bstNode *newNode(const string& item){

    bstNode* temp= new bstNode;
    temp->info = item;
    temp->length=item.size();
    temp->count=1;
    temp->left =NULL;
    temp->right = NULL;
    return temp;
}


// INSERTS A WORD IN THE TREE
bstNode* insert(bstNode* bstNode, const string &info)
{
    // IF ROOT IS NULL OR THE WORD IS NOT FOUND
    if (bstNode == NULL) {
        return newNode(info);
    }

    //IF THE WORD EXISTS INCREMENT COUNTER AND RETURN
    if (info == bstNode->info){
        bstNode->count++;
        return bstNode;
    }

    // CHECK THE REST OF THE TREE
    if (info < bstNode->info){
        bstNode->left = insert(bstNode->left, info);
    }
    else{
        bstNode->right = insert(bstNode->right, info);
    }
    return bstNode;
}



// IN-ORDER TRAVERSAL FOR DISPLAYING STATISTICS
void inorder(bstNode *root)
{
    if (root != NULL)
    {
        inorder(root->left);

        // --------- GET STATISTICS--------------

        totalNrOfWords += root->count;
        totalNrCharacters += (root->length * root->count);

        if (root->length > 3) {
            nrWordsMoreThanThreeCharacters += root->count;
        }

            totalNrOfUniqueWords++;

            if (root->length > 3) {
                uniqueMoreThanThreeCharacters++;
            }


        if(root->length>3 && root->count>usedMostOftenCounter){
            usedMostOftenCounter=root->count;
            theWordUsedMostOften=root->info;
        }

        //-----------------------------------

        inorder(root->right);
    }
}



// IN-ORDER TRAVERSAL FOR STYLE WARNING

void inorderStyleWarning(bstNode *root){

    if (root != NULL)
    {
        inorderStyleWarning(root->left);

        //---------------------- STYLE WARNING ------------------------

        double percentage = (0.05*nrWordsMoreThanThreeCharacters);
        if(root->length>3 && root->count>percentage){
            frequencyCounter++;
            cout<<"\n "<<frequencyCounter<<") ⚠️ "<<root->info<<" "<<root->count;
        }
        //----------------------------------------------------------------

        inorderStyleWarning(root->right);
    }
}



// ITERATIVE IN-ORDER TRAVERSAL FOR ALPHABETICAL PRINTING
void iterativeInOrder(bstNode *root){
    stack<bstNode *> s;
    bstNode *curr = root;

    bool toPrint=true;
    char first='a';

    while (curr != NULL || !s.empty())  {
        while (curr !=  NULL){
            s.push(curr);
            curr = curr->left;
        }

        curr = s.top();
        s.pop();


        // -------PRINT ALPHABETICALLY ------------

        // MAKING SURE NO EMPTY SPACES ARE INDEXED FOR THE FIRST LETTER, IF THERE ARE ANY!
        if(curr->length>0) {
            char current;

            if(toPrint){
                current=curr->info.at(0);
                if(current=='a'){
                    cout<<"\n _______ A _______\n";
                    toPrint=false;
                }
            }

                current = curr->info.at(0);
                char index = (char) (current - 32);

                if (current != first) {
                    cout << "\n _______ " << index << " _______\n";
                    first = current;
                }

            cout <<" "<<curr->info<< "\n";
        }
        //-----------------------------------------

        curr = curr->right;
    }
}




int main(){

    // ACCURACY(nr of words) |  MY PROGRAM - 15532 , MS WORD - 15581 , GOOGLE DOCS - 15520
    // WAR AND PEACE - CHAPTERS I-X
    //https://www.gutenberg.org/files/2600/2600-h/2600-h.htm



    string fileName;
    cout<<" ENTER THE NAME OF THE FILE: ";
    cin>>fileName;

    ifstream fileObject( fileName.c_str() );
    if(fileObject){
        cout<<"\n ______________________________| ANALYTICS |_______________________________";
        cout<<"\n FILE NAME: "<<fileName<<endl;
        cout<<" --------------------------------------------------------------------------";
        readFile( fileObject );
        fileObject.close();
    }
    else {
        cout<<" THERE WAS A PROBLEM OPENING THE FILE: "
            << fileName << endl;
        fileName="DOESN'T EXIST";
        cout<<" FILE NAME: "<<fileName<<endl; }

    return 0;
}





void readFile(ifstream& fileObject ){
    string input;
    string currentWord;

    // READ ALL THE WORDS UNTIL THE END OF THE FILE
    while(fileObject >> input) {

        nrSentences +=countNrSentences(input);
        currentWord = removeSpecialCharacters(input);
        // MAKE EVERYTHING LOWERCASE TO AVOID PRECEDENCE OF CAPITAL LETTERS WHEN COMPARING
        transform(currentWord.begin(), currentWord.end(), currentWord.begin(), ::tolower);
        root= insert(root,currentWord);

    }

    cout<<"\n";

    // STATISTICAL SUMMARY
    inorder(root);
    displayStats();

    //STYLE WARNING
    displayStyleWarnings();

    // INDEX OF UNIQUE WORDS
    cout<<"\n                         INDEX OF UNIQUE WORDS                            \n";
    cout<<" --------------------------------------------------------------------------";

    iterativeInOrder(root);

}



// COUNT THE NR OF SENTENCES
int countNrSentences(string &s){

    int count =0;
    int periods=0;
    bool found =false;

// COUNTS ONLY ONE ENDMARK IF THERE ARE MANY IN A WORD
    for(char t : s) {
        if (t == '?' || t == '!') {
            count++;
        }
        if(t=='.'){
            periods++;
        }
        if (periods > 1) {
            found = true;
        }
    }
// MANY PERIODS DO NOT INDICATE THE END OF A SENTENCE
// THERE ARE SOME OTHER EDGE CASES HERE THAT I DIDN'T CONSIDER LIKE  Mr.  or Mrs.

    if(found){
        periods=0;
    }
    if(count>1){
        count=1;
    }
    if(count>periods){
        return count;
    } else{
        return periods;
    }

}

// REMOVES SPECIAL CHARACTERS
string removeSpecialCharacters(string &input){

    for (int i = 0; i < input.size(); i++) {
        if ((input[i] < 'A' || input[i] > 'Z') && (input[i] < 'a' || input[i] > 'z')) {
            input.erase(i, 1);
            i--;
        }
    }
    return input;
}



// DISPLAY STATISTICS
void displayStats(){
    if(totalNrOfWords>0) {
        averageWordLength = round((double) totalNrCharacters / totalNrOfWords);
    }
    else{
        averageWordLength=totalNrCharacters;
    }

    cout<<"                          STATISTICAL SUMMARY                               ";
    cout<<"\n --------------------------------------------------------------------------";
    cout<<"\n THE TOTAL NR OF WORDS IS: "<<totalNrOfWords;
    cout<<"\n THE TOTAL NR OF UNIQUE WORDS IS: "<<totalNrOfUniqueWords;
    cout<<"\n THE TOTAL NR OF UNIQUE WORDS MORE THAN 3 LETTERS IS: "<<uniqueMoreThanThreeCharacters;
    cout<<"\n THE AVERAGE LENGTH FOR WORD IS: "<<averageWordLength;
    cout<<"\n THE NUMBER OF SENTENCES IS: "<<nrSentences;
    cout<<"\n THE WORD WITH MORE THAN 3 LETTERS USED MOST OFTEN IS: "<<theWordUsedMostOften<<" - "<<usedMostOftenCounter<<" times";
    cout<<"\n THE NR OF WORDS MORE THAN 3 LETTERS ARE : "<<nrWordsMoreThanThreeCharacters;
    cout<<"\n TOTAL NR OF CHARACTERS IS: "<<totalNrCharacters;
    cout<<"\n --------------------------------------------------------------------------";

}

//DISPLAY WARNINGS
void displayStyleWarnings(){

    if(nrSentences>0) {
        averageSentenceLength = round((double) totalNrOfWords / nrSentences);
    }
    else{
        averageSentenceLength=totalNrOfWords;
    }


    cout<<"\n                            STYLE WARNINGS                                  ";
    cout<<"\n --------------------------------------------------------------------------\n";

    cout<<" -WORDS USED TOO OFTEN: (Words of more than 3 letters that are used more \n  than"
          " 5% of the total number of words) ARE: \n";
    inorderStyleWarning(root);
    if(frequencyCounter==0){
        cout<<" -✅ NONE! "<<0;
    }
    if(averageSentenceLength>AV_SENTENCE_LENGTH) {
        cout << "\n\n -⚠️ AVERAGE SENTENCE LENGTH TOO LONG - " <<averageSentenceLength<<" words";
    } else{
        cout << "\n\n -✅ AVERAGE SENTENCE LENGTH LOOKS GOOD! - " <<averageSentenceLength<<" words";
    }

    if(averageWordLength>AV_WORD_LENGTH) {
        cout << "\n -⚠️ AVERAGE WORD LENGTH TOO LONG - " <<averageWordLength<<" characters";
    } else{
        cout << "\n -✅ AVERAGE WORD LENGTH LOOKS GOOD! - " <<averageWordLength<<" characters";

    }

    cout<<"\n --------------------------------------------------------------------------";

}
