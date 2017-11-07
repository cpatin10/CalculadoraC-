#include "scanner.h" 
#include "calcex.h"
#include <iostream>
#include <string>
#include <cctype>
#include <fstream>
#include <sstream>

using namespace std;

//Uncomment this to get debug information
//#define debug

const int numberOfKeywords = 6;

const string keywd[numberOfKeywords] = {
  string("S"), string("R"), string("M"), string("P"), string("C"), string("let")
};

int isWhiteSpace(char c) {
  return (c == ' ' || c == '\t' || c == '\n');
}

Scanner::Scanner():
  lineCount(1),
  colCount(-1),
  needToken(true),
  lastToken(0)
{
  inStream = new istringstream(terminalLine());
}

Scanner::Scanner(string file):
  lineCount(1),
  colCount(-1),
  needToken(true),
  lastToken(0)
{
  inStream = new istringstream(fileContent(file));
}

Scanner::~Scanner() {
   try {
      delete inStream;
   } catch (...) {}
}

void Scanner::putBackToken() {
   needToken = false;
}

Token* Scanner::getToken() {
  if (!needToken) {
    needToken=true;
    return lastToken;
  }

  Token* t;
  int state=0;
  bool foundOne=false;
  char c;
  string lex;
  TokenType type;
  int k;
  int column, line;

  c = inStream->get();

  while (!foundOne) {
    colCount++;
    switch (state) {
    case 0 : 
      lex = "";
      column=colCount;
      line = lineCount;
      if (isalpha(c)) state=1;
      else if (isdigit(c)) state=2;
      else if (c=='+') state=3;
      else if (c=='-') state=4;
      else if (c=='*') state=5;
      else if (c=='/') state=6;
      else if (c=='(') state=7;
      else if (c==')') state=8;
      else if (c=='%') state=9;
      else if (c=='=') state=10;
      else if (c=='\n') state=11; 
      else if (isWhiteSpace(c));
      else if (inStream->eof() || c == -1) {
	foundOne=true;
	type=eof;
      }
      else {
	foundOne=true;
	type=unrecognized;	
      }
      break;
    case 1 : if (isalpha(c) || isdigit(c)) state=1;
      else {
	for (k=0;k<numberOfKeywords;k++)
	  if (lex == keywd[k]) {
	    foundOne = true;
	    type = keyword;
	  }
	if (!foundOne) {
	  type = identifier;
	  foundOne = true;
	}
      }
      break;
    case 2 :
      if (isdigit(c)) state=2;
      else {
	type = number;
	foundOne=true;
      }
      break;
    case 3 :
      type = add;
      foundOne = true;
      break;
    case 4 :
      type = sub;
      foundOne = true;
      break;
    case 5 :
      type = times;
      foundOne=true;
      break;
    case 6 :
      type = divide;
      foundOne=true;
      break;
    case 7 :
      type = lparen;
      foundOne=true;
      break;
    case 8 :
      type = rparen;
      foundOne=true;
      break;
    case 9 :
      type = mod;
      foundOne = true;
      break;
    case 10 :
      type = equals;
      foundOne = true;
      break;
    case 11 :
      colCount=0; 
      lineCount++;
      foundOne=true;
      type=eol;
    }
      
    if (!foundOne) {
      lex = lex + c;
      c = inStream->get();
    }
  }

  inStream->putback(c);

  colCount--;
  if (type == number || type == identifier || type == keyword) {
    t = new LexicalToken(type,new string(lex), line, column);
  }
  else {
    t = new Token(type,line,column);
  }

#ifdef debug
  cout << "just found " << lex << " with type " << type << endl;
#endif

  lastToken = t;
  return t;
}

string Scanner::fileContent(string file) {
  const char *c = file.c_str();
  ifstream infile(c);
  string content((istreambuf_iterator<char>(infile)), istreambuf_iterator<char>());
  infile.close();
  return content;
}

string Scanner::terminalLine() {
  string line;
  cout << "> ";
  getline(cin, line);
  if (!cin.eof()) {
    line += "\n";
  }
  return line;
}

void Scanner::setInStream() {
  inStream = new istringstream(terminalLine());
}
