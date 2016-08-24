#include <iostream>
using namespace std; 

#include "DBins.h"
#include "DEvent.h"
#include "DISPackage.h"
#include "Dumper.h"
#include "h22Reader.h"
#include "NathanArchive.h"

int main(int argc, char * argv[])
{
  if (argc < 2) exit(0); 

  // GSIM should set itself 
  h22Reader reader; 
  for (int iarg=1; iarg<argc; iarg++) reader.AddFile( argv[iarg] );
  reader.Init();

  DISHistograms histos; 
  DBins xBins(50, 0.01, 1.1);
  DBins qqBins(50, 1, 5);
  DBins wBins(10, 1, 5);
  histos.set_bins(xBins, qqBins, wBins);
  histos.init();

  int runno = reader.runno();
  NathanEIDWrapper nathan; 
  nathan.set_info(runno,reader.GSIM);
  
  for (int ientry=0; ientry<reader.GetEntries(); ientry++) 
    {
      reader.GetEntry(ientry);
      if (runno != reader.runno()){ runno = reader.runno(); nathan.set_info(runno, reader.GSIM); } 
      DEvent event( reader.GetEvent() );
      int e_index = nathan.get_electron(event.tracks);
      
      // We got one! 
      if (e_index > -123)
	{
	  event.set_e_index(e_index);
	  histos.fill(event, reader.GSIM);
	}

      if (ientry%1000 == 0) { cout << "\r done " << ientry << flush; }
    }

  // Dump Histograms 
  HistogramDumper my_dump; 
  for (int s=0; s<7; s++)  { my_dump.dump(histos.h1_hits_x_by_qq[s][0]); } 

  histos.output_name = "fix.root"; 
  histos.save();
  
  return 0;
}
