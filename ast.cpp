#include "ast.h"
#include <iostream>
#include <sstream>
#include <vector>
#include "calculator.h"

// for debug information uncomment
//#define debug

AST::AST() {}

AST::~AST() {}

BinaryNode::BinaryNode(AST* left, AST* right):
   AST(),
   leftTree(left),
   rightTree(right)
{}

BinaryNode::~BinaryNode() {
#ifdef debug
   cout << "In BinaryNode destructor" << endl;
#endif

   try {
      delete leftTree;
   } catch (...) {}

   try {
      delete rightTree;
   } catch(...) {}
}
   
AST* BinaryNode::getLeftSubTree() const {
   return leftTree;
}

AST* BinaryNode::getRightSubTree() const {
  return rightTree;
}

UnaryNode::UnaryNode(AST* sub):
   AST(),
   subTree(sub)
{}

UnaryNode::~UnaryNode() {
#ifdef debug
   cout << "In UnaryNode destructor" << endl;
#endif

   try {
      delete subTree;
   } catch (...) {}
}
   
AST* UnaryNode::getSubTree() const {
   return subTree;
}

AddNode::AddNode(AST* left, AST* right):
   BinaryNode(left,right)
{}

int AddNode::evaluate() {
  return getLeftSubTree()->evaluate() + getRightSubTree()->evaluate();
}

string AddNode::eweCompiler() {
  return binaryOperations('+', getLeftSubTree(), getRightSubTree());
}

SubNode::SubNode(AST* left, AST* right):
   BinaryNode(left,right)
{}

int SubNode::evaluate() {
   return getLeftSubTree()->evaluate() - getRightSubTree()->evaluate();
}

string SubNode::eweCompiler() {
  return binaryOperations('-', getLeftSubTree(), getRightSubTree());
}

TimesNode::TimesNode(AST* left, AST* right):
   BinaryNode(left,right)
{}

int TimesNode::evaluate() {
   return getLeftSubTree()->evaluate() * getRightSubTree()->evaluate();
}

string TimesNode::eweCompiler() {
  return binaryOperations('*', getLeftSubTree(), getRightSubTree());
}

DivideNode::DivideNode(AST* left, AST* right):
   BinaryNode(left,right)
{}

int DivideNode::evaluate() {
   return getLeftSubTree()->evaluate() / getRightSubTree()->evaluate();
}

string DivideNode::eweCompiler() {
  return binaryOperations('/', getLeftSubTree(), getRightSubTree());
}

ModNode::ModNode(AST* left, AST* right) :
  BinaryNode(left, right)
{}

int ModNode::evaluate() {
  return getLeftSubTree()->evaluate() % getRightSubTree()->evaluate();
}

string ModNode::eweCompiler() {
  return binaryOperations('%', getLeftSubTree(), getRightSubTree());
}

NumNode::NumNode(int n) :
   AST(),
   val(n)
{}

NumNode::~NumNode() {}

int NumNode::evaluate() {
  return val;
}

string NumNode::eweCompiler() {
  ostringstream num;
  num << val;
  return valOperation(num.str());
}

RecallNode::RecallNode() :
AST()
{}

RecallNode::~RecallNode () {}

int RecallNode::evaluate() {
  return calc->recall();
}

string RecallNode::eweCompiler() {  
  ostringstream sroot;
  sroot << "# Recall" << endl
	<< minussp()
	<< "  M[sp+0] := memory" << endl;
  return sroot.str();
}

ClearNode::ClearNode() :
  AST()
{}

ClearNode::~ClearNode() {}

int ClearNode::evaluate(){
  calc->store(0);
  return calc->recall();
}

string ClearNode::eweCompiler() {
  ostringstream sroot;
  sroot << "# Clear" << endl
	<< "  memory := zero" << endl
	<< minussp()
	<< "  M[sp+0] := memory" << endl;
  return sroot.str();  
}
  
IdentNode::IdentNode(string s) :
  AST(),
  name(s)
{}

IdentNode::~IdentNode() {}

int IdentNode::evaluate() {
  return calc->getValueIdent(name);
}

string IdentNode::eweCompiler() {
  return valOperation(name);
}

EmptyNode::EmptyNode() :
  AST()
{}

EmptyNode::~EmptyNode() {}

int EmptyNode::evaluate() {
  return 0;
}

string EmptyNode::eweCompiler() {
  return valOperation("0");
}

ListNode::ListNode() :
  AST()
{}

ListNode::~ListNode() {
  try {
    for (int i = 0; i < list.size(); ++i) {
      delete list[i];
    }
   } catch (...) {}
}

int ListNode::evaluate() {
  return 0;
}

string ListNode::eweCompiler() {
  ostringstream sroot;
  sroot << "start:" << endl
	<< "  one := 1" << endl
	<< "  zero := 0" << endl    
	<< "  memory := zero" << endl
	<< mapDeclaration();
  for (int i = 0; i < list.size(); ++i) {
    sroot << "expr" << i + 1 << ":" << endl
	  << list[i]->eweCompiler();
  }
  sroot << eweEnd();
  return sroot.str();
}

void ListNode::pushNode(AST* node) {
  list.push_back(node);
}

AST* ListNode::getLastNode() {
  return list.back();
}

StoreNode::StoreNode(AST* sub):
  UnaryNode(sub)
{}

int StoreNode::evaluate(){
  calc->store(getSubTree()->evaluate());  
  return calc->recall();
}

string StoreNode::eweCompiler() {
  ostringstream sroot;
  sroot << "# Store" << endl
  	<< "  memory := M[sp+0]" << endl;
  ostringstream stot;
  stot << getSubTree()->eweCompiler()
       << sroot.str();
  return stot.str();
}

PlusNode::PlusNode(AST* sub) :
  UnaryNode(sub)
{}

int PlusNode::evaluate() {
  //  calc->store(calc->recall() + getSubTree()->evaluate());
  calc->store(getSubTree()->evaluate() + calc->recall());
  return calc->recall();
}

string PlusNode::eweCompiler() {
  return memOperation('+', getSubTree());
}
  
MinusNode::MinusNode(AST* sub) :
  UnaryNode(sub)
{}

int MinusNode::evaluate(){
  //calc->store(calc->recall() - getSubTree()->evaluate());
  int r = getSubTree()->evaluate();
  calc->store(calc->recall() - r);
  return calc->recall();
}

string MinusNode::eweCompiler() {
  return memOperation('-', getSubTree());
}

NegativeNode::NegativeNode(AST* sub) :
  UnaryNode(sub)
{}

int NegativeNode::evaluate(){
  return -1 * getSubTree()->evaluate();
}

string NegativeNode::eweCompiler() {
  ostringstream sroot;
  sroot << "# Negate" << endl
	<< "  operator1 := zero - operator1" << endl
	<< "  M[sp+0] := operator1" << endl;
  ostringstream stot;
  stot << getSubTree()->eweCompiler()
       << sroot.str();
  return stot.str();
}

EqualsNode::EqualsNode(string i, AST* sub) :
  UnaryNode(sub),
  ident(i)
{}

int EqualsNode::evaluate(){  
  calc->storeIdent(getSubTree()->evaluate(), ident);
  return calc->getValueIdent(ident);
}

string EqualsNode::eweCompiler() {
  ostringstream sroot;
  sroot << "# Assign" << endl
	<< "  " << ident << " := M[sp+0]" << endl;
  ostringstream stot;
  stot << getSubTree()->eweCompiler()
       << sroot.str();
  return stot.str();  
}

InitNode::InitNode(AST* sub) :
  UnaryNode(sub)
{}

int InitNode::evaluate() {
  return getSubTree()->evaluate();
}

string InitNode::eweCompiler() {
  ostringstream sroot;
  sroot << eweBegin()
	<< getSubTree()->eweCompiler()
	<< eweExprEnd();
  return sroot.str();
}

string binaryOperations(char op, AST* left, AST* right) {
  string nameop;
  
  switch(op) {
    
  case '+':
    nameop = "Add";
    break;    
  case '-':
    nameop = "Sub";
    break;    
  case '*':
    nameop = "Times";
    break;    
  case '/':
    nameop = "Divide";
    break;
  case '%':
    nameop = "Module";
    break;
  }

  ostringstream sroot;
  sroot << "# " << nameop << endl
	<< "  operator2 := M[sp+0]" << endl
	<< "  operator1 := M[sp+1]" << endl
	<< "  operator1 := operator1 " << op << " operator2" << endl
	<< plussp()
	<< "  M[sp+0] := operator1" << endl;

  ostringstream stot;
  stot << left->eweCompiler()
       << right->eweCompiler()
       << sroot.str(); 
  return stot.str();
}

string valOperation(string id) {
  ostringstream sroot;  
  sroot << "# push(" << id << ")" << endl
	<< minussp()
	<< "  operator1 := " << id << endl
	<< "  M[sp+0] := operator1" << endl;  
  return sroot.str();
}

string memOperation(char op, AST* sub) {
  string nameop;
  ostringstream sroot;
  
  if (op == '+') {
    nameop = "Plus";
  } else {
    nameop = "Minus";
  }
  sroot << "# Memory " << nameop << endl
	<< "  operator1 := M[sp+0]" << endl
	<< "  memory := memory " << op << " operator1" << endl
	<< "  M[sp+0] := memory" << endl;

  ostringstream stot;
  stot << sub->eweCompiler()
       << sroot.str();
  return stot.str();  
}

string eweBegin() {
  ostringstream ss;
  ss << "# Instrucciones antes del recorrido del arbol abstracto sintactico" << endl
     << "  sp := 1000" << endl
     << "# Comienza el recorrido del arbol" << endl;
  return ss.str();
}

string eweExprEnd() {
  ostringstream ss;
  ss << "# Write Result" << endl
     << "  operator1 := M[sp+0]" << endl
     << minussp()
     << "  writeInt(operator1)" << endl;
  return ss.str();
}

string eweEnd() {
  ostringstream ss;
  ss << "end: halt" << endl;

  int count = 0;
  ss << equ("memory", count)
     << equ("one", count)
     << equ("zero", count)
     << equ("operator1", count)
     << equ("operator2", count)
     << equ("sp", count)
     << equMap(count);
  count = 1000;
  ss << equ("stack", count);

  return ss.str();
}    

string minussp() {
  return "  sp := sp - one\n";
}

string plussp() {
  return "  sp := sp + one\n";
}

string equ(string param, int &i) {
  ostringstream ss;
  ss << "equ " << param
     << " M[" << i << "]"
     << endl;
  i++;
  return ss.str();
}

string equMap(int &count) {
  ostringstream ss;
  vector<string> v = calc->getIdent(true);
  for (int i = 0; i < v.size(); ++i) {
    ss << equ(v[i], count);
  }
  return ss.str();
}

string mapDeclaration() {
  ostringstream ss;
  vector<string> v;
  v = calc->getIdent(false);
  for (int i = 0; i < v.size(); ++i) {
    ss << "  " << v[i] << " := "
       << calc->getValueInitIdent(v[i])
       << endl;
  }
  return ss.str();
} 
