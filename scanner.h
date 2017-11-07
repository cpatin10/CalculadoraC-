#pragma once

#include <iostream>
#include "token.h"

class Scanner {
 public:
  Scanner(); 
  Scanner(string file);
  ~Scanner();

  Token* getToken();
  void putBackToken();

  string fileContent(string file);
  string terminalLine();
  void setInStream();
 
 private:
  istream* inStream;
  int lineCount;
  int colCount;

  bool needToken;
  Token* lastToken;

};
