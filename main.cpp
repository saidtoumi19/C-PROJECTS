#include <string>
#include <cctype>
#include <iostream>
#include <unordered_map>

using namespace std;


class Token {
public:
    enum TokenType { LE, LT, EQ, ASSIGN, GE, GT, NE, OR, BITOR, AND, BITAND };
    TokenType type;
};

void fail() {
    std::cout << "Erreur : opérateur relop non reconnu." << std::endl;
}

class LexicalAnalyser {
private:
    string output;
    int line = 1;
    char forward;
    const char* leximebegin;
    const char* input;

public:
    getOutput(){
        cout << output << endl;
    }
    void readch() {
        if (*input != '\0') {
            forward = *input++;
        } else {
            forward = '\0';
        }
    }


  //  use  BUFFER [X++] + 2
public:
    LexicalAnalyser(const string& inputStr) {
        input = inputStr.c_str();
        leximebegin = input;
        readch();
    }

    void stringAnalysis() {
        unordered_map<string, string> symbolTable;
        symbolTable["if"] = "MOT_CLE";
        symbolTable["while"] = "MOT_CLE";
        symbolTable["for"] = "MOT_CLE";

        while (forward != '\0') {
            for (; ; readch()) {
                if (forward == ' ' || forward == '\t' || forward == ';') continue;
                else if (forward == '\n') line++;
                else break;
            }


            int state = 0;
            bool tokenFound = false;

            while (!tokenFound) {
                switch (state) {
                    case 0:
                        if (isalpha(forward)) {
                            // Identificateurs ou mots-clés
                            string buffer;
                            do {
                                buffer += forward;
                                readch();
                            }
                             while (isalpha(forward) || isdigit(forward));

                                    if (symbolTable.find(buffer) != symbolTable.end()) {
                                        output += symbolTable[buffer] + " ";
                                    } else {
                                        output += "IDENTIFIER ";
                                    }
                                    tokenFound = true;
                                } else if (isdigit(forward)) {
                                    // Nombres
                                    string number;
                                    do {
                                        number += forward;
                                        readch();
                                    } while (isdigit(forward));

                                    output += "NOMBRE ";
                                    tokenFound = true;
                                } else {
                                    // Automate pour les opérateurs
                                    if (forward == '<') state = 1;
                                    else if (forward == '=') state = 4;
                                    else if (forward == '>') state = 7;
                                    else if (forward == '!') state = 10;
                                    else if (forward == '|') state = 12;
                                    else if (forward == '&') state = 15;
                                    else {
                                        fail();
                                        return;
                                    }
                                    readch();
                                }
                                break;

                            case 1: // '<' ou '<='
                                if (forward == '=') {
                                    output += "LE ";
                                    readch();
                                } else {
                                    output += "LT ";
                                }
                                tokenFound = true;
                                break;

                            case 4: // '=' ou '=='
                                if (forward == '=') {
                                    output += "EQ ";
                                    readch();
                                } else {
                                    output += "ASSIGN ";
                                }
                                tokenFound = true;
                                break;

                            case 7: // '>' ou '>='
                                if (forward == '=') {
                                    output += "GE ";
                                    readch();
                                } else {
                                    output += "GT ";
                                }
                                tokenFound = true;
                                break;

                            case 10: // '!='
                                if (forward == '=') {
                                    output += "NE ";
                                    readch();
                                    tokenFound = true;
                                } else {
                                    fail();
                                    return;
                                }
                                break;

                            case 12: // '||' ou '|'
                                if (forward == '|') {
                                    output += "OR ";
                                    readch();
                                } else {
                                    output += "BITOR ";
                                }
                                tokenFound = true;
                                break;

                            case 15: // '&&' ou '&'
                                if (forward == '&') {
                                    output += "AND ";
                                    readch();
                                } else {
                                    output += "BITAND ";
                                }
                                tokenFound = true;
                                break;

                            default:
                                fail();
                                return;
            }}
                   }

    }
};

int main() {
    std::string inputStr = "if a == b && c != d || e > 10  int x = 42;";
    LexicalAnalyser lex = LexicalAnalyser(inputStr);
    lex.stringAnalysis();
    lex.getOutput();
    return 0;
}
