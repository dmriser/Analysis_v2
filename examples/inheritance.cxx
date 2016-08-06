////////////////////////////////////////
/*
 David Riser, University of Connecticut
 
 July 17, 2016
 
 template.cxx -> Your info here.
 
 */
////////////////////////////////////////

// c++ includes
#include <iostream>
#include <vector>
using namespace std;

// my includes from h22 library
#include "CommonTools.h"
#include "Corrections.h"
#include "ElasticPackage.h"
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
  Analysis();
  ~Analysis();

  ParticleFilter filter; 
  ElasticEvent elasticEvent; 

  // Returns Number of Events ( Just a test ) 
  void add_files(vector<string>);
  void init();
  void loop();
  int events();
};

Analysis::Analysis() : h22Reader(0)
{
  
}

Analysis::~Analysis()
{
  //  fchain->Delete();
}

void Analysis::add_files(vector<string> files)
{
  for (int ifile=0; ifile < files.size(); ifile++) AddFile(files[ifile]);
}

int Analysis::events()
{
  return fchain->GetEntries();
}

void Analysis::init()
{
  Init();
}

void Analysis::loop()
{

  // Event loop
  elasticEvent.set_w_max( 1.08 );
  
}

int main(int argc, char * argv[])
{
    
    // Setup Options
    h22Options opts;
    opts.set(argc,argv);

    
    
    Analysis tester;
    tester.add_files(opts.ifiles);
    
    cout << tester.events() << endl;
    
    return 0;
}
