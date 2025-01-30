#include <bits/stdc++.h>

using namespace std;

// Define token codes for each identifier
#define IDENTIFIER 0
#define NUMBERS 1
#define OPERATORS 2
#define COMMENTS 3
#define DEF 4
#define LITERAL 5
#define EXTERN 6
#define EOFTOKEN 7

double numVal;     // Declaring global variable for numeric literal
string idenStr;    // GV for tok_identifier
string commentStr; // GV for comments
string opStr;      // GV for operators
string strliteral; // GV for string literals

int gettok(ifstream &inputFile)
{
    int lastChar = ' ';
    // Skip any whitespace
    while (isspace(lastChar))
    {
        lastChar = inputFile.get();
        if (lastChar == EOF)
            return EOFTOKEN;
    }
    if (isalpha(lastChar))
    {
        idenStr = lastChar;
        while (isalnum((lastChar = inputFile.get())))
        {
            if (lastChar == EOF)
                return EOFTOKEN;
            idenStr += lastChar;
        }
        if (idenStr == "def")
            return DEF;
        if (idenStr == "extern")
            return EXTERN;
        return IDENTIFIER; // All words except "def" & "extern" are just identifiers
    }
    // Processing string literals
    if (lastChar == '"')
    {
        strliteral = "";
        while ((lastChar = inputFile.get()) != '"')
        {
            if (lastChar == EOF)
                return EOFTOKEN;
            if (lastChar == '\n' || lastChar == '\r')
            { // if the string literal is not enclosed in double quotes
                cerr << "Error: Invalid string literal detected -> '" << strliteral
                     << "'. String literals must be enclosed in double quotes." << endl;
                return -1; // Return an invalid token or handle error accordingly
            }
            strliteral += lastChar; // append the character to the string literal
        }
        lastChar = inputFile.get(); // skip the closing double quote
        return LITERAL;             // return the token for string literals
    }
    // Processing operators
    // there can be multiple operators within one line
    if (lastChar == '+' || lastChar == '-' || lastChar == '*' || lastChar == '/' || lastChar == '<' || lastChar == '>' || lastChar == '=' || lastChar == '!' || lastChar == '&' || lastChar == '|')
    {
        opStr = lastChar;
        while (lastChar = inputFile.get())
        {
            if (lastChar == EOF)
                return EOFTOKEN; // Return EOF token if the end of file is reached
            if (lastChar == '+' || lastChar == '-' || lastChar == '*' || lastChar == '/' || lastChar == '<' || lastChar == '>' || lastChar == '=' || lastChar == '!' || lastChar == '&' || lastChar == '|')
            {
                opStr += lastChar;
            }
            else
            {
                break; // break the loop if the character is not an operator.
            }
        }
        return OPERATORS;
    }
    // Processing numbers
    if (isdigit(lastChar) || lastChar == '.')
    {
        string numStr;
        bool hasDecimal = false;
        do
        {
            numStr += lastChar;
            if (lastChar == '.')
            {
                if (hasDecimal)
                { // if there is already a decimal point in the number
                    // raise error as this is not a valid number because it has more than one decimal points
                    cerr << "Error: Invalid number detected -> '" << numStr
                         << "'. Multiple decimal points are not allowed." << endl;
                    // Stop processing and discard the rest of the invalid number
                    while (isdigit(lastChar = inputFile.get()) || lastChar == '.')
                        ;      // Skip the rest of the invalid number by reading the next character and doing nothing
                    return -1; // Return an invalid token or handle error accordingly
                }
                hasDecimal = true; // set hasDecimal to true if a decimal point is found
            }
            lastChar = inputFile.get();
            if (lastChar == EOF)
                return EOFTOKEN;
        } while (isdigit(lastChar) || lastChar == '.');
        // handle the edge case where the number ends with a decimal point
        if (numStr.back() == '.')
        {
            cerr << "Error: Invalid number detected -> '" << numStr
                 << "'. Number cannot end with a decimal point." << endl;
            return -1; // Return an invalid token or handle error accordingly
        }

        numVal = strtod(numStr.c_str(), 0);
        return NUMBERS;
    }
    // Processing comments
    if (lastChar == '#')
    {
        commentStr = "";
        do
        {
            commentStr += lastChar;
            lastChar = inputFile.get();
            if (lastChar == EOF)
                return EOFTOKEN;
        } while (lastChar != EOF && lastChar != '\n' && lastChar != '\r');
        return COMMENTS;
    }
    // Handle some unmatched inputs
    if (lastChar == EOF)
    {
        return EOFTOKEN;
    }
    // Return character as its ASCII value
    int thisChar = lastChar;
    lastChar = inputFile.get();
    return thisChar;
}

int main()
{
    map<int, string> int2token;
    int2token[IDENTIFIER] = "Identifier";
    int2token[NUMBERS] = "Number";
    int2token[COMMENTS] = "Comments";
    int2token[OPERATORS] = "Operator";
    int2token[EOFTOKEN] = "EOF";
    int2token[DEF] = "def_token";
    int2token[EXTERN] = "extern_token";
    int2token[LITERAL] = "Literal";

    ifstream inputFile("code.txt");
    if (!inputFile.is_open())
    {
        cerr << "Error opening file" << endl;
        return 1;
    }

    int token;
    while (true)
    {
        token = gettok(inputFile);
        if (token == EOFTOKEN)
            break;
        cout << int2token[token] << " ";
        if (token == IDENTIFIER)
            cout << idenStr;
        if (token == NUMBERS)
            cout << numVal;
        if (token == COMMENTS)
            cout << commentStr;
        if (token == OPERATORS)
            cout << opStr;
        if (token == LITERAL)
            cout << strliteral;
        cout << endl;
    }

    inputFile.close();
    return 0;
}