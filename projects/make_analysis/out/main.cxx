#include <iostream>
using namespace std;

#include "EventMonitor.h"
#include "EventMonitor.cxx"


int main(int argc, char * argv[]){

        EventMonitor Analyzer;
        if (argc < 2) { return 0; }

        for (int ifile=1; ifile<argc; ifile++){ Analyzer.AddFile(argv[ifile]); } 
        Analyzer.Init(); 
        Analyzer.Loop(); 
        return 0; 
}
