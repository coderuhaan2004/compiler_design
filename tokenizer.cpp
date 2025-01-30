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
#define EXPRESSION 8

double numVal;     // Declaring global variable for numeric literal
string idenStr;    // GV for tok_identifier
string commentStr; // GV for comments
string opStr;      // GV for operators
string strliteral; // GV for string literals
string expressionStr;

int gettok(ifstream &inputFile)
{
	// Processing identifiers
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
    // Processing expressions inside parentheses
	if (lastChar == '(')
	{
	    expressionStr = "";
	    int openParens = 1;
	    while (openParens > 0 && (lastChar = inputFile.get()) != EOF)
	    {
		if (lastChar == '(') 
		    openParens++;
		else if (lastChar == ')') 
		{
		    openParens--;
		    if (openParens == 0) 
		        break;  // Stop when the outermost closing ')' is found
		}
		if (openParens > 0)  // Only store the inner expression, ignore outermost `(`
		    expressionStr += lastChar;
	    }
	    return EXPRESSION;
	}
    // Processing operators
    if (strchr("+-*/<>=!&|;", lastChar))
    {
        opStr = lastChar;
        while (strchr("+-*/<>=!&|", (lastChar = inputFile.get())))
        {
            opStr += lastChar;
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
                    cerr << "Error: Invalid number detected -> '" << numStr
                         << "'. Multiple decimal points are not allowed." << endl;
                    while (isdigit(lastChar = inputFile.get()) || lastChar == '.')
                        ;
                    return -1; 
                }
                hasDecimal = true;
            }
            lastChar = inputFile.get();
        } while (isdigit(lastChar) || lastChar == '.');
        if (numStr.back() == '.')
        {
            cerr << "Error: Invalid number detected -> '" << numStr
                 << "'. Number cannot end with a decimal point." << endl;
            return -1;
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
    int2token[EXPRESSION] = "Expression";

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
        cout <<"Token: "<<int2token[token] << " ";
        if (token == IDENTIFIER)
            cout <<"Value: "<<idenStr;
        if (token == NUMBERS)
            cout <<"Value: "<<numVal;
        if (token == COMMENTS)
            cout <<"Value: "<<commentStr;
        if (token == OPERATORS)
            cout <<"Value: "<<opStr;
        if (token == LITERAL)
            cout <<"Value: "<<strliteral;
        if (token == EXPRESSION)
            cout <<"Value: "<<expressionStr;
		cout<<endl;
    }
	
    inputFile.close();
    return 0;
}

