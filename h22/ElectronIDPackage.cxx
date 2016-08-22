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
#include "TH1.h"
#include "TH2.h"
#include "TVector3.h"

/////////////////////////////////////////////////////////
/*

  ElectronIDManager 
  
*/
/////////////////////////////////////////////////////////

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
  selector = ElectronSelector(parfile);
  selector.set_info(runno, mc_status);
  
}

void ElectronIDHistograms::fill(DEvent event, int c_index)
{
  // c index contains the cut index 0:None, 1:Passes this cut, 2:Passes all cuts 

  selector.set_info(runno, mc_status);
  
  switch( c_index )
    {
      // No Cuts ( all negative tracks )
    case 0:
      {
	
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

		h2_ec_sampling[0][s] ->Fill(event.tracks.etot[ipart]/event.tracks.p[ipart], event.tracks.p[ipart]);
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

		h2_ec_sampling[0][0] ->Fill(event.tracks.etot[ipart]/event.tracks.p[ipart], event.tracks.p[ipart]);
		h2_ec_uvw[0][0]      ->Fill(event.tracks.ech_x[ipart], event.tracks.ech_y[ipart]);
		h2_dcr1[0][0]        ->Fill(event.tracks.rot_dc1x(ipart), event.tracks.rot_dc1y(ipart));
		h2_dcr3[0][0]        ->Fill(event.tracks.tl3_x[ipart], event.tracks.tl3_y[ipart]);
		h2_cc[0][0]          ->Fill(event.tracks.rphi(ipart), event.tracks.theta_cc(ipart));
	    }
	  }
      }

      // Passes this cut 
    case 1:
      {
	
      }

      // ElectronSelector returns true for event 
    case 2:
      {
	
      }
      
    }
  
}

void ElectronIDHistograms::write_and_close()
{
  TFile f(output_name.c_str(),"recreate");
 
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
  sf_s1_cut->bs     = pars.ECSSA[0];
  sf_s1_cut->cs     = pars.ECSSA[0];
  sf_s1_cut->ds     = pars.ECSSA[0];
  sf_s1_cut->nsigma = pars.ECSNSIGMA[0];

  sf_s2_cut->am     = pars.ECSMA[1];
  sf_s2_cut->bm     = pars.ECSMB[1]; 
  sf_s2_cut->cm     = pars.ECSMC[1];
  sf_s2_cut->dm     = pars.ECSMD[1]; 
  sf_s2_cut->as     = pars.ECSSA[1];
  sf_s2_cut->bs     = pars.ECSSA[1];
  sf_s2_cut->cs     = pars.ECSSA[1];
  sf_s2_cut->ds     = pars.ECSSA[1];
  sf_s2_cut->nsigma = pars.ECSNSIGMA[1];

  sf_s3_cut->am     = pars.ECSMA[2];
  sf_s3_cut->bm     = pars.ECSMB[2]; 
  sf_s3_cut->cm     = pars.ECSMC[2];
  sf_s3_cut->dm     = pars.ECSMD[2]; 
  sf_s3_cut->as     = pars.ECSSA[2];
  sf_s3_cut->bs     = pars.ECSSA[2];
  sf_s3_cut->cs     = pars.ECSSA[2];
  sf_s3_cut->ds     = pars.ECSSA[2];
  sf_s3_cut->nsigma = pars.ECSNSIGMA[2];

  sf_s4_cut->am     = pars.ECSMA[3];
  sf_s4_cut->bm     = pars.ECSMB[3]; 
  sf_s4_cut->cm     = pars.ECSMC[3];
  sf_s4_cut->dm     = pars.ECSMD[3]; 
  sf_s4_cut->as     = pars.ECSSA[3];
  sf_s4_cut->bs     = pars.ECSSA[3];
  sf_s4_cut->cs     = pars.ECSSA[3];
  sf_s4_cut->ds     = pars.ECSSA[3];
  sf_s4_cut->nsigma = pars.ECSNSIGMA[3];

  sf_s5_cut->am     = pars.ECSMA[4];
  sf_s5_cut->bm     = pars.ECSMB[4]; 
  sf_s5_cut->cm     = pars.ECSMC[4];
  sf_s5_cut->dm     = pars.ECSMD[4]; 
  sf_s5_cut->as     = pars.ECSSA[4];
  sf_s5_cut->bs     = pars.ECSSA[4];
  sf_s5_cut->cs     = pars.ECSSA[4];
  sf_s5_cut->ds     = pars.ECSSA[4];
  sf_s5_cut->nsigma = pars.ECSNSIGMA[4];

  sf_s6_cut->am     = pars.ECSMA[5];
  sf_s6_cut->bm     = pars.ECSMB[5]; 
  sf_s6_cut->cm     = pars.ECSMC[5];
  sf_s6_cut->dm     = pars.ECSMD[5]; 
  sf_s6_cut->as     = pars.ECSSA[5];
  sf_s6_cut->bs     = pars.ECSSA[5];
  sf_s6_cut->cs     = pars.ECSSA[5];
  sf_s6_cut->ds     = pars.ECSSA[5];
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
