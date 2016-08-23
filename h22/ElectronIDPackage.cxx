/////////////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  August 17, 2016 

  ElectronIDPackage.cxx 


  --> ElectronIDManager 
  --> ElectronIDHistograms 
  --> ElectronSelector 
  

*/
/////////////////////////////////////////////////////////

#ifndef electronid_package_cxx
#define electronid_package_cxx

// c++ includes
#include <iostream>
using namespace std;

// my includes
#include "CommonTools.h"
#include "Corrections.h"
#include "DBins.h"
#include "DCut.h"
#include "DEvent.h"
#include "DSelection.h"
#include "ElectronIDPackage.h"
#include "h22Event.h"
#include "h22Reader.h"

// root includes
#include "TF1.h"
#include "TGraphErrors.h"
#include "TH1.h"
#include "TH2.h"
#include "TVector3.h"

/////////////////////////////////////////////////////////
/*

  ElectronIDManager 
  
*/
/////////////////////////////////////////////////////////

ElectronIDManager::ElectronIDManager() : h22Reader(0)
{
  // Initialize Everything
  GSIM = 0;
  parfile = "";
}

ElectronIDManager::~ElectronIDManager()
{

}

void ElectronIDManager::init()
{
  int starting_runno = runno();
  if (starting_runno < 37000 || starting_runno > 39000) { GSIM = 1; }
  Init();

  selector.set_info(starting_runno, GSIM);
  selector.set_parfile(parfile);
}

void ElectronIDManager::calculate_values()
{
  /** For this cut to work it's better to use some basic identification before 
      applying the gaussian fits in each bin.  Being used below is the standard 
      electron selector, but with most cuts disabled. 

   */
  
  //  selector.enable_all();
  selector.disable_by_regex("Samp Frac");
  
  // Physical Parameters
  double P_MIN  = 0.5;
  double P_MAX  = 2.80;
  int P_BINS    = 100;
  const int N_SLICES = P_BINS;
  double P_STEP = (P_MAX-P_MIN)/(N_SLICES-1);

  TH1D * h1_slice[6][N_SLICES];
  TH2F * h2_ec_sampling[6];

  TF1 * f_gauss = new TF1("f_gauss","gaus",0.25,0.45);
  TF1 * f_mean_pol3[6];
  TF1 * f_sigma_pol3[6];

  TGraphErrors * g_mean[6];
  TGraphErrors * g_sigma[6];

  double mean[N_SLICES];
  double sigma[N_SLICES];
  double mean_e[N_SLICES];
  double sigma_e[N_SLICES];
  double x[N_SLICES];
  double dx[N_SLICES];

  // For passing to TGraph constructor
  for (int b=0; b<N_SLICES; b++) {x[b] = (double) b*P_STEP + P_MIN; dx[b] = 0.0;}

  for (int s=0; s<6; s++)
    {
      h2_ec_sampling[s] = new TH2F(Form("h2_ec_sampling_%d",s),Form("EC Sampling Fraction Sector %d",s+1),P_BINS,P_MIN,P_MAX,100,0.05,0.5);
    }

  int nev = GetEntries();

  cout << " Generating EC Sampling Fraction cut parameters for " << nev << " events... " << endl;
  for (int iev=0; iev<nev; iev++)
    {
      GetEntry(iev);
      DEvent event(GetEvent());
      if ( selector.passes(event, 0) ) { int sector = -1+event.tracks.dc_sect[0];  h2_ec_sampling[sector]->Fill(event.tracks.p[0],event.tracks.etot[0]/event.tracks.p[0]); }
    }

  // Do the fitting and slicing
  for (int s=0; s<6; s++)
    {
      for (int b=0; b<N_SLICES; b++)
	{
	  string name       = Form("h1_slice_%d_%d",s,b);
	  h1_slice[s][b]    = new TH1D(name.c_str(),name.c_str(),100,0.05,0.5);
	  h2_ec_sampling[s] ->ProjectionY(name.c_str(),b+1,b+2);
	  h1_slice[s][b]    ->Fit("f_gauss","RQ");

	  mean[b]    = f_gauss->GetParameter(1);
	  sigma[b]   = f_gauss->GetParameter(2);
	  mean_e[b]  = f_gauss->GetParError(1);
	  sigma_e[b] = f_gauss->GetParError(2);
	}

      f_mean_pol3[s]  = new TF1(Form("f_mean_pol3_%d",s),"pol3",0,N_SLICES);
      f_sigma_pol3[s] = new TF1(Form("f_sigma_pol3_%d",s),"pol3",0,N_SLICES);
      g_mean[s]       = new TGraphErrors(N_SLICES,x,mean,dx,mean_e);
      g_sigma[s]      = new TGraphErrors(N_SLICES,x,sigma,dx,sigma_e);
      g_mean[s]  ->Fit(Form("f_mean_pol3_%d",s),"RQ");
      g_sigma[s] ->Fit(Form("f_sigma_pol3_%d",s),"RQ");
    }

  cout << " Printing EC parameters to " << parfile << endl; 
  
  // Writing Parameters to File
  for (int s=0; s<6; s++)
    {
      pars.ECSSA[s] = f_sigma_pol3[s]->GetParameter(3);
      pars.ECSSB[s] = f_sigma_pol3[s]->GetParameter(2);
      pars.ECSSC[s] = f_sigma_pol3[s]->GetParameter(1);
      pars.ECSSD[s] = f_sigma_pol3[s]->GetParameter(0);

      pars.ECSMA[s] = f_mean_pol3[s]->GetParameter(3);
      pars.ECSMB[s] = f_mean_pol3[s]->GetParameter(2);
      pars.ECSMC[s] = f_mean_pol3[s]->GetParameter(1);
      pars.ECSMD[s] = f_mean_pol3[s]->GetParameter(0);

      pars.ECSNSIGMA[s] = 2.50;
    }
  
  pars.save(parfile);
  selector.summarize();
  
}

/////////////////////////////////////////////////////////
/*

  ElectronIDHistograms 
  
*/
/////////////////////////////////////////////////////////

ElectronIDHistograms::ElectronIDHistograms()
{
  cut[0] = "raw";
  cut[1] = "this";
  cut[2] = "all";

  output_name = "eid";
  parfile     = "epars.dat";
  
}

ElectronIDHistograms::ElectronIDHistograms(string out, string par)
{
  output_name = out; parfile = par;
  cut[0] = "raw";
  cut[1] = "this";
  cut[2] = "all";
}

ElectronIDHistograms::~ElectronIDHistograms()
{
  //  cout << " histograms destroyed " << endl; 
}

void ElectronIDHistograms::init()
{

  //! Initialize
  for (int c=0; c<3; c++)
    for (int s=0; s<7; s++)
      {
	h1_nphe[c][s]  = new TH1F(Form("h1_nphe_%s_%d",cut[c].c_str(),s),Form(" Number of Photoelectrons %s Sector %d",cut[c].c_str(),s),200,0,100);
	h1_vz[c][s]    = new TH1F(Form("h1_vz_%s_%d",cut[c].c_str(),s),Form(" Z-Vertex %s Sector %d",cut[c].c_str(),s),                  200,-35.0,-15.0);
	h1_ec_in[c][s] = new TH1F(Form("h1_ec_in_%s_%d",cut[c].c_str(),s),Form(" EC Inner Energy Dep. %s Sector %d",cut[c].c_str(),s),   200,0,2.0);
	h1_ecu[c][s]   = new TH1F(Form("h1_ecu_%s_%d",cut[c].c_str(),s),Form(" EC-U Coord. %s Sector %d",cut[c].c_str(),s),200,-500,500);
	h1_ecv[c][s]   = new TH1F(Form("h1_ecv_%s_%d",cut[c].c_str(),s),Form(" EC-V Coord. %s Sector %d",cut[c].c_str(),s),200,-500,500);
	h1_ecw[c][s]   = new TH1F(Form("h1_ecw_%s_%d",cut[c].c_str(),s),Form(" EC-W Coord. %s Sector %d",cut[c].c_str(),s),200,-500,500);
	
	h2_ec_sampling[c][s] = new TH2F(Form("h2_ec_sampling_%s_%d",cut[c].c_str(),s),Form(" EC Sampling %s Sector %d ",cut[c].c_str(),s),   100,0.5,4.0,100,0.05,0.5);
	h2_ec_uvw[c][s]      = new TH2F(Form("h2_ec_uvw_%s_%d",cut[c].c_str(),s),Form(" EC UVW %s Sector %d ",cut[c].c_str(),s),             100,-450,450,100,-450,450);
	h2_dcr1[c][s]        = new TH2F(Form("h2_dcr1_%s_%d",cut[c].c_str(),s),Form(" DC Region 1 Fid. %s Sector %d ",cut[c].c_str(),s),     100,15,60,100,-50,50);
	h2_dcr3[c][s]        = new TH2F(Form("h2_dcr3_%s_%d",cut[c].c_str(),s),Form(" DC Region 3 Fid. %s Sector %d ",cut[c].c_str(),s),     100,20,500,100,-500,500);
	h2_cc[c][s]          = new TH2F(Form("h2_cc_%s_%d",cut[c].c_str(),s),Form(" Cherenkov Counter Fid. %s Sector %d ",cut[c].c_str(),s), 100,-30,30,100,10,60);
      }  

  // Setting up ElectronSelector 
  selector.set_parfile(parfile);
  selector.set_info(runno, mc_status);
  
}

void ElectronIDHistograms::fill(DEvent event)
{
  selector.set_info(runno, mc_status);

  
  for (int ipart=0; ipart<event.tracks.gpart; ipart++)
    {
      
      if (selector.qc_cut->passes(event, ipart))
	{
	  int s        = event.tracks.dc_sect[ipart]; 
	  TVector3 uvw = event.tracks.uvw(ipart);
	  
	  h1_nphe[0][s]  ->Fill(event.tracks.nphe[ipart]/10);
	  h1_vz[0][s]    ->Fill(corr.vz(event.tracks, ipart, runno, mc_status)); // This needs to be the correced vz, but I dont want to include corrections class in here 
	  h1_ec_in[0][s] ->Fill(event.tracks.ec_ei[ipart]);
	  h1_ecu[0][s]   ->Fill(uvw.X());
	  h1_ecv[0][s]   ->Fill(uvw.Y());
	  h1_ecw[0][s]   ->Fill(uvw.Z());
	  
	  h2_ec_sampling[0][s] ->Fill(event.tracks.p[ipart], event.tracks.etot[ipart]/event.tracks.p[ipart]);
	  h2_ec_uvw[0][s]      ->Fill(event.tracks.ech_x[ipart], event.tracks.ech_y[ipart]);
	  h2_dcr1[0][s]        ->Fill(event.tracks.rot_dc1x(ipart), event.tracks.rot_dc1y(ipart));
	  h2_dcr3[0][s]        ->Fill(event.tracks.tl3_x[ipart], event.tracks.tl3_y[ipart]);
	  h2_cc[0][s]          ->Fill(event.tracks.rphi(ipart), event.tracks.theta_cc(ipart));
	  
	  h1_nphe[0][0]  ->Fill(event.tracks.nphe[ipart]/10);
	  h1_vz[0][0]    ->Fill(corr.vz(event.tracks, ipart, runno, mc_status)); // This needs to be the correced vz, but I dont want to include corrections class in here 
	  h1_ec_in[0][0] ->Fill(event.tracks.ec_ei[ipart]);
	  h1_ecu[0][0]   ->Fill(uvw.X());
	  h1_ecv[0][0]   ->Fill(uvw.Y());
	  h1_ecw[0][0]   ->Fill(uvw.Z());
	  
	  h2_ec_sampling[0][0] ->Fill(event.tracks.p[ipart], event.tracks.etot[ipart]/event.tracks.p[ipart]);
	  h2_ec_uvw[0][0]      ->Fill(event.tracks.ech_x[ipart], event.tracks.ech_y[ipart]);
	  h2_dcr1[0][0]        ->Fill(event.tracks.rot_dc1x(ipart), event.tracks.rot_dc1y(ipart));
	  h2_dcr3[0][0]        ->Fill(event.tracks.tl3_x[ipart], event.tracks.tl3_y[ipart]);
	  h2_cc[0][0]          ->Fill(event.tracks.rphi(ipart), event.tracks.theta_cc(ipart));
	}
    }


     	
  for (int ipart=0; ipart<event.tracks.gpart; ipart++) {
    if (selector.qc_cut->passes(event, ipart) && selector.negativity_cut->passes(event, ipart))
      {
	int s        = event.tracks.dc_sect[ipart]; 
	TVector3 uvw = event.tracks.uvw(ipart);
	
	if (selector.cc_fid_cut->passes(event, ipart))
	  {
	    h2_cc[1][0] ->Fill(event.tracks.rphi(ipart), event.tracks.theta_cc(ipart));
	    h2_cc[1][s] ->Fill(event.tracks.rphi(ipart), event.tracks.theta_cc(ipart));
	  }
	
	if (selector.dcr1_fid_cut->passes(event, ipart))
	  {
	    h2_dcr1[1][0] ->Fill(event.tracks.rot_dc1x(ipart), event.tracks.rot_dc1y(ipart));
	    h2_dcr1[1][s] ->Fill(event.tracks.rot_dc1x(ipart), event.tracks.rot_dc1y(ipart));
	  }
	
	if (selector.dcr3_fid_cut->passes(event, ipart))
	  {
	    h2_dcr3[1][0] ->Fill(event.tracks.tl3_x[ipart], event.tracks.tl3_y[ipart]);
	    h2_dcr3[1][s] ->Fill(event.tracks.tl3_x[ipart], event.tracks.tl3_y[ipart]);
	  }
	
	if (selector.edep_cut->passes(event, ipart))
	  {
	    h1_ec_in[1][0] ->Fill(event.tracks.ec_ei[ipart]);
	    h1_ec_in[1][s] ->Fill(event.tracks.ec_ei[ipart]);
	  }    
	
	if (selector.ecu_cut->passes(event, ipart))
	  {
	    h1_ecu[1][0] ->Fill(uvw.X());
	    h1_ecu[1][s] ->Fill(uvw.X());
	  }     
	
	if (selector.ecv_cut->passes(event, ipart))
	  {
	    h1_ecv[1][0] ->Fill(uvw.Y());
	    h1_ecv[1][s] ->Fill(uvw.Y());
	  }     
	
	if (selector.ecw_cut->passes(event, ipart))
	  {
	    h1_ecw[1][0] ->Fill(uvw.Z());
	    h1_ecw[1][s] ->Fill(uvw.Z());
	  }     
	
	if (selector.ecu_cut->passes(event, ipart) && selector.ecv_cut->passes(event, ipart) && selector.ecw_cut->passes(event, ipart))
	  {
	    h2_ec_uvw[1][0] ->Fill(event.tracks.ech_x[ipart], event.tracks.ech_y[ipart]);
	    h2_ec_uvw[1][s] ->Fill(event.tracks.ech_x[ipart], event.tracks.ech_y[ipart]);
	  }
	      
	if (selector.nphe_cut->passes(event, ipart))
	  {
	    h1_nphe[1][0] ->Fill(event.tracks.nphe[ipart]/10);
	    h1_nphe[1][s] ->Fill(event.tracks.nphe[ipart]/10);
	  }    
	
	if (selector.sf_s1_cut->applies(event, ipart) && selector.sf_s1_cut->passes(event, ipart))
	  {
	    h2_ec_sampling[1][0] ->Fill(event.tracks.p[ipart],event.tracks.etot[ipart]/event.tracks.p[ipart]);
		  h2_ec_sampling[1][1] ->Fill(event.tracks.p[ipart],event.tracks.etot[ipart]/event.tracks.p[ipart]);
	  }
	
	if (selector.sf_s2_cut->applies(event, ipart) && selector.sf_s2_cut->passes(event, ipart))
	  {
	    h2_ec_sampling[1][0] ->Fill(event.tracks.p[ipart],event.tracks.etot[ipart]/event.tracks.p[ipart]);
	    h2_ec_sampling[1][2] ->Fill(event.tracks.p[ipart],event.tracks.etot[ipart]/event.tracks.p[ipart]);
	  }
	
	if (selector.sf_s3_cut->applies(event, ipart) && selector.sf_s3_cut->passes(event, ipart))
	  {
	    h2_ec_sampling[1][0] ->Fill(event.tracks.p[ipart],event.tracks.etot[ipart]/event.tracks.p[ipart]);
	    h2_ec_sampling[1][3] ->Fill(event.tracks.p[ipart],event.tracks.etot[ipart]/event.tracks.p[ipart]);
	  }
	
	if (selector.sf_s4_cut->applies(event, ipart) && selector.sf_s4_cut->passes(event, ipart))
	  {
	    h2_ec_sampling[1][0] ->Fill(event.tracks.p[ipart],event.tracks.etot[ipart]/event.tracks.p[ipart]);
	    h2_ec_sampling[1][4] ->Fill(event.tracks.p[ipart],event.tracks.etot[ipart]/event.tracks.p[ipart]);
	  }
	
	if (selector.sf_s5_cut->applies(event, ipart) && selector.sf_s5_cut->passes(event, ipart))
	  {
	    h2_ec_sampling[1][0] ->Fill(event.tracks.p[ipart],event.tracks.etot[ipart]/event.tracks.p[ipart]);
	    h2_ec_sampling[1][5] ->Fill(event.tracks.p[ipart],event.tracks.etot[ipart]/event.tracks.p[ipart]);
	  }
	
	if (selector.sf_s6_cut->applies(event, ipart) && selector.sf_s6_cut->passes(event, ipart))
	  {
	    h2_ec_sampling[1][0] ->Fill(event.tracks.p[ipart],event.tracks.etot[ipart]/event.tracks.p[ipart]);
	    h2_ec_sampling[1][6] ->Fill(event.tracks.p[ipart],event.tracks.etot[ipart]/event.tracks.p[ipart]);
	  }
	
	if (selector.vz_cut->passes(event, ipart))
	  {
	    h1_vz[1][0] ->Fill(corr.vz(event.tracks, ipart, runno, mc_status)); 
	    h1_vz[1][s] ->Fill(corr.vz(event.tracks, ipart, runno, mc_status)); 
	  }      
      } 
  }
  
      
  for (int ipart=0; ipart<event.tracks.gpart; ipart++) 
    {
      if (selector.passes(event, ipart))
	{
	  int s        = event.tracks.dc_sect[ipart]; 
	  TVector3 uvw = event.tracks.uvw(ipart);
	  
	  h1_nphe[2][s]  ->Fill(event.tracks.nphe[ipart]/10);
	  h1_vz[2][s]    ->Fill(corr.vz(event.tracks, ipart, runno, mc_status)); // This needs to be the correced vz, but I dont want to include corrections class in here 
	  h1_ec_in[2][s] ->Fill(event.tracks.ec_ei[ipart]);
	  h1_ecu[2][s]   ->Fill(uvw.X());
	  h1_ecv[2][s]   ->Fill(uvw.Y());
	  h1_ecw[2][s]   ->Fill(uvw.Z());
	  
	  h2_ec_sampling[2][s] ->Fill(event.tracks.p[ipart],event.tracks.etot[ipart]/event.tracks.p[ipart]);
	  h2_ec_uvw[2][s]      ->Fill(event.tracks.ech_x[ipart], event.tracks.ech_y[ipart]);
	  h2_dcr1[2][s]        ->Fill(event.tracks.rot_dc1x(ipart), event.tracks.rot_dc1y(ipart));
	  h2_dcr3[2][s]        ->Fill(event.tracks.tl3_x[ipart], event.tracks.tl3_y[ipart]);
	  h2_cc[2][s]          ->Fill(event.tracks.rphi(ipart), event.tracks.theta_cc(ipart));
	  
	  h1_nphe[2][0]  ->Fill(event.tracks.nphe[ipart]/10);
	  h1_vz[2][0]    ->Fill(corr.vz(event.tracks, ipart, runno, mc_status)); // This needs to be the correced vz, but I dont want to include corrections class in here 
	  h1_ec_in[2][0] ->Fill(event.tracks.ec_ei[ipart]);
	  h1_ecu[2][0]   ->Fill(uvw.X());
	  h1_ecv[2][0]   ->Fill(uvw.Y());
	  h1_ecw[2][0]   ->Fill(uvw.Z());
	  
	  h2_ec_sampling[2][0] ->Fill(event.tracks.p[ipart], event.tracks.etot[ipart]/event.tracks.p[ipart]);
	  h2_ec_uvw[2][0]      ->Fill(event.tracks.ech_x[ipart], event.tracks.ech_y[ipart]);
	  h2_dcr1[2][0]        ->Fill(event.tracks.rot_dc1x(ipart), event.tracks.rot_dc1y(ipart));
	  h2_dcr3[2][0]        ->Fill(event.tracks.tl3_x[ipart], event.tracks.tl3_y[ipart]);
	  h2_cc[2][0]          ->Fill(event.tracks.rphi(ipart), event.tracks.theta_cc(ipart));
	}
    }
}



void ElectronIDHistograms::write_and_close()
{
  TFile f(Form("%s.root",output_name.c_str()),"recreate");
 
  //! Saving 
  for (int c=0; c<3; c++)
    for (int s=0; s<7; s++)
      {
	h1_nphe[c][s]  ->Write();
	h1_vz[c][s]    ->Write();
	h1_ec_in[c][s] ->Write();
	h1_ecu[c][s]   ->Write();
	h1_ecv[c][s]   ->Write();
	h1_ecw[c][s]   ->Write();
	
	h2_ec_sampling[c][s] ->Write();
	h2_ec_uvw[c][s]      ->Write();
	h2_dcr1[c][s]        ->Write();
	h2_dcr3[c][s]        ->Write();
	h2_cc[c][s]          ->Write();
      }

  f.Write();
  f.Close();
}

/////////////////////////////////////////////////////////
/*

  ElectronSelector : DSelection 
  
*/
/////////////////////////////////////////////////////////

ElectronSelector::ElectronSelector()
{
  // Initialize Everything 
  runno = 0; mc_status = false;
  set_name("Default ElectronID");
}

ElectronSelector::~ElectronSelector()
{
  //  cout << " selector destroyed " << endl; 
}

ElectronSelector::ElectronSelector(string file)
{
  runno = 0; mc_status = false;
  set_name("Default ElectronID");
  parameter_file = file;
  init();
}

bool ElectronSelector::passes(DEvent event, int index)
{
  // Loop over the cuts we have and make sure all pass.
  bool status = true;

  // Here we need to set the vertex correctly and do any other correction. This keeps the Corrections object out of DCut and DSelector base class.
  event.tracks.vz[index] = corr.vz(event.tracks,index,runno,mc_status); // corr is an instance of Corrections 
  if ( !corr.good_sc_paddle(event.tracks, index) ) { return false; }
  
  vector<DCut*>::iterator it;
  for (it = cuts.begin(); it!=cuts.end(); it++)
    {
      if ( (*it)->is_on() && (*it)->applies(event, index) && !(*it)->passes(event, index)) { status = false; }
    }

  return status;
}

void ElectronSelector::init()
{

  // Getting the numbers 
  pars.load( parameter_file );

  // Call cut constructors 
  negativity_cut = new ChargeCut();
  cc_fid_cut     = new CCFiducialCut();
  dcr1_fid_cut   = new DCR1FiducialCut();
  dcr3_fid_cut   = new DCR3FiducialCut();
  edep_cut       = new ECEdepInnerCut();
  ecu_cut        = new ECUCut();
  ecv_cut        = new ECVCut();
  ecw_cut        = new ECWCut();
  nphe_cut       = new NPheCut();
  sf_s1_cut      = new SampFracCut(1);
  sf_s2_cut      = new SampFracCut(2);
  sf_s3_cut      = new SampFracCut(3);
  sf_s4_cut      = new SampFracCut(4);
  sf_s5_cut      = new SampFracCut(5);
  sf_s6_cut      = new SampFracCut(6);
  qc_cut         = new TrackQualityCut();
  vz_cut         = new ZVertexCut();

  // Set limits on cuts from parameters 
  negativity_cut->set_min(-1.1);
  negativity_cut->set_max(-0.9);

  cc_fid_cut->a = pars.CCFIDA;
  cc_fid_cut->b = pars.CCFIDB; 

  dcr1_fid_cut->height = pars.DCR1FIDH;
  dcr1_fid_cut->angle  = pars.DCR1FIDA; 
  dcr3_fid_cut->height = pars.DCR3FIDH;
  dcr3_fid_cut->angle  = pars.DCR3FIDA; 

  edep_cut->set_min( pars.ECEDEPMIN );
  ecu_cut->set_min( pars.ECUMIN[0] );
  ecu_cut->set_max( pars.ECUMAX[0] ); 
  ecv_cut->set_min( pars.ECVMIN[0] );
  ecv_cut->set_max( pars.ECVMAX[0] ); 
  ecw_cut->set_min( pars.ECWMIN[0] );
  ecw_cut->set_max( pars.ECWMAX[0] ); 

  nphe_cut->set_min( pars.CCNPHE );
  
  sf_s1_cut->am     = pars.ECSMA[0];
  sf_s1_cut->bm     = pars.ECSMB[0]; 
  sf_s1_cut->cm     = pars.ECSMC[0];
  sf_s1_cut->dm     = pars.ECSMD[0]; 
  sf_s1_cut->as     = pars.ECSSA[0];
  sf_s1_cut->bs     = pars.ECSSB[0];
  sf_s1_cut->cs     = pars.ECSSC[0];
  sf_s1_cut->ds     = pars.ECSSD[0];
  sf_s1_cut->nsigma = pars.ECSNSIGMA[0];

  
  sf_s2_cut->am     = pars.ECSMA[1];
  sf_s2_cut->bm     = pars.ECSMB[1]; 
  sf_s2_cut->cm     = pars.ECSMC[1];
  sf_s2_cut->dm     = pars.ECSMD[1]; 
  sf_s2_cut->as     = pars.ECSSA[1];
  sf_s2_cut->bs     = pars.ECSSB[1];
  sf_s2_cut->cs     = pars.ECSSC[1];
  sf_s2_cut->ds     = pars.ECSSD[1];
  sf_s2_cut->nsigma = pars.ECSNSIGMA[1];

  sf_s3_cut->am     = pars.ECSMA[2];
  sf_s3_cut->bm     = pars.ECSMB[2]; 
  sf_s3_cut->cm     = pars.ECSMC[2];
  sf_s3_cut->dm     = pars.ECSMD[2]; 
  sf_s3_cut->as     = pars.ECSSA[2];
  sf_s3_cut->bs     = pars.ECSSB[2];
  sf_s3_cut->cs     = pars.ECSSC[2];
  sf_s3_cut->ds     = pars.ECSSD[2];
  sf_s3_cut->nsigma = pars.ECSNSIGMA[2];

  sf_s4_cut->am     = pars.ECSMA[3];
  sf_s4_cut->bm     = pars.ECSMB[3]; 
  sf_s4_cut->cm     = pars.ECSMC[3];
  sf_s4_cut->dm     = pars.ECSMD[3]; 
  sf_s4_cut->as     = pars.ECSSA[3];
  sf_s4_cut->bs     = pars.ECSSB[3];
  sf_s4_cut->cs     = pars.ECSSC[3];
  sf_s4_cut->ds     = pars.ECSSD[3];
  sf_s4_cut->nsigma = pars.ECSNSIGMA[3];

  sf_s5_cut->am     = pars.ECSMA[4];
  sf_s5_cut->bm     = pars.ECSMB[4]; 
  sf_s5_cut->cm     = pars.ECSMC[4];
  sf_s5_cut->dm     = pars.ECSMD[4]; 
  sf_s5_cut->as     = pars.ECSSA[4];
  sf_s5_cut->bs     = pars.ECSSB[4];
  sf_s5_cut->cs     = pars.ECSSC[4];
  sf_s5_cut->ds     = pars.ECSSD[4];
  sf_s5_cut->nsigma = pars.ECSNSIGMA[4];

  sf_s6_cut->am     = pars.ECSMA[5];
  sf_s6_cut->bm     = pars.ECSMB[5]; 
  sf_s6_cut->cm     = pars.ECSMC[5];
  sf_s6_cut->dm     = pars.ECSMD[5]; 
  sf_s6_cut->as     = pars.ECSSA[5];
  sf_s6_cut->bs     = pars.ECSSB[5];
  sf_s6_cut->cs     = pars.ECSSC[5];
  sf_s6_cut->ds     = pars.ECSSD[5];
  sf_s6_cut->nsigma = pars.ECSNSIGMA[5];

  vz_cut->set_min( pars.VZMIN );
  vz_cut->set_max( pars.VZMAX );

  // Push back the cuts 
  add_cut( negativity_cut );
  add_cut( cc_fid_cut );
  add_cut( dcr1_fid_cut );
  add_cut( dcr3_fid_cut );
  add_cut( edep_cut );
  add_cut( ecu_cut );
  add_cut( ecv_cut );
  add_cut( ecw_cut );
  add_cut( nphe_cut );
  add_cut( sf_s1_cut );
  add_cut( sf_s2_cut );
  add_cut( sf_s3_cut );
  add_cut( sf_s4_cut );
  add_cut( sf_s5_cut );
  add_cut( sf_s6_cut );
  add_cut( qc_cut );
  add_cut( vz_cut );
  
}


#endif 
