#pragma once

#include <string>
#include <map>
#include <vector>
#include "ast.h"

#include <fstream>
 
using namespace std;


class Calculator {
 public:
  Calculator();
  void eval();
  void evalTree(string filename);
  void store(int val);
  int recall();
  int getValueIdent(string name);
  int getValueInitIdent(string name);
  void storeIdent(int val, string name);
  void storeInitIdent(int val, string name);
  vector<string> getIdent(bool all);
  void setEweComp(bool ewecomp);
  void setReadFile(bool readfile);
  void addFile(string filename);
  void writeFile(string name, AST* tree);

 private:
  int memory;
  map<string, int> identifiers;
  map<string, int> initialIdentifiers;    
  bool ewecomp;
  bool readfile;
  vector<string> readingfiles;
};

string limitFileName(string s);

extern Calculator* calc;
