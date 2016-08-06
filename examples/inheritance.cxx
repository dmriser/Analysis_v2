////////////////////////////////////////
/*
 David Riser, University of Connecticut
 
 July 17, 2016
 
 template.cxx -> Your info here.
 
 */
////////////////////////////////////////

// c++ includes
#include <iostream>
using namespace std;

// my includes from h22 library
#include "CommonTools.h"
#include "Corrections.h"
#include "h22Event.h"
#include "h22Option.h"
#include "h22Reader.h"
#include "pars.h"
#include "ParticleFilter.h"

// root includes
#include "TStopwatch.h"

class Analysis : public h22Reader
{
public:
  Analysis(TChain*);
  ~Analysis();

  // Returns Number of Events ( Just a test ) 
  int events();
};

Analysis::Analysis(TChain* t) : h22Reader(0)
{
  fchain = t;
  //  Init();
}

Analysis::~Analysis()
{
  //  fchain->Delete();
}

int Analysis::events()
{
  return fchain->GetEntries();
}

int main(int argc, char * argv[])
{
    
    // Setup Options
    h22Options opts;
    opts.set(argc,argv);
    int GSIM     = opts.args["MC"].arg;
    
    // Setup Reader
    h22Reader fReader(GSIM);
    for (auto it=opts.ifiles.begin(); it<opts.ifiles.end(); it++) { fReader.AddFile(*it); }

    Analysis tester( fReader.fchain );

    cout << tester.events() << endl;
    
    return 0;
}
