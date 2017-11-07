#pragma once

#include "ast.h"
#include "scanner.h"
#include <vector>

class Parser {
 public:
  Parser();
  Parser(string file);
  ~Parser();
  AST* parse();

  AST* CppCalc();
  AST* Lineas();  
  AST* Linea();
  AST* Expr();
  AST* RestExpr(AST* e);
  AST* Term();
  AST* RestTerm(AST* t);
  AST* Storable();
  AST* Negative();
  AST* MemOperation(AST* s);
  AST* Factor();

 private:
  Scanner* scan;
  bool readFile;
  void throwExcp(Token* t);
};

