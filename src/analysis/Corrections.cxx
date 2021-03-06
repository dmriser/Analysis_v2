////////////////////////////////////////////////
/*
 
 Corrections.cxx -> Class that holds the hard work
 other people did.  (Nathan H. & Marco)
 
 Contains:
 - Z-Vertex Corrections (Nathan@JLAB)
 - SC Time Corrections for Electron and Hadronic (Nathan@JLAB)
 - Electron Momentum Corrections (Marco@INFN)
 */
////////////////////////////////////////////////

#ifndef CORR_CXX
#define CORR_CXX

// c++ includes
#include <cmath>
#include <iostream>

// my includes
#include "CommonTools.h"
#include "Corrections.h"
#include "h22Event.h"

float vertexParameters::rotationVector[3][6] = {{ 1.0,  0.5, -0.5, -1.0, -0.5, 0.5},
						{ 0.0,  0.866025388, 0.866025388,
						  0.0, -0.866025388, -0.866025388},
						{ 0.0, 0.0, 0.0, 0.0, 0.0, 0.0}};
Corrections::Corrections(){
    // Nothing to do.
}

Corrections::~Corrections(){
    // Nothing to destroy.
}

void Corrections::correctEvent(h22Event *event, int runno, int GSIM){
  /*

    I am applying vertex corrections directly in the GetEntry method 
    of the h22Reader class.

   */

    if( !GSIM ){
      int electronIndex = event->GetElectronIndex();
      //      event->corr_vz[electronIndex]   = vz((*event), electronIndex, GSIM);
      event->corr_sc_t[electronIndex] = electron_sct((*event),electronIndex,runno,GSIM);

      //      double startTime = event->corr_sc_t[electronIndex] - event->sc_r[electronIndex]/speed_of_light; 
      double startTime = event->corr_sc_t[electronIndex] - event->sc_r[electronIndex]/(event->b[electronIndex]*speed_of_light);
      event->SetStartTime(startTime); 

      for(int ipart=0; ipart<event->gpart; ipart++){
	if (ipart != electronIndex){
	  //	  event->corr_vz[ipart] = vz((*event), ipart, GSIM);

	  double beta = event->sc_r[ipart]/(event->sc_t[ipart]-startTime)/speed_of_light;
	    
	  if(event->q[ipart] != 0){
	    event->corr_sc_t[ipart] = hadron_sct((*event), ipart, runno, GSIM);
	    beta = event->sc_r[ipart]/(event->corr_sc_t[ipart]-startTime)/speed_of_light;
	  }

	  event->corr_b[ipart] = beta;
	}
      }
      
      event->SetCorrectedStatus(1); 
    }


}

double Corrections::vz(h22Event &event, int ipart, int GSIM){

  /* 
     
     Vertex correction routine that centers the midplane of the 
     sector correctly by taking into account the offset (x0,y0)
     of the beam.  This offset is calculated by selecting events 
     from the foil.  

     The original source of this method is unknown to me.  The 
     logical within is not clearly described in any source that
     I have.  If anyone knows who/where this code comes from and 
     exactly the methodology applied please contant me.  Thanks.

     - David Riser 
     - May 1, 2018

   */


  // Does this routine really need to accept GSIM?
  // Can we not just ensure that this method is never
  // called for MC?  
  if(GSIM){
    return 0.0; 
  }

  int s = event.dc_sect[ipart] -1;
  if (s<0) {
    return 0.00;
  }

  double px = event.cx[ipart]*event.p[ipart];
  double py = event.cy[ipart]*event.p[ipart];
  double pz = event.cz[ipart]*event.p[ipart];
  
  double s0, sp, sv;
  double x0, y0, z0; 

  // This is the real position of the beam
  // at the z position of the foil.
  x0 =  0.15;
  y0 = -0.25;
  z0 =  0.00;

  // Still trying to figure out exactly what
  // happens here.
  s0 = x0 * vertexParameters::rotationVector[0][s] + 
       y0 * vertexParameters::rotationVector[1][s] + 
       z0 * vertexParameters::rotationVector[2][s];
  
  sp = px * vertexParameters::rotationVector[0][s] + 
       py * vertexParameters::rotationVector[1][s] + 
       pz * vertexParameters::rotationVector[2][s];
  
  sv = event.vx[ipart] * vertexParameters::rotationVector[0][s] + 
       event.vy[ipart] * vertexParameters::rotationVector[1][s] + 
       event.vz[ipart] * vertexParameters::rotationVector[2][s];

  double correctedVertex;
  if(fabs(sp) > 1e-9){
    double A = (s0 - sv) / sp; 
    correctedVertex = event.vz[ipart] + A*pz;
  }
  else{
    correctedVertex = event.vz[ipart];
  }
  
  return correctedVertex;
}

double Corrections::electron_sct(h22Event &event,int ipart,int runno,int GSIM)
{
    double sctime = event.sc_t[ipart];
    int paddle    = event.sc_pd[ipart];
    int sector    = event.sc_sect[ipart];
    
    if(GSIM) return sctime;
    
    if(sector == 2 && paddle == 16 && runno >= 37776 && runno <= 38548) return sctime + 0.5;
    if(sector == 2 && paddle == 16 && runno >= 38549) return sctime + 0.9;
    if(sector == 3 && paddle == 11 && runno >= 37777) return sctime - 2.3;
    if(sector == 4 && paddle == 5 && runno >= 38548) return sctime + 2.0;
    if(sector == 5 && paddle == 3 && runno >= 37673 && runno <= 37854) return sctime + 31.0;
    if(sector == 5 && paddle == 3 && runno >= 37855) return sctime - 0.25;
    if(sector == 5 && paddle == 18 && runno >= 38050 && runno <= 38548) return sctime + 1.1;
    if(sector == 5 && paddle == 20 && runno >= 37777) return sctime - 0.5;
    if(sector == 6 && paddle == 18 && runno >= 38050 && runno <= 38548) return sctime - 1.6;
    
    // note: electrons very rarely hit paddle 2, so the below values were copied from the hadron correction function further down (having these vs not having these makes very little difference)
    if(sector == 3 && paddle == 2 && runno >= 0) return sctime - 15.45;
    if(sector == 4 && paddle == 2 && runno <= 37853) return sctime - 1.9;
    if(sector == 5 && paddle == 2 && runno <= 38240) return sctime - 17.9;
    if(sector == 5 && paddle == 2 && runno >= 38241) return sctime - 19.15;
    
    return sctime;
}

double Corrections::hadron_sct(h22Event &event,int ipart,int runno,int GSIM)
{
    double sctime = event.sc_t[ipart];
    int paddle    = event.sc_pd[ipart];
    int sector    = event.sc_sect[ipart];
    
    if(GSIM) return sctime;
    
    // calibrated using negative tracks: (low paddle numbers)
    if(sector == 6 && paddle == 1 && runno >= 0) return sctime + 18.25;
    if(sector == 3 && paddle == 2 && runno >= 0) return sctime - 15.45;
    if(sector == 4 && paddle == 2 && runno <= 37853) return sctime - 1.9;
    if(sector == 5 && paddle == 2 && runno <= 38240) return sctime - 17.9;
    if(sector == 5 && paddle == 2 && runno >= 38241) return sctime - 19.15;
    if(sector == 5 && paddle == 3 && runno >= 37673 && runno <= 37854) return sctime + 31.0;
    if(sector == 5 && paddle == 3 && runno >= 37855) return sctime - 0.25;
    
    // calibrated using positive tracks:
    if(sector == 1 && paddle == 24 && runno >= 37749) return sctime + 1.13334;
    if(sector == 2 && paddle == 16 && runno >= 37776 && runno <= 38548) return sctime + 0.565033;
    if(sector == 2 && paddle == 16 && runno >= 38549) return sctime + 1.04168;
    if(sector == 2 && paddle == 38 && runno >= 38535) return sctime - 1.89592;
    if(sector == 3 && paddle == 11 && runno >= 37777) return sctime - 2.26126;
    if(sector == 3 && paddle == 24 && runno >= 37855 && runno <= 38546) return sctime - 1.78266;
    if(sector == 3 && paddle == 25 && runno >= 37743 && runno <= 38266) return sctime + 2.44804;
    if(sector == 3 && paddle == 27 && runno >= 37854 && runno <= 38546) return sctime - 1.85815;
    if(sector == 3 && paddle == 28 && runno >= 37854) return sctime + 1.21704;
    if(sector == 4 && paddle == 5 && runno >= 38549) return sctime + 1.91688;
    if(sector == 4 && paddle == 19 && runno >= 37854) return sctime - 0.365798;
    if(sector == 4 && paddle == 34 && runno >= 37854) return sctime - 2.33721;
    if(sector == 4 && paddle == 42 && runno >= 37750) return sctime - 1.4118;
    if(sector == 4 && paddle == 45 && runno >= 38551) return sctime - 3.36406;
    if(sector == 5 && paddle == 18 && runno >= 37854 && runno <= 38545) return sctime + 1.24884;
    if(sector == 5 && paddle == 20 && runno >= 37809) return sctime - 0.468722;
    if(sector == 5 && paddle == 34 && runno <= 37853) return sctime - 1.0;
    if(sector == 5 && paddle == 34 && runno >= 37854) return sctime + 6.0;
    if(sector == 5 && paddle == 36 && runno >= 37748) return sctime + 1.07962;
    if(sector == 6 && paddle == 18 && runno >= 37854 && runno <= 38545) return sctime - 1.69106;
    if(sector == 6 && paddle == 42 && runno >= 37854 && runno <= 38545) return sctime - 6.0;
    
    return sctime;
}

bool Corrections::good_sc_paddle(h22Event &event, int ipart)
{
    int sector = event.sc_sect[ipart];
    int paddle = event.sc_pd[ipart];
    
    // Bad Paddles
    if(sector == 3 && paddle == 2) return 0;
    if(sector == 5 && paddle == 3) return 0;
    if(sector == 2 && paddle == 16) return 0;
    if(sector == 2 && paddle == 40) return 0;
    if(sector == 3 && paddle == 40) return 0;
    if(sector == 5 && paddle == 40) return 0;
    if(sector == 6 && paddle == 40) return 0;
    if(sector == 1 && paddle == 41) return 0;
    if(sector == 2 && paddle == 41) return 0;
    if(sector == 3 && paddle == 41) return 0;
    if(sector == 5 && paddle == 41) return 0;
    if(sector == 1 && paddle == 42) return 0;
    if(sector == 2 && paddle == 42) return 0;
    if(sector == 3 && paddle == 42) return 0;
    if(sector == 5 && paddle == 42) return 0;
    if(sector == 6 && paddle == 42) return 0;
    if(sector == 2 && paddle == 43) return 0;
    if(sector == 3 && paddle == 43) return 0;
    if(sector == 4 && paddle == 43) return 0;
    if(sector == 5 && paddle == 43) return 0;
    if(sector == 1 && paddle == 44) return 0;
    if(sector == 3 && paddle == 44) return 0;
    if(sector == 5 && paddle == 44) return 0;
    if(sector == 6 && paddle == 44) return 0;
    if(sector == 1 && paddle == 45) return 0;
    if(sector == 2 && paddle == 45) return 0;
    if(sector == 3 && paddle == 45) return 0;
    if(sector == 6 && paddle == 45) return 0;
    if(sector == 1 && paddle == 46) return 0;
    if(sector == 2 && paddle == 46) return 0;
    if(sector == 3 && paddle == 46) return 0;
    if(sector == 4 && paddle == 46) return 0;
    if(sector == 5 && paddle == 46) return 0;
    if(sector == 1 && paddle == 47) return 0;
    if(sector == 5 && paddle == 47) return 0;

    // All the other Paddle are good 
    return 1;
    
}

#endif
