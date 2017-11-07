#include <iostream>
#include <sstream>
#include <string>
#include <unistd.h>
#include <vector>
#include "calcex.h"
#include "calculator.h"

using namespace std;

Calculator* calc;

int main(int argc, char* argv[]) {
  string line;
 
  try {

    calc = new Calculator();
    
    int c;
    bool cflag = false;
    int countArg = 1;
    
    //getopt() for c, https://www.youtube.com/watch?v=0PrXd9Wo6jA    
    while ((c = getopt(argc, argv, "v:c")) != -1){

      switch(c) {
	  
      case 'v': {
	countArg +=2;
	string assigment(optarg);
	int posEq = assigment.find_first_of('=');	
	if (posEq != -1){
	  string identifier(assigment.substr(0, posEq));
	  string svalue(assigment.substr(posEq + 1));
	  istringstream buffer(svalue);  //convert str toint, http://www.cplusplus.com/forum/general/13135/
	  int value;
	  buffer >> value;
	  calc->storeInitIdent(value, identifier);
	}
	else {
	  throw CommandError;
	}	
	break;
      }
	
      case 'c':
	countArg++;	
	if (!cflag){
	  calc->setEweComp(true);
	  cflag = true;
	} else {
	  cerr << "command -c can only be used once" << endl;
	  throw CommandError;
	}	
	break;
	
      case '?':	
	if(optopt == 'v'){
	  throw CommandError;
	}
	throw CommandError;
	break;
	
      default:  
	throw CommandError;
      }
    }

    if (countArg < argc) {
      calc->setReadFile(true);
      for (int i = countArg; i < argc; i++){
	string file(argv[i]);
	calc->addFile(file);
      }
    }
    
    calc->eval();      
    delete calc;
    
  }
  catch(Exception ex) {
    cout << "*" << endl; 
  }
}
   
