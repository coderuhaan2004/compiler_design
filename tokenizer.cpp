#include <bits/stdc++.h>

using namespace std;

// Define token codes for each identifier
#define IDENTIFIER 0
#define NUMBERS 1
#define OPERATORS 2
#define COMMENTS 3
#define DEF 4
#define EXTERN 6
#define EOFTOKEN 7

double numVal; // Declaring global variable for numeric literal 
string idenStr; // GV for tok_identifier
//string def_token;
//string extern_token;

int gettok(ifstream &inputFile){
    int lastChar = ' ';
    // Skip any whitespace
    while(isspace(lastChar)){
        lastChar = inputFile.get();
        if(lastChar == EOF) return EOFTOKEN;
    }
    if(isalpha(lastChar)){
        idenStr = lastChar;
        while(isalnum((lastChar = inputFile.get()))){
            if(lastChar == EOF) return EOFTOKEN;
            idenStr += lastChar;
        }
        if(idenStr == "def")
            return DEF;
        if(idenStr == "extern")
            return EXTERN;
        return IDENTIFIER; // All words except "def" & "extern" are just identifiers
    }
    // Processing numbers
    if(isdigit(lastChar) || lastChar == '.'){
        string numStr;
        do{
            numStr += lastChar;
            lastChar = inputFile.get();
            if(lastChar == EOF) return EOFTOKEN;
        } while(isdigit(lastChar) || lastChar == '.');
        numVal = strtod(numStr.c_str(), 0);
        return NUMBERS;
    }
    // Processing comments
    if(lastChar == '#'){
        do{
            lastChar = inputFile.get();
            if(lastChar == EOF) return EOFTOKEN;
        } while(lastChar != EOF && lastChar != '\n' && lastChar != '\r');
        if(lastChar != EOF)
            return gettok(inputFile);
    }
    // Handle some unmatched inputs
    if(lastChar == EOF){
        return EOFTOKEN;
    }
    // Return character as its ASCII value
    int thisChar = lastChar;
    lastChar = inputFile.get();
    return thisChar;
}

int main(){
    map<int, string> int2token;
    int2token[IDENTIFIER] = "Identifier";
    int2token[NUMBERS] = "Number";
    int2token[COMMENTS] = "Comments";
    int2token[OPERATORS] = "Operator";
    int2token[EOFTOKEN] = "EOF";
    int2token[DEF] = "def_token";
    int2token[EXTERN] = "extern_token";

    ifstream inputFile("code.txt");
    if(!inputFile.is_open()){
        cerr << "Error opening file" << endl;
        return 1;
    }

    int token;
    while(true){
        token = gettok(inputFile);
        if(token == EOFTOKEN) break;
        cout << int2token[token] << " ";
        if(token == IDENTIFIER) cout << idenStr;
        if(token == NUMBERS) cout << numVal;
        cout << endl;
    }

    inputFile.close();
    return 0;
}

