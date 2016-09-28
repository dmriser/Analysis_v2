#include <iostream>
using namespace std;

#include "AppendInformationToh22.h"
#include "AppendInformationToh22.cxx"


int main(int argc, char * argv[]){

        AppendInformationToh22 Analyzer;
        if (argc < 2) { return 0; }

        for (int ifile=1; ifile<argc; ifile++){ Analyzer.AddFile(argv[ifile]); } 
        Analyzer.Init(); 
        Analyzer.Loop(); 
        return 0; 
}
