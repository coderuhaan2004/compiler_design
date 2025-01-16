/*Lexical analyser system for simple arithmetic expressions*/

#include <stdio.h>
#include <ctype.h>

/*Global declarations*/
int charClass; //stores the class of the character
char lexeme[100]; //stores the lexeme
char nextChar; //stores the next character
int lexLen; //stores the length of the lexeme
int token; //stores the token
int nextToken; //stores the next token

FILE *in_f, *fopen(); //2 file pointers for input and output files

/*Function declarations*/
void addChar();
void getChar();
void getNonBlank();
int lex();

/*Character Class*/
#define LETTER 0
#define DIGIT 1
#define UNKNOWN 99

/*Token Codes*/
#define INT_LIT 10
#define IDENT 11
#define ASSIGN_OP 20
#define ADD_OP 21
#define SUB_OP 22
#define MULT_OP 23
#define DIV_OP 24
#define LEFT_PAREN 25
#define RIGHT_PAREN 26

/********************************************/
/*main driver function*/

/*Open the input data file and process its contents*/
main() {
    if((in_f = fopen("front.in","r")) == NULL){
        printf("ERROR - cannot open front.in \n");
    } else {
        getChar();
        do {
            lex();
        } while (nextToken != EOF);
    }
}

/********************************************/
/*addChar - add nextChar to lexeme*/

void addChar(){
    if(lexLen <= 98){
        lexeme[lexLen++] = nextChar; //add nextChar to lexeme by incrementing lexLen
        lexeme[lexLen] = 0; //null character at the end
    }
    else
        printf("Error - lexeme is too long \n");
}

/********************************************/
/*getChar - get the next character of input and determine its character class*/

void getChar(){
    if((nextChar = getc(in_f)) != EOF){
        if(isalpha(nextChar))
            charClass = LETTER;
        else if(isdigit(nextChar))
            charClass = DIGIT;
        else
            charClass = UNKNOWN;
    }
    else
        charClass = EOF;
    
}

/********************************************/
/*getNonBlank - call getChar until it returns a non-whitespace character*/

void getNonBlank(){
    while(isspace(nextChar)) //if nextChar is a whitespace character, get the next character by calling getChar which determines the character class
        getChar();
}

/********************************************/
/*lex - a simple lexical analyser*/

int lex(){
    lexLen = 0;
    getNonBlank(); //charClass is now set

    switch(charClass){
        /*Parse identifiers*/
        case LETTER:
            addChar();
            getChar();
            while(charClass == LETTER || charClass == DIGIT){
                addChar();
                getChar();
            }
            nextToken = IDENT;
            break;
        /*Parse integer literals*/
        case DIGIT:
            addChar();
            getChar();
            while(charClass == DIGIT){
                addChar();
                getChar();
            }
            nextToken = INT_LIT;
            break;
        /*Parentheses and operators*/
        case UNKNOWN:
            lookup(nextChar);
            getChar();
            break;
        /*EOF*/
        case EOF:
            nextToken = EOF;
            lexeme[0] = 'E';
            lexeme[1] = 'O';
            lexeme[2] = 'F';
            lexeme[3] = 0;
            break;
    }
    printf("Next token is: %d, Next lexeme is %s\n", nextToken, lexeme);
    return nextToken;
}
