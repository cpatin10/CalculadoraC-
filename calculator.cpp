#include "calculator.h"
#include "parser.h"
#include "ast.h"
#include <string>
#include <iostream>
#include <sstream>
#include <vector>
#include <fstream>

Calculator::Calculator():
  memory(0),
  ewecomp(false),
  readfile(false)
{}

void Calculator::eval() {
  if (readfile) {
    for (int i = 0; i < readingfiles.size(); ++i) {
      memory = 0;
      identifiers.clear();
      identifiers = initialIdentifiers;
      evalTree(readingfiles[i]);
    }
  }
  else {
    identifiers = initialIdentifiers;
    evalTree("");
  }
}

void Calculator::evalTree(string filename) {
  Parser* parser;
  if (readfile) {
    parser = new Parser(filename);
  }
  else {
    parser = new Parser();
  }
  AST* tree = parser->parse();
  writeFile(filename, tree);

  delete tree;   
  delete parser;
}

void Calculator::store(int val) {
  memory = val;
}

int Calculator::recall() {
  return memory;
}

int Calculator::getValueIdent(string name) {
  return identifiers[name];
}

int Calculator::getValueInitIdent(string name) {
  return initialIdentifiers[name];
}

void Calculator::storeIdent(int val, string name) {
  identifiers[name] = val;
}

void Calculator::storeInitIdent(int val, string name) {
  initialIdentifiers[name] = val;
}

vector<string> Calculator::getIdent(bool all) {
  vector<string> v;
  if(all) {
    for (map<string, int>::iterator it = identifiers.begin(); it != identifiers.end(); ++it) {
      v.push_back(it->first);
    }
  }
  else {
    for (map<string, int>::iterator it = initialIdentifiers.begin(); it != initialIdentifiers.end(); ++it) {
      v.push_back(it->first);
    }
  }
  return v;
}

void Calculator::setEweComp(bool ewecomp) {
  this->ewecomp = ewecomp;
}

void Calculator::setReadFile(bool readfile) {
  this->readfile = readfile;
}

void Calculator::addFile(string filename) {
  readingfiles.push_back(filename);
}    

void Calculator::writeFile(string name, AST* tree) {
  if (ewecomp) {
    ofstream outfile;
    if (name != "") {
      name = limitFileName(name);
      name += ".ewe";
      const char *c = name.c_str();
      outfile.open(c);
    } else {  
      outfile.open("a.ewe");
    }
    outfile << tree->eweCompiler();
    outfile.close();
  }
}

string limitFileName(string file) {
  int posExt = file.find_last_of(".calc") - 4;
  if (posExt != -1) {
    file = file.substr(0, posExt);
  }  
  return file;
}

