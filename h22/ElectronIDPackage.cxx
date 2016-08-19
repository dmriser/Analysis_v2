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

  // Here we need to set the vertex correctly and do any other correction. 
  // event.tracks.vz[index] = corr.vz(event.tracks,index,runno,mc_status); // corr is an instance of Corrections 
  
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
  sf_s1_cut      = new SampFracCut(1);
  sf_s2_cut      = new SampFracCut(2);
  sf_s3_cut      = new SampFracCut(3);
  sf_s4_cut      = new SampFracCut(4);
  sf_s5_cut      = new SampFracCut(5);
  sf_s6_cut      = new SampFracCut(6);
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
  add_cut( sf_s1_cut );
  add_cut( sf_s2_cut );
  add_cut( sf_s3_cut );
  add_cut( sf_s4_cut );
  add_cut( sf_s5_cut );
  add_cut( sf_s6_cut );
  add_cut( vz_cut );
  
}


#endif 
