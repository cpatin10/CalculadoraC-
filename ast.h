#pragma once

#include <string>
#include <vector>
#include <sstream>

using namespace std;

class AST {
 public:
   AST();
   virtual ~AST() = 0;
   virtual int evaluate() = 0;
   virtual string eweCompiler() = 0;
};

class BinaryNode : public AST {
 public:
   BinaryNode(AST* left, AST* right);
   ~BinaryNode();
   AST* getLeftSubTree() const;
   AST* getRightSubTree() const;
 private:
   AST* leftTree;
   AST* rightTree;
};

class UnaryNode : public AST {
 public:
   UnaryNode(AST* sub);
   ~UnaryNode();
   AST* getSubTree() const;
 private:
   AST* subTree;
};

class AddNode : public BinaryNode {
 public:
  AddNode(AST* left, AST* right);   
  int evaluate();
  string eweCompiler();
};

class SubNode : public BinaryNode {
 public:
  SubNode(AST* left, AST* right);
  int evaluate();
  string eweCompiler();
};

class TimesNode: public BinaryNode {
 public:
  TimesNode(AST* left, AST* right);
  int evaluate();
  string eweCompiler();
};

class DivideNode : public BinaryNode {
 public:
  DivideNode(AST* left, AST* right);
  int evaluate();
  string eweCompiler();
};

class ModNode : public BinaryNode {
 public:
  ModNode(AST* left, AST* right);
  int evaluate();
  string eweCompiler();
};

class NumNode : public AST {
 public:
   NumNode(int n);
   ~NumNode();
   int evaluate();
   string eweCompiler();
 private:
   int val;
};

class RecallNode : public AST {
 public:
  RecallNode();
  ~RecallNode();
  int evaluate();
  string eweCompiler();
};

class ClearNode : public AST {
 public:
  ClearNode();
  ~ClearNode();
  int evaluate();
  string eweCompiler();
};

class IdentNode : public AST {
 public:
  IdentNode(string s);
  ~IdentNode();
  int evaluate();
  string eweCompiler();
 private:
  string name;
};

class EmptyNode : public AST {
 public:
  EmptyNode();
  ~EmptyNode();
  int evaluate();
  string eweCompiler();
};

class ListNode : public AST {
 public:
  ListNode();
  ~ListNode();
  int evaluate();
  string eweCompiler();
  void pushNode(AST* node);
  AST* getLastNode();  
 private:
  vector<AST*> list;
};

class StoreNode : public UnaryNode {
 public:
  StoreNode(AST* sub);
  int evaluate();
  string eweCompiler();
};

class PlusNode : public UnaryNode {
 public:
  PlusNode(AST* sub);
  int evaluate();
  string eweCompiler();
};

class MinusNode : public UnaryNode {
 public:
  MinusNode(AST* sub);
  int evaluate();
  string eweCompiler();
};

class NegativeNode : public UnaryNode {
 public:
  NegativeNode(AST* sub);
  int evaluate();
  string eweCompiler();
};

class EqualsNode : public UnaryNode {
 public:
  EqualsNode(string i, AST* sub);
  int evaluate();
  string eweCompiler();
 private:
  string ident;
};

class InitNode : public UnaryNode {
 public:
  InitNode(AST* sub);
  int evaluate();
  string eweCompiler();
};

//for creating the compiler for ewe

string binaryOperations(char c, AST* left, AST* right);

string valOperation(string id);

string memOperation(char op, AST* sub);

string eweBegin();

string eweExprEnd();

string eweEnd();

string minussp();

string plussp();

string equ(string param, int &i);

string equMap(int &count);

string mapDeclaration();
