#include <iostream>
using namespace std;

#include "MCDumper.h"
#include "MCDumper.cxx"


int main(int argc, char * argv[]){

        MCDumper Analyzer;
        if (argc < 2) { return 0; }

        for (int ifile=1; ifile<argc; ifile++){ Analyzer.AddFile(argv[ifile]); } 
        Analyzer.Init(); 
        Analyzer.Loop(); 
        return 0; 
}
