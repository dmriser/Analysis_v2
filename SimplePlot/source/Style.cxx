#ifndef style_cxx
#define style_cxx

#include <iostream>
#include <vector>
using std::string; 
using std::vector;
using std::cout; 
using std::endl; 

#include <fstream>
#include <cstdlib>

#include "Style.h"
#include "Tools.h"

namespace SimplePlot{

  Style::Style(){
    primaryColor   = 99; 
    secondaryColor = 65; 
    fontIndex      = 42; 
    gridx          = false; 
    gridy          = false; 
    fillStatus     = true; 
    alpha          = 0.2;
    name           = "default";
  }

  Style::~Style(){

  }

  void Style::saveStyle(string filename){
    ofstream outputFile;
    outputFile.open(filename.c_str());

    outputFile.width(10); outputFile << name; 
    outputFile.width(10); outputFile << primaryColor; 
    outputFile.width(10); outputFile << secondaryColor;  
    outputFile.width(10); outputFile << fontIndex; 
    outputFile.width(10); outputFile << alpha; 
    outputFile.width(10); outputFile << gridx; 
    outputFile.width(10); outputFile << gridy; 
    outputFile.width(10); outputFile << fillStatus << std::endl; 

    outputFile.close();
  }

  bool Style::loadStyle(string filename){
    ifstream input;
    input.open(filename.c_str());

    if(input.is_open()){
      string line; 
      int tokenIndex = 0; 
      
      while( !input.eof() ){
	input >> line; 
	
	// This feels like a caveman way to do things.
	if (tokenIndex == 0)      { this->name           = line;       }
	else if (tokenIndex == 1) { this->primaryColor   = atoi(line.c_str()); }
	else if (tokenIndex == 2) { this->secondaryColor = atoi(line.c_str()); }
	else if (tokenIndex == 3) { this->fontIndex      = atoi(line.c_str()); } 
	else if (tokenIndex == 4) { this->alpha          = atof(line.c_str()); }
	else if (tokenIndex == 5) { this->gridx          = atoi(line.c_str()); }
	else if (tokenIndex == 6) { this->gridy          = atoi(line.c_str()); }
	else if (tokenIndex == 7) { this->fillStatus     = atoi(line.c_str()); }
	else {
	  cout << "[Style::loadStyle] Token index not found " << tokenIndex << " for line " << line << endl; 
	}
	
	tokenIndex++; 
      }
      
    } else {
      cout << "[Style::loadStyle] Style not found " << filename << endl; 
    }

    input.close();
  }

}

#endif
