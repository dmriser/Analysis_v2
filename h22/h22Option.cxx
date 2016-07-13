//////////////////////////////////////
/*
 
 h22Option.cxx
 
 */
//////////////////////////////////////

#ifndef H22OPTIONS_CXX
#define H22OPTIONS_CXX

// c++ includes
#include <cstdlib>
#include <fstream>
#include <map>
#include <iostream>
#include <set>
#include <vector>
using namespace std;

// my includes
#include "h22Option.h"

h22Options::h22Options()
{
    // Setup Options
    args["N"].arg  = 0;
    args["N"].type = 0;
    args["N"].name = "Max number of events to process";
    args["N"].help = "Max number of events to process";
    
    args["BEAM_E"].arg  = 5.498;
    args["BEAM_E"].type = 0;
    args["BEAM_E"].name = "Beam Energy";
    args["BEAM_E"].help = "Beam Energy";
 
    args["PRINT"].arg  = 100000;
    args["PRINT"].type = 0;
    args["PRINT"].name = "Print message every number of events";
    args["PRINT"].help = "Print message every number of events";
    
    args["MC"].arg  = 0;
    args["MC"].type = 0;
    args["MC"].name = "Monte Carlo Status";
    args["MC"].help = "Monte Carlo Status (0-False, 1-True)";
    
    args["VERBOSE"].arg  = 0;
    args["VERBOSE"].type = 0;
    args["VERBOSE"].name = "Verbosity Status";
    args["VERBOSE"].help = "Verbosity Status (0-False, 1-True)";
    
    args["EPARS"].args = "elec_default.dat";
    args["EPARS"].type = 1;
    args["EPARS"].name = "EID Parameters File";
    args["EPARS"].help = "EID Parameters File";
    
    args["HPARS"].args = "had_default.dat";
    args["HPARS"].type = 1;
    args["HPARS"].name = "HID Parameters File";
    args["HPARS"].help = "HID Parameters File";
    
    args["DATASET"].args = "e1f";
    args["DATASET"].type = 1;
    args["DATASET"].name = "Type of data";
    args["DATASET"].help = "Type of data - default e1f";
    
}

h22Options::~h22Options()
{
    // Nothing to destroy.
}

void h22Options::set(int argc, char*argv[])
{
    string arg;
    string com, com2;
    string opt;
    cout << endl;
    string comp;
    
    map<string, opts>::iterator itm;
    
    
    // If Maps option are specified, overriding default
    for(int i=1; i<argc; i++)
    {
        arg = argv[i];
        map<string, opts>::iterator itm;
        for(itm = args.begin(); itm != args.end(); itm++)
        {
            com = "-" + itm->first + "=";
            comp.assign(arg, 0, arg.find("=") + 1);
            if(comp == com)
            {
                opt.assign(arg, com.size(), arg.size()-com.size());
                itm->second.args = opt;
                itm->second.arg  = atof(opt.c_str());
                cout <<  " >>> Options: " << itm->second.name << " set to: " ;
                if(itm->second.type) cout << itm->second.args;
                else cout << itm->second.arg  ;
                cout << endl;
            }
        }
    }
    
    // If argument is a file, adding it to the list of files
    for(int i=1; i<argc; i++)
    {
        ifstream isFile(argv[i], ios::in | ios::binary);
        if(isFile) ifiles.push_back(argv[i]);
    }
    
    cout << endl;
    
    for(unsigned int i=0; i<ifiles.size(); i++)
        cout << " Input file found: " << ifiles[i] << endl;
    
    cout << endl;
}

#endif
