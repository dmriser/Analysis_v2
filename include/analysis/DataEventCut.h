/////////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  August 14, 2016 

  Dcut.h -> Define class structure for DataEventCut class.

  DataEventCut is a base class not intended to be directly 
  used but rather overwrite the virtual method passes().

  For each new cut, a class is created that inherits 
  from DataEventCut and the method is overwritten.  The 
  DSelection class then contains a vector<DataEventCut> that
  stores the cuts for a certain selection criteria. 


*/
/////////////////////////////////////////////////////

#ifndef dcut_h
#define dcut_h

// c++ includes
#include <iostream>
#include <map>

// my includes
#include "h22Event.h"
#include "Parameters.h"

// root 
#include "TFile.h"

class DataEventCut{

 public:
  DataEventCut();
  ~DataEventCut();

  int n_pass, n_fail;
  
  // Data Types
 private:
  bool fIsEnabled;
  double fMinValue, fMaxValue;
  std::string fCutName;
  
  // Member functions 
 public:
  void Disable(){ fIsEnabled = false; }
  void Enable(){ fIsEnabled  = true; }
  void SetMin( double x ) { fMinValue = x; }
  void SetMax( double x ) { fMaxValue = x; }
  void SetName( std::string n ){ fCutName = n; }
  bool IsOn(){ return fIsEnabled; }
  bool operator<(DataEventCut *otherCut);
  bool operator>(DataEventCut *otherCut);
  
  int NumberPass(){ return n_pass; }
  int NumberFail(){ return n_fail; }
  int NumberTotal(){ return n_pass+n_fail; }
  
  double GetMin(){ return fMinValue; }
  double GetMax(){ return fMaxValue; }
  double GetPassFraction(){ return (double)n_pass/(n_pass+n_fail); }
  
  std::string GetName(){ return fCutName; }
  
  virtual bool CanBeApplied(h22Event &event, int); /** Pass in the event, and the particle index of interest */
  virtual bool IsPassed(h22Event &event, int); /** Pass in the event, and the particle index of interest */
  virtual float GetFractionalDistance(h22Event &event, int index); /** Pass in the event and the index, get a number 0->1 which tells how far the event falls between min and max */
  virtual void Configure(Parameters *params);

};

/////////////////////////////////////////////////////////////////////
/*

  Charge Cut 

 */
/////////////////////////////////////////////////////////////////////

class ChargeCut : public DataEventCut 
{
 public:
  ChargeCut();
  ~ChargeCut();

  bool IsPassed(h22Event &event, int);

};

/////////////////////////////////////////////////////////////////////
/*

  Track Quality Cut 

 */
/////////////////////////////////////////////////////////////////////

class TrackQualityCut : public DataEventCut 
{
 public:
  TrackQualityCut();
  ~TrackQualityCut();

  bool IsPassed(h22Event &event, int);

};

/////////////////////////////////////////////////////////////////////
/*

  MomentumCut 

 */
/////////////////////////////////////////////////////////////////////

class MomentumCut : public DataEventCut 
{
 public:
  MomentumCut();
  ~MomentumCut();

  bool IsPassed(h22Event &event, int);
  float GetFractionalDistance(h22Event &event, int index);
  void Configure(Parameters *params);
};


/////////////////////////////////////////////////////////////////////
/*

  SamplingFractionCut 

 */
/////////////////////////////////////////////////////////////////////

class SampFracCut : public DataEventCut 
{
 public:
  SampFracCut(int);
  ~SampFracCut();

  int sector; 
  double am, as, bm, bs, cm, cs, dm, ds, nsigma, GetMin, GetMax;
  bool CanBeApplied(h22Event &event, int);
  bool IsPassed(h22Event &event, int);
  float GetFractionalDistance(h22Event &event, int index); 
  void Configure(Parameters *params);
};

/////////////////////////////////////////////////////////////////////
/*

  CCThetaMatchingCut

 */
/////////////////////////////////////////////////////////////////////

class CCThetaMatchingCut : public DataEventCut 
{
 public:
  CCThetaMatchingCut(int);
  ~CCThetaMatchingCut();

  int sector; 
  double am, as, bm, bs, cm, cs, dm, ds, nsigma, GetMin, GetMax;
  bool CanBeApplied(h22Event &event, int);
  bool IsPassed(h22Event &event, int);
  float GetFractionalDistance(h22Event &event, int index);
  void Configure(Parameters *params);
};

/////////////////////////////////////////////////////////////////////
/*

  EC U Cut 

 */
/////////////////////////////////////////////////////////////////////

class ECUCut : public DataEventCut 
{
 public:
  ECUCut();
  ~ECUCut();

  bool IsPassed(h22Event &event, int);
  float GetFractionalDistance(h22Event &event, int index);
  void Configure(Parameters *params);
};

/////////////////////////////////////////////////////////////////////
/*

  EC V Cut 

 */
/////////////////////////////////////////////////////////////////////

class ECVCut : public DataEventCut 
{
 public:
  ECVCut();
  ~ECVCut();
  bool IsPassed(h22Event &event, int);
  float GetFractionalDistance(h22Event &event, int index);
  void Configure(Parameters *params);
};

/////////////////////////////////////////////////////////////////////
/*

  EC W Cut 

 */
/////////////////////////////////////////////////////////////////////

class ECWCut : public DataEventCut 
{
 public:
  ECWCut();
  ~ECWCut();
  bool IsPassed(h22Event &event, int);
  float GetFractionalDistance(h22Event &event, int index);
  void Configure(Parameters *params);
};


/////////////////////////////////////////////////////////////////////
/*

  Z-Vertex Cut 

 */
/////////////////////////////////////////////////////////////////////

class ZVertexCut : public DataEventCut 
{
 public:
  ZVertexCut();
  ~ZVertexCut();
  bool IsPassed(h22Event &event, int);
  float GetFractionalDistance(h22Event &event, int index); 
  void Configure(Parameters *params);
};

/////////////////////////////////////////////////////////////////////
/*

  CC Fid Cut 

 */
/////////////////////////////////////////////////////////////////////

class CCFiducialCut : public DataEventCut 
{
 public:
  CCFiducialCut();
  ~CCFiducialCut();

  double a, b, c; 
  bool IsPassed(h22Event &event, int);
  void Configure(Parameters *params);
  float GetFractionalDistance(h22Event &event, int index); 
};

/////////////////////////////////////////////////////////////////////
/*

  EC Energy Deposition Inner Cut 

 */
/////////////////////////////////////////////////////////////////////

class ECEdepInnerCut : public DataEventCut 
{
 public:
  ECEdepInnerCut();
  ~ECEdepInnerCut();
  bool IsPassed(h22Event &event, int);
  float GetFractionalDistance(h22Event &event, int index); 
  void Configure(Parameters *params);
};

/////////////////////////////////////////////////////////////////////
/*

  DCR1FiducialCut

 */
/////////////////////////////////////////////////////////////////////

class DCR1FiducialCut : public DataEventCut 
{
 public:
  DCR1FiducialCut();
  ~DCR1FiducialCut();

  double angle, height; 

  bool IsPassed(h22Event &event, int);
  float GetFractionalDistance(h22Event &event, int index); 
};

/////////////////////////////////////////////////////////////////////
/*

  DCR3FiducialCut

 */
/////////////////////////////////////////////////////////////////////

class DCR3FiducialCut : public DataEventCut 
{
 public:
  DCR3FiducialCut();
  ~DCR3FiducialCut();

  double angle, height; 
  
  bool IsPassed(h22Event &event, int);
  float GetFractionalDistance(h22Event &event, int index); 
  void Configure(Parameters *params);
};

/////////////////////////////////////////////////////////////////////
/*

  Number of Photoelectrons Cut 

 */
/////////////////////////////////////////////////////////////////////

class NPheCut : public DataEventCut 
{
 public:
  NPheCut();
  ~NPheCut();

  bool IsPassed(h22Event &event, int);

};

/////////////////////////////////////////////////////////////////////
/*

  CC Phi Matching Cut 

 */
/////////////////////////////////////////////////////////////////////

class CCPhiMatchingCut : public DataEventCut 
{
 public:
  CCPhiMatchingCut();
  ~CCPhiMatchingCut();

  bool IsPassed(h22Event &event, int);

};
/////////////////////////////////////////////////////////////////////
/*

  Delta Beta Cut 

 */
/////////////////////////////////////////////////////////////////////

class DataEventCut_DeltaBetaCut : public DataEventCut {
 public:
  DataEventCut_DeltaBetaCut(int PID);
  ~DataEventCut_DeltaBetaCut();

  bool IsPassed(h22Event &event, int hadronIndex);

 protected:
  int fPID; 
};

/////////////////////////////////////////////////////////////////////
/*

  Delta Z-Vertex Cut 

 */
/////////////////////////////////////////////////////////////////////

class DataEventCut_DeltaZVertexCut : public DataEventCut {
 public:
  DataEventCut_DeltaZVertexCut();
  ~DataEventCut_DeltaZVertexCut();

  bool IsPassed(h22Event &event, int index);
  float GetFractionalDistance(h22Event &event, int index);
};

/////////////////////////////////////////////////////////////////////
/*

    TOF Mass Cut

 */
/////////////////////////////////////////////////////////////////////

class DataEventCut_TOFMassCut : public DataEventCut{
 public:
  DataEventCut_TOFMassCut();
  ~DataEventCut_TOFMassCut();

  bool CanBeApplied(h22Event &event, int index);
  bool IsPassed(h22Event &event, int index);
};

/////////////////////////////////////////////////////////////////////
/*

  DBetaMinimizerCut 

 */
/////////////////////////////////////////////////////////////////////

class DataEventCut_DBetaMinimizerCut : public DataEventCut {
 public:
  DataEventCut_DBetaMinimizerCut();
  ~DataEventCut_DBetaMinimizerCut();

  bool CanBeApplied(h22Event &event, int index);
  bool IsPassed(h22Event &event, int index);

  void AddPossibleParticle(int pid){
    possibles.push_back(pid); 
  } 

  void SetTarget(int pid){
    target = pid; 
  } 

  int GetMin(std::vector<float> x){
    int index = 0; 

    for(int i=1; i<x.size(); i++){
      if (x[i] < x[index]){
	index = i;
      }
    }

    return index; 
  }

 protected:
  int              target; 
  std::vector<int> possibles; 
};

/////////////////////////////////////////////////////////////////////
/*

  Beta Vs. P. Pol3 Cut 

 */
/////////////////////////////////////////////////////////////////////

class DataEventCut_BetaPCut : public DataEventCut {
 public:
  DataEventCut_BetaPCut(int s);
  ~DataEventCut_BetaPCut();

  int sector; 

  double am, bm, cm, dm; 
  double as, bs, cs, ds; 
  double nsigma; 

  bool CanBeApplied(h22Event &event, int index);
  bool IsPassed(h22Event &event, int index);
  float GetFractionalDistance(h22Event &event, int index);
};

/////////////////////////////////////////////////////////////////////
/*

  Beta Vs. P. Likelihood Cut for positives 

 */
/////////////////////////////////////////////////////////////////////

// [sector][parameter]
struct beta_p_likelihood_data { 
  double mu[6][3];
  double sigma[6][3];
  double mass; 
  double confidence;
  double theory, mean, reso, residual, likelihood; 
};

class DataEventCut_BetaPLikelihood : public DataEventCut {
 public:
  DataEventCut_BetaPLikelihood(int pid);
  ~DataEventCut_BetaPLikelihood();

  void AddPossibleParticle(int pid);
  bool CanBeApplied(h22Event &event, int index);
  bool IsPassed(h22Event &event, int index);
  float GetFractionalDistance(h22Event &event, int index);
  float GetConfidence(); 
  float GetConfidence(int pid);   
  float GetLikelihood(int pid);
  void Configure(Parameters *params); 

 protected:
  int   fPid; 
  float fConfidenceMin;

  const static int n_momentum_bins = 80; 
  const float p_min = 0.0; 
  const float p_max = 5.5; 

  float pion_prior[n_momentum_bins];
  float kaon_prior[n_momentum_bins];
  float proton_prior[n_momentum_bins];

  // this data structure will hopefully be better 
  std::map<int, beta_p_likelihood_data> fData;
};

/////////////////////////////////////////////////////////////////////
/*

  TestCut

 */
/////////////////////////////////////////////////////////////////////

class TestCut : public DataEventCut{
 public:
  TestCut(); 
  ~TestCut(); 

  bool CanBeApplied(h22Event &event, int index);
  bool IsPassed(h22Event &event, int index);
};

#endif
