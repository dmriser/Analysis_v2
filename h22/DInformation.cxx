///////////////////////////////////////////////
/*
  
  David Riser, University of Connecticut 
  August 7, 2016 

  DInformation.cxx -> Shamelessly naming 
  my classes after myself. 


*/
///////////////////////////////////////////////

#ifndef dinfo_cxx
#define dinfo_cxx

// c++ includes
#include <iostream>
#include <fstream>
#include <sstream>
#include <map>
using namespace std;

// my includes
#include "DInformation.h"


DInformation::DInformation()
{
  // Nothing to do. 
}

DInformation::~DInformation()
{
  // Nothing to do. 
}

void DInformation::load(string db_file)
{
  ifstream file( db_file.c_str() );

  if (file)
    {
      string line;
      while ( getline(file, line) )
	{
	  DInfo incoming_info;
	  int i = 0;
	  istringstream incoming_data(line);	  
	  while ( !incoming_data.eof())
	    {
	      string buffer;
	      incoming_data >> buffer;

	      switch(i)
		{
		case 0: 
		  incoming_info.runno = atoi(buffer.c_str());
		case 1:
		  incoming_info.dQ = atoi(buffer.c_str());
		case 2:
		  incoming_info.dN = atoi(buffer.c_str());
		}

	      i++;
	    }
	  
	  info[incoming_info.runno].runno = incoming_info.runno; 
	  info[incoming_info.runno].dQ = incoming_info.dQ;
	  info[incoming_info.runno].dN = incoming_info.dN; 

	}
    }
}

bool DInformation::has_information()
{
  if (info.size() > 0) return true;
  return false; 
}

#endif
