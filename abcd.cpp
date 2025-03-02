#include <iostream>
#include <fstream>
#include <string>
//#include <Windows.h>


using namespace std;

const int alphaSize = 26;//size of alphabet 
const int MaxSuggestion = 10;//total no of suggestions as per manuual 

struct Trie { // node with 3 parts eow , size and its mean 
    bool EOW; // end of word 
    Trie* child[alphaSize];
    string meaning;
};

Trie* NewNode() {//creating and initilizing new node
    Trie* node = new Trie;
    node->EOW = false;
    for (int i = 0; i < alphaSize; ++i) {
        node->child[i] = nullptr;
    }
    return node;
}

void animation()
{
    cout << "DDDDDDD  IIIIIIIII  CCCCCCCC TTTTTTTTT IIIIIIIII   OOOOO    NNN   NNN AAAAAAAAA RRRRRR   YYY     YYY  |''''|   \n";
    cout << "DDDDDDDD IIIIIIIII CCCCCCCCC TTTTTTTTT IIIIIIIII  OOOOOOO   NNNN  NNN AAAAAAAAA RRR  RRR  YYY   YYY   |    |   \n";
    cout << "DDD   DDD   III   CCCC          TTT       III    OOO   OOO  NNNNN NNN AAA   AAA RRR   RRR  YYYYYYY    |    |   \n";
    cout << "DDD   DDD   III   CCC           TTT       III   OOO     OOO NNNNNNNNN AAAAAAAAA RRR  RRR     YYY      |____|   \n";
    cout << "DDD   DDD   III   CCCC          TTT       III    OOO   OOO  NNN NNNNN AAAAAAAAA RRRRRR       YYY               \n";
    cout << "DDDDDDDD IIIIIIIII CCCCCCCCC    TTT    IIIIIIIII  OOOOOOO   NNN  NNNN AAA   AAA RRR  RRR     YYY      |""""|   \n";
    cout << "DDDDDDD  IIIIIIIII  CCCCCCCC    TTT    IIIIIIIII   OOOOO    NNN   NNN AAA   AAA RRR   RRR    YYY      |____|   \n\n\n\n";
    system("PAUSE");
}



int GETindex(char ch)
{    //returns index of chsrctrter in array 
    return ch - 'a';
}


void insert(Trie*& root, string& word, string& mean) {// insert the word in tire node
    if (root == nullptr)
        root = NewNode(); // if empty create new node

    Trie* temp = root;
    // traverse to make node for each character of word

    for (int i = 0; i < word.length(); i++)
    {
        char x = word[i];
        int index = GETindex(x);
        if (temp->child[index] == nullptr)
        {
            temp->child[index] = NewNode();
        }
        temp = temp->child[index];
    }
    // used as flag; jb insert ho to to eow ko true kry and store meaning in trie node meaning part
    temp->EOW = true;
    temp->meaning = mean;
}

bool wordExistsInFile(const string& fileName, const string& word) {
    ifstream checkFile(fileName);
    string wordFromFile;

    while (checkFile >> wordFromFile) {
        if (wordFromFile == word) {
            cout << "Word already exist in file, cannot enter same word again\n";
            checkFile.close();
            return true;
        }
        // Skip the meaning part
        checkFile >> wordFromFile;
    }

    checkFile.close();
    return false;
}

void LoadDictionary(Trie*& root) {//loads the dictionary words from dictionary.txt file iusing file handling 
    ifstream myfile("Dictionary.txt");
    string word, meaning;
    int count = 0;
    cout << "DICTIONARY IS LOADING" << endl;
    if (myfile.is_open()) {//file handlig use ho rhi
        while (myfile >> word >> meaning) {
            insert(root, word, meaning);
            count++;
        }
        myfile.close();
    }
    else {
        cout << "Unable to open file." << endl;//if the .txt file is  not found
    }
    cout << "Dictionary contains " << count << " words." << endl;//if the .txt file is found
}

string SearchWord(Trie* root, const string& word) {
    // search the word in given .txt file
    if (root == nullptr)
        return " ";

    Trie* temp = root;

    //traverse through each chracter to find entered word

    for (int i = 0; i < word.length(); i++) {
        int index = GETindex(word[i]);
        temp = temp->child[index];
        if (temp == nullptr)
            return "No word found.";
    }

    if (temp->EOW) {
        return "Meaning: " + temp->meaning;
    }
    else {
        return "  ";
    }
}

bool delFunc(Trie* root, const string& wrd, int x) {
    if (!root) return false;

    if (wrd.size() == x) {
        if (!root->EOW) {
            return false;
        }
        root->EOW = false;
        return true;
    }

    int index = GETindex(wrd[x]);
    if (delFunc(root->child[index], wrd, x + 1)) {
        // If the child node is deleted, check if it has no children
        if (root->child[index] == nullptr) {
            delete root->child[index];
            root->child[index] = nullptr;
            return true;
        }
    }

    return false;
}

void deleteWord(Trie* root, const string& s) { //main deleteWord function called from int main()
    if (!root)
        return;

    delFunc(root, s, 0);
}

void AddWord(Trie* root, string newword, string mean) {
    // Check if the word already exists in the dictionary
    /*if (wordExistsInFile("Dictionary.txt", newword)) {
        cout << "The word already exists in the dictionary. You cannot add the same word again." << endl;
        return;
    }*///SearchWord(Trie* root, const string& word)
    // if (SearchWord(root, newword) == "No word found.")
    {

        // Open the file in output mode to append the new word
        ofstream file("Dictionary.txt", ios::out | ios::app);

        if (file.is_open()) {
            file << newword << "\t" << mean << endl; // Writing new word to file
            file.close();
            LoadDictionary(root);
            cout << "Word added." << endl;
        }
        else {
            cout << "Unable to open file for writing." << endl;
        }
    }

}

void UpdateWord(Trie* root, const string& word) {
    string mean;//store new meaning 
    cout << "Enter new meaning for the word :";
    cin >> mean;

    deleteWord(root, word);//delete exsisting 
    AddWord(root, word, mean);//updating wuth new 
    cout << "Word updated." << endl;
}
//depth first search for travrse and search for suggwsetion 

void Dfs(Trie* node, const string& prefix, int& count, string suggestions[]) {
    if (node == nullptr)
        return;

    if (node->EOW && count < MaxSuggestion) {
        suggestions[count++] = prefix;
    }

    if (count >= MaxSuggestion)//chk if max sugg reach which is const num =10
        return;
    // child node exists, recursively call DFS 
    for (int i = 0; i < alphaSize; ++i) {
        if (node->child[i] != nullptr) {
            Dfs(node->child[i], prefix + char('a' + i), count, suggestions);
            if (count >= MaxSuggestion)
                return;
        }
    }
}

// WordSuggestions function to display top 10 suggestions using above dfs function 
void ContinuouswordSuggestions(Trie* root, string alpha) {


    Trie* temp = root;
    string suggestions[MaxSuggestion];
    int count = 0;

    for (int i = 0; i < alpha.length(); i++) {
        int index = GETindex(alpha[i]);
        if (temp == nullptr || temp->child[index] == nullptr) {
            cout << "No suggestions for entered alphabets :" << endl;
            return;
        }
        temp = temp->child[index];
    }

    cout << "Word suggestions for entered alphabets: " << endl;

    while (true) {
        char nextChar;
        cin.get(nextChar);

        if (nextChar == '\n') {
            // User pressed enter, exit the loop
            break;
        }

        int index = GETindex(nextChar);
        if (temp == nullptr || temp->child[index] == nullptr) {
            cout << "No suggestions for entered alphabets :" << endl;
            return;
        }

        alpha += nextChar;
        temp = temp->child[index];
        Dfs(temp, alpha, count, suggestions);

        if (count == 0) {
            cout << "No suggestions available." << endl;
        }
        else {
            cout << "Suggestions: ";
            for (int i = 0; i < count; ++i) {
                cout << suggestions[i] << " ";
            }
            cout << endl;
        }
    }

    cout << "Selected word: " << alpha << endl;
}

void WordSuggestions(Trie* root, string prefix) {
    Trie* temp = root;
    string suggestions[MaxSuggestion];
    int count = 0;

    for (int i = 0; i < prefix.length(); i++) {
        int index = GETindex(prefix[i]);
        if (temp == nullptr || temp->child[index] == nullptr) {
            cout << "No suggestions for entered alphabets:" << endl;
            return;
        }
        temp = temp->child[index];
    }

    Dfs(temp, prefix, count, suggestions);

    if (count == 0) {
        cout << "No suggestions available." << endl;
    }
    else {
        for (int i = 0; i < count; ++i) {
            cout << suggestions[i] << endl;
        }
    }

}
bool hasNumber(string& str) {
    for (char ch : str) {
        if (isdigit(ch)) {
            return true; // Found a digit, return true
        }
    }
    return false; // No digit found in the string
}
bool hasspace(string& str) {
    for (char ch : str) 
    {
        if (isspace(ch)) {
            return true; // Found space, return true
        }
    }
    return false; // No space found in the string
}

void ToLowerCase(string& str)
{
    for (int i = 0; i < str.length(); i++)
    {
        str[i] = tolower(str[i]);
    }
}


int main() {
    Trie* root = nullptr;

    string del, update, newWord, newmean, suggest;
    char choice = 0;
    system("color 3F");
    animation();
    system("cls");

    cout << "\t\t\t\tD I C T I O N A R Y ";
    //while(!(choice<1) && !(choice>7))
    while (choice != '7') {
        system("color 70");
        cout << "\n1. Load Dictionary." << endl;
        cout << "2. Add word." << endl;
        cout << "3. Search word." << endl;
        cout << "4. Delete word." << endl;
        cout << "5. Update word." << endl;
        cout << "6. Word suggestion." << endl;
        cout << "7. Exit." << endl;

        cout << "Enter choice: ";
        cin >> choice;
        system("cls");
        switch (choice) {
        case '1':
            LoadDictionary(root);
            break;

        case '2':
            cin.ignore();
            cout << "Enter new word to add in the dictionary: ";
            getline(cin, newWord);
            ToLowerCase(newWord);
            while (hasNumber(newWord) || hasspace(newWord))
            {
                
                cout << "Word cannot contain number or space\nEnter word again: ";
                getline(cin, newWord);
            }
            cout << "Enter meaning of the word: ";
            getline(cin, newmean);
            ToLowerCase(newmean);
            while (hasNumber(newmean) || hasspace(newmean))
            {

                cout << "Meaning cannot contain number or space\nEnter meaning again: ";
                getline(cin, newmean);
            }

            if (SearchWord(root, newWord) == "No word found.")
            {
                AddWord(root, newWord, newmean);
                cout << "New word added." << endl;
            }
            else
            {
                cout << "The word already exists in the dictionary." << endl;
            }
            break;
        case '3':

            cout << "Enter word to search: ";
            cin >> update;
            ToLowerCase(update);

            cout << SearchWord(root, update) << endl << endl;

            break;
        case '4':

            cout << "Enter word to delete: ";
            cin >> del;
            ToLowerCase(del);
            if (SearchWord(root, newWord) == "No word found.")
            {
                cout << newWord << "is not in the dictionary" << endl;
            }
            else
                deleteWord(root, del);
            cout << "Word deleted successfully." << endl;
            break;
        case '5':

            cout << "Enter word to update: ";
            cin >> update;
            ToLowerCase(del);

            UpdateWord(root, update);
            break;
        case '6':

            cout << "Enter Alphabets for suggestions: ";
            cin >> suggest;
            ToLowerCase(del);
            WordSuggestions(root, suggest);
            break;

        case '7':
            cout << "Program terminated sucessfully\n\n\n";
            break;
        default:
            continue;

        }
    }

    return 0;
}
