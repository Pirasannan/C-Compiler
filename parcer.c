#include <stdio.h>
#include <stdlib.h> //for memory allocation
#include <string.h> 
#include <ctype.h> // for isdigit function , to convert all strings from text to int.

typedef enum{
    TOKEN_NUMBER,
    TOKEN_INT,
    TOKEN_ID, // variable names
    TOKEN_ASSIGN,// =
    TOKEN_PRINT, //print
    TOKEN_PLUS, //+
    TOKEN_MINUS, //-
    TOKEN_MULTIPLY, //*
    TOKEN_DIVIDE, // /
    TOKEN_SEMICOLON, //;
    TOKEN_EOF, //end of file
    TOKEN_LPARAN,  //(  
    TOKEN_LBRACE,  //{
    TOKEN_RBRACE,  //}
    TOKEN_RPARAN,  //)
    TOKEN_ERROR, //for error
} TokenType;

typedef struct {
    TokenType type;
    char text[100]; //to store word, "x", "20"

} Token;

FILE *sourceFile;
Token currentToken;

void tokenizer(){

    int c;
    
    //to skip white spacce
    while ((c= fgetc(sourceFile)) != EOF){
        if (!isspace(c)) break;
    }

    if (c == EOF){
        currentToken.type  = TOKEN_EOF;
        return;
    }

    //check Identifiers and Keywords
    if(isalpha(c)){
        int i =0;
        currentToken.text[i++] = c;
        while (isalnum(c = fgetc(sourceFile))){   // read until its not a letter/number
            currentToken.text[i++] = c;
        }
        ungetc(c, sourceFile);  //if theres any extra char , have to put it back
        currentToken.text[i] = '\0';// Null terminate string

        //check keywords
        if(strcmp(currentToken.text, "int") == 0) currentToken.type = TOKEN_INT ;
        else if (strcmp(currentToken.text, "print") == 0) currentToken.type = TOKEN_PRINT;
        else currentToken.type = TOKEN_ID;
    }

    //check numbers
    else if (isdigit(c)){
        int i = 0;
        currentToken.text[i++] = c;
        while(isdigit(c = fgetc(sourceFile))) {
            currentToken.text[i++] = c;
        }
        ungetc(c, sourceFile);
        currentToken.text[i] = '\0';
        currentToken.type = TOKEN_NUMBER;
    }

    //check symbols
    else{
        currentToken.text[0] = c;
        currentToken.text[1] = '\0';

        switch(c){
            case'=' : currentToken.type = TOKEN_ASSIGN; break;
            case'+' : currentToken.type = TOKEN_PLUS; break;
            case'-' : currentToken.type = TOKEN_MINUS; break;
            case'*' : currentToken.type = TOKEN_MULTIPLY; break;
            case'/' : currentToken.type = TOKEN_DIVIDE; break;
            case';' : currentToken.type = TOKEN_SEMICOLON; break;
            case'(' : currentToken.type = TOKEN_LPARAN; break;
            case')' : currentToken.type = TOKEN_RPARAN; break;
            // case'{' : currentToken.type = TOKEN_LBRACE; break;
            // case'}' : currentToken.type = TOKEN_RBRACE; break;
           default: currentToken.type = TOKEN_ERROR; break;
        }
    }
}


// small database to store variables and values

struct Variable{
    char name[50]; // for "x","y"....
    int value; // for values
};

struct Variable variables[100]; // array to store variables
int varCount = 0; // to track of number of variables

// function to set variable value
void setVariable(char *varName, int val){
    for (int i =0; i < varCount; i++){

        //update exitising variable
        if (strcmp(variables[i].name, varName) == 0){
            variables[i].value = val; 
            return;
        }
    }
    //add new variable
    strcpy(variables[varCount].name, varName);
    variables[varCount].value = val;
    varCount++;
}

// get variable value
int getVariable(char *varname){
    //searching names
    for (int i =0; i < varCount; i++){
        if (strcmp(variables[i].name, varname) == 0){
            return variables[i].value; 
        }
    }
    //semantic error handling
    printf("Error: Variable '%s' not defined.\n", varname);
    exit(1); //which stops the process immediately
}

// Semantic Check: Check if a variable has already been declared.
int isVariableDeclared(char *varName) {
    for (int i = 0; i < varCount; i++) {
        if (strcmp(variables[i].name, varName) == 0) {
            return 1; // Found
        }
    }
    return 0; // Not found
}

// Helper to convert a token type to a human-readable string for error messages.
const char* tokenTypeToString(TokenType type) {
    switch (type) {
        case TOKEN_NUMBER: return "number";
        case TOKEN_INT: return "'int' keyword";
        case TOKEN_ID: return "identifier";
        case TOKEN_ASSIGN: return "'='";
        case TOKEN_PRINT: return "'print' keyword";
        case TOKEN_PLUS: return "'+'";
        case TOKEN_MINUS: return "'-'";
        case TOKEN_MULTIPLY: return "'*'";
        case TOKEN_DIVIDE: return "'/'";
        case TOKEN_SEMICOLON: return "';'";
        case TOKEN_EOF: return "end of file";
        case TOKEN_LPARAN: return "'('";
        case TOKEN_RPARAN: return "')'";
        // case TOKEN_LBRACE: return "'{'";
        // case TOKEN_RBRACE: return "'}'";
        case TOKEN_ERROR: return "error token";
        default: return "unknown token";
    }
}

//Check expected token
void expect(TokenType expected){
    if (currentToken.type == expected){
        tokenizer(); // move to next token
    } else {
        printf("Syntax Error: Unexpected token '%s'. Expected %s.\n", 
               currentToken.text, 
               tokenTypeToString(expected));
        exit(1);
    }
}


//Parsing Logic
int parseExpression(); // Forward declaration for handling parentheses in parseFactor

// Handles numbers, identifiers, and parenthesized expressions.
// <factor> ::= <number> | <identifier> | ( <expression> )
int parseFactor() {
    int val = 0;
    if (currentToken.type == TOKEN_NUMBER) {
        val = atoi(currentToken.text); // Convert string to int
        expect(TOKEN_NUMBER);
    } else if (currentToken.type == TOKEN_ID) {
        val = getVariable(currentToken.text); // Look up variable value
        expect(TOKEN_ID);
    } else if (currentToken.type == TOKEN_LPARAN) {
        expect(TOKEN_LPARAN);
        val = parseExpression(); // Recursively parse the inner expression
        expect(TOKEN_RPARAN);
    } else {
        printf("Syntax Error: Unexpected token '%s' in expression. Expected number, identifier, or '('. \n", currentToken.text);
        exit(1);
    }
    return val;
}

// Handles multiplication and division.
// <term> ::= <factor> { (*|/) <factor> }
int parseTerm() {
    int val = parseFactor();
    while (currentToken.type == TOKEN_MULTIPLY || currentToken.type == TOKEN_DIVIDE) {
        if (currentToken.type == TOKEN_MULTIPLY) {
            expect(TOKEN_MULTIPLY);
            val *= parseFactor();
        } else if (currentToken.type == TOKEN_DIVIDE) {
            expect(TOKEN_DIVIDE);
            int divisor = parseFactor();
            // Runtime Semantic Error: Division by zero
            if (divisor == 0) {
                printf("Runtime Error: Division by zero.\n");
                exit(1);
            }
            val /= divisor;
        }
    }
    return val;
}

// Handles addition and subtraction.
// <expression> ::= <term> { (+|-) <term> }
int parseExpression() {
    int val = parseTerm();
    while (currentToken.type == TOKEN_PLUS || currentToken.type == TOKEN_MINUS) {
        if (currentToken.type == TOKEN_PLUS) {
            expect(TOKEN_PLUS);
            val += parseTerm();
        } else if (currentToken.type == TOKEN_MINUS) {
            expect(TOKEN_MINUS);
            val -= parseTerm();
        }
    }
    return val;
}


void parseStatement() {
    if (currentToken.type == TOKEN_INT) {
        // Found: int x = 5;
        expect(TOKEN_INT);
        
        char varName[20];
        strcpy(varName, currentToken.text); // Save variable name
        expect(TOKEN_ID);

        // Semantic Check: Prevent variable redeclaration.
        if (isVariableDeclared(varName)) {
            printf("Semantic Error: Variable '%s' has already been declared.\n", varName);
            exit(1);
        }
        
        expect(TOKEN_ASSIGN);
        
        int value = parseExpression(); // Calculate the value ... 5 or 20+5
        setVariable(varName, value);   // Store in symbol table
        
        expect(TOKEN_SEMICOLON);
    } 
    else if (currentToken.type == TOKEN_PRINT) {
        // Found: print(z);
        expect(TOKEN_PRINT);
        expect(TOKEN_LPARAN);
        
        int val = parseExpression(); // Parse the expression inside print()
        
        expect(TOKEN_RPARAN);
        expect(TOKEN_SEMICOLON);
        
        printf("%d\n", val); // Actually print it to the screen
    }
    else {
        printf("Syntax Error: Unknown statement starting with %s\n", currentToken.text);
        exit(1);
    }
}

void parseProgram() {
    tokenizer(); // load first token
    // expect(TOKEN_LBRACE);
    while (currentToken.type != TOKEN_EOF) // until end of file
    { 
        // if (currentToken.type == TOKEN_RBRACE) break; 
        parseStatement();
    }
    // expect(TOKEN_RBRACE);
}

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Enter: ./parser <inputfile>\n");
        return 1;
    }

    sourceFile = fopen(argv[1], "r");
    if (!sourceFile) {
        printf("Error: Could not open file %s\n", argv[1]);
        return 1;
    }

    parseProgram();

    fclose(sourceFile);
    return 0;
}