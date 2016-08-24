////////////////////////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  August 24, 2016 

  DISPackage.cxx 

*/
////////////////////////////////////////////////////////////////////

#ifndef dis_cxx
#define dis_cxx

// c++ includes 
#include <iostream>
#include <vector>
using namespace std; 

// my includes
#include "DBins.h"
#include "DEvent.h"
#include "DISPackage.h"

//  root includes
#include "TH1.h"

////////////////////////////////////////////////////////////////////////
/*

  DISHistograms 

*/
////////////////////////////////////////////////////////////////////////

DISHistograms::DISHistograms()
{
  sect[0] = "all";
  sect[1] = "s1";
  sect[2] = "s2";
  sect[3] = "s3";
  sect[4] = "s4";
  sect[5] = "s5";
  sect[6] = "s6";
  output_name = "unset";
}


DISHistograms::~DISHistograms()
{
  // Destroy things. 
}

void DISHistograms::init()
{
  // Initialize Histograms

  // Generated Events for x-by-qq
  for (int s=0; s<7; s++) {
    vector<TH1F*> v;
    for (int b=0; b<(xBins.number()+1); b++)
      {
	string name  = Form("h1_gen_x_by_qq_%d_%s",b,sect[s].c_str());
	string title = Form("gen x-by-qq for bin %d of %s",b,sect[s].c_str());
	v.push_back( new TH1F(name.c_str(), title.c_str(), xBins.number(), xBins.min(), xBins.max()) );
      }
    h1_gen_x_by_qq.push_back(v);
  }

  // Reconstructed Events for x-by-qq
  for (int s=0; s<7; s++) {
    vector<TH1F*> v;
    for (int b=0; b<(xBins.number()+1); b++)
      {
	string name  = Form("h1_rec_x_by_qq_%d_%s",b,sect[s].c_str());
	string title = Form("rec x-by-qq for bin %d of %s",b,sect[s].c_str());
	v.push_back( new TH1F(name.c_str(), title.c_str(), xBins.number(), xBins.min(), xBins.max()) );
      }
    h1_rec_x_by_qq.push_back(v);
  }

  // Data Events for x-by-qq
  for (int s=0; s<7; s++) {
    vector<TH1F*> v;
    for (int b=0; b<(xBins.number()+1); b++)
      {
	string name  = Form("h1_hits_x_by_qq_%d_%s",b,sect[s].c_str());
	string title = Form("hits x-by-qq for bin %d of %s",b,sect[s].c_str());
	v.push_back( new TH1F(name.c_str(), title.c_str(), xBins.number(), xBins.min(), xBins.max()) );
      }
    h1_hits_x_by_qq.push_back(v);
  }

  // Generated Events for x-by-w
  for (int s=0; s<7; s++) {
    vector<TH1F*> v;
    for (int b=0; b<(xBins.number()+1); b++)
      {
	string name  = Form("h1_gen_x_by_w_%d_%s",b,sect[s].c_str());
	string title = Form("gen x-by-w for bin %d of %s",b,sect[s].c_str());
	v.push_back( new TH1F(name.c_str(), title.c_str(), xBins.number(), xBins.min(), xBins.max()) );
      }
    h1_gen_x_by_w.push_back(v);
  }

  // Reconstructed Events for x-by-w
  for (int s=0; s<7; s++) {
    vector<TH1F*> v;
    for (int b=0; b<(xBins.number()+1); b++)
      {
	string name  = Form("h1_rec_x_by_w_%d_%s",b,sect[s].c_str());
	string title = Form("rec x-by-w for bin %d of %s",b,sect[s].c_str());
	v.push_back( new TH1F(name.c_str(), title.c_str(), xBins.number(), xBins.min(), xBins.max()) );
      }
    h1_rec_x_by_w.push_back(v);
  }

  // Data Events for x-by-w
  for (int s=0; s<7; s++) {
    vector<TH1F*> v;
    for (int b=0; b<(xBins.number()+1); b++)
      {
	string name  = Form("h1_hits_x_by_w_%d_%s",b,sect[s].c_str());
	string title = Form("hits x-by-w for bin %d of %s",b,sect[s].c_str());
	v.push_back( new TH1F(name.c_str(), title.c_str(), xBins.number(), xBins.min(), xBins.max()) );
      }
    h1_hits_x_by_w.push_back(v);
  }

  
  // Generated Events for qq-by-x
  for (int s=0; s<7; s++) {
    vector<TH1F*> v;
    for (int b=0; b<(qqBins.number()+1); b++)
      {
	string name  = Form("h1_gen_qq_by_x_%d_%s",b,sect[s].c_str());
	string title = Form("gen qq-by-x for bin %d of %s",b,sect[s].c_str());
	v.push_back( new TH1F(name.c_str(), title.c_str(), qqBins.number(), qqBins.min(), qqBins.max()) );
      }
    h1_gen_qq_by_x.push_back(v);
  }

  // Reconstructed Events for x-by-w
  for (int s=0; s<7; s++) {
    vector<TH1F*> v;
    for (int b=0; b<(qqBins.number()+1); b++)
      {
	string name  = Form("h1_rec_qq_by_x_%d_%s",b,sect[s].c_str());
	string title = Form("rec qq-by-x for bin %d of %s",b,sect[s].c_str());
	v.push_back( new TH1F(name.c_str(), title.c_str(), qqBins.number(), qqBins.min(), qqBins.max()) );
      }
    h1_rec_qq_by_x.push_back(v);
  }

  // Data Events for qq-by-x
  for (int s=0; s<7; s++) {
    vector<TH1F*> v;
    for (int b=0; b<(qqBins.number()+1); b++)
      {
	string name  = Form("h1_hits_qq_by_x_%d_%s",b,sect[s].c_str());
	string title = Form("hits qq-by-x for bin %d of %s",b,sect[s].c_str());
	v.push_back( new TH1F(name.c_str(), title.c_str(), qqBins.number(), qqBins.min(), qqBins.max()) );
      }
    h1_hits_qq_by_x.push_back(v);
  }

  // Generated Events for qq-by-w
  for (int s=0; s<7; s++) {
    vector<TH1F*> v;
    for (int b=0; b<(qqBins.number()+1); b++)
      {
	string name  = Form("h1_gen_qq_by_w_%d_%s",b,sect[s].c_str());
	string title = Form("gen qq-by-w for bin %d of %s",b,sect[s].c_str());
	v.push_back( new TH1F(name.c_str(), title.c_str(), qqBins.number(), qqBins.min(), qqBins.max()) );
      }
    h1_gen_qq_by_w.push_back(v);
  }

  // Reconstructed Events for qq-by-w
  for (int s=0; s<7; s++) {
    vector<TH1F*> v;
    for (int b=0; b<(qqBins.number()+1); b++)
      {
	string name  = Form("h1_rec_qq_by_w_%d_%s",b,sect[s].c_str());
	string title = Form("rec qq-by-w for bin %d of %s",b,sect[s].c_str());
	v.push_back( new TH1F(name.c_str(), title.c_str(), qqBins.number(), qqBins.min(), qqBins.max()) );
      }
    h1_rec_qq_by_w.push_back(v);
  }

  // Data Events for qq-by-w
  for (int s=0; s<7; s++) {
    vector<TH1F*> v;
    for (int b=0; b<(qqBins.number()+1); b++)
      {
	string name  = Form("h1_hits_qq_by_w_%d_%s",b,sect[s].c_str());
	string title = Form("hits qq-by-w for bin %d of %s",b,sect[s].c_str());
	v.push_back( new TH1F(name.c_str(), title.c_str(), qqBins.number(), qqBins.min(), qqBins.max()) );
      }
    h1_hits_qq_by_w.push_back(v);
  }

 
}

void DISHistograms::fill(DEvent event, int index)
{
  if (event.e_index == -123){ cout << " You must set DEvent::e_index before filling histograms! " << endl; return; }
  
  int s = event.tracks.dc_sect[event.e_index];
  int xbin  = 1+xBins .find_bin( event.x );
  int qqbin = 1+qqBins.find_bin( event.qq);
  int wbin  = 1+wBins .find_bin( event.w );

  // Monte Carlo 
  if (index == 1) {
    h1_rec_x_by_qq[0][0]     ->Fill(event.x);
    h1_rec_x_by_qq[s][0]     ->Fill(event.x);
    h1_rec_x_by_qq[s][qqbin] ->Fill(event.x);

    h1_rec_x_by_w[0][0]    ->Fill(event.x);
    h1_rec_x_by_w[s][0]    ->Fill(event.x);
    h1_rec_x_by_w[s][wbin] ->Fill(event.x);

    h1_rec_qq_by_x[0][0]    ->Fill(event.qq);
    h1_rec_qq_by_x[s][0]    ->Fill(event.qq);
    h1_rec_qq_by_x[s][xbin] ->Fill(event.qq);
    
    h1_rec_qq_by_w[0][0]    ->Fill(event.qq);
    h1_rec_qq_by_w[s][0]    ->Fill(event.qq);
    h1_rec_qq_by_w[s][wbin] ->Fill(event.qq);

  }

  // Data Events
  else if (index == 0) {
    h1_hits_x_by_qq[0][0]     ->Fill(event.x);
    h1_hits_x_by_qq[s][0]     ->Fill(event.x);
    h1_hits_x_by_qq[s][qqbin] ->Fill(event.x);

    h1_hits_x_by_w[0][0]    ->Fill(event.x);
    h1_hits_x_by_w[s][0]    ->Fill(event.x);
    h1_hits_x_by_w[s][wbin] ->Fill(event.x);

    h1_hits_qq_by_x[0][0]    ->Fill(event.qq);
    h1_hits_qq_by_x[s][0]    ->Fill(event.qq);
    h1_hits_qq_by_x[s][xbin] ->Fill(event.qq);

    h1_hits_qq_by_w[0][0]    ->Fill(event.qq);
    h1_hits_qq_by_w[s][0]    ->Fill(event.qq);
    h1_hits_qq_by_w[s][wbin] ->Fill(event.qq);
  }

}

#endif
