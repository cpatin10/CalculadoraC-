#include "parser.h"
#include "calcex.h"
#include <string>
#include <sstream>

Parser::Parser() :
  readFile(false)
{
  scan = new Scanner();
}

Parser::Parser(string file) :
  readFile(true)
{
  scan = new Scanner(file);
}

Parser::~Parser() {
   try {
      delete scan;
   } catch (...) {}
}

AST* Parser::parse() {
  return CppCalc();
}

AST* Parser::CppCalc() {
  return Lineas();
}

AST* Parser::Lineas() {
  ListNode* n = new ListNode();  
  Token* t = scan->getToken();
  
  while (t->getType() != eof) {
    try {
      scan->putBackToken();
      AST* in = new InitNode(Linea());
      n->pushNode(in);
      cout << (n->getLastNode())->evaluate() << endl;
      if (!readFile) {
	scan->setInStream();
      }
      t = scan->getToken();
    }  
    catch(...) {
      if (!readFile) {
	scan->setInStream();
      }
      else {
      	while (t->getType() != eol && t->getType() != eof) {
      	  t = scan->getToken();
      	}
      }
      t = scan->getToken();
    }
  }  
  AST* ast = n;
  return ast;
}


AST* Parser::Linea() {
  Token* t = scan->getToken();
  if (t->getType() != eol){
    if (t->getType() == keyword && t->getLex() == "let"){
      t = scan->getToken();
      if (t->getType() == identifier) {
	string ident = t->getLex();
	t = scan->getToken();
	if (t->getType() == equals) {
	  cout << "= " << ident << "<-";
	  AST* en = new EqualsNode(ident, Expr());
	  t = scan->getToken();
	  if (t->getType() != eol) {
	    scan->putBackToken();
	  }
	  return en;
	}
      }
      throwExcp(t);
    }
    scan->putBackToken();
    AST* e = Expr();
    cout << "= ";
    t = scan->getToken();
    if (t->getType() != eol) {
      scan->putBackToken();
    }
    return e;
  }
  cout << "= ";
  return new EmptyNode();
}

AST* Parser::Expr() {
  return RestExpr(Term());
}

AST* Parser::RestExpr(AST* e) {
   Token* t = scan->getToken();

   if (t->getType() == add) {
     return RestExpr(new AddNode(e,Term()));
   }
   if (t->getType() == sub)
     return RestExpr(new SubNode(e,Term()));

   scan->putBackToken();

   return e;
}

AST* Parser::Term() {
  return RestTerm(Storable());
}

AST* Parser::RestTerm(AST* e) {
  Token* t = scan->getToken();
  
  if (t->getType() == times) {
    return RestTerm(new TimesNode(e, Storable()));//Term()));
  }
  if (t->getType() == divide) {
    return RestTerm(new DivideNode(e, Storable()));//Term()));
  }
  if (t->getType() == mod) {
    return RestTerm(new ModNode(e, Storable()));//Term()));
  }
  scan->putBackToken();
  return e;
}

AST* Parser::Storable() {
  return MemOperation(Negative());
}

AST* Parser::Negative(){
  Token* t = scan->getToken();
  if (t->getType() == sub) {
    return new NegativeNode(Factor());
  }
  scan->putBackToken();
  return Factor();
}

AST* Parser::MemOperation(AST* s){
  Token* t = scan->getToken();

  if (t->getType() == keyword){
    if(t->getLex() == "S"){
      return new StoreNode(s);
    }
    if (t->getLex() == "P"){
      return new PlusNode(s);
    }
    if (t->getLex() == "M"){
      return new MinusNode(s);
    }
  }
  scan->putBackToken();
  return s;
}

AST* Parser::Factor() {
  Token* t = scan->getToken();

  if (t->getType() == number){
    istringstream in(t->getLex());
    int val;
    in >> val;
    return new NumNode(val);
  }

  if (t->getType() == keyword) {
    if (t->getLex() == "R") {
      return new RecallNode();
    } else if (t->getLex() == "C") {
      return new ClearNode();
    } else {
      throwExcp(t);
    }
  }

  if (t->getType() == lparen){
    AST *res = Expr();
    t = scan->getToken();
    if (t->getType() == rparen){
      return res;
    }
    throwExcp(t);
  }
  
  if(t->getType() == identifier){
    return new IdentNode(t->getLex());
  }
  throwExcp(t);
}

void Parser::throwExcp(Token* t) {
      cerr <<  "* parse error line " << t->getLine() << " column " << t->getCol() << endl;
    throw ParseError;
}
