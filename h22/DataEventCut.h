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

// my includes
#include "h22Event.h"

class DataEventCut{

 public:
  DataEventCut();
  ~DataEventCut();

  int n_pass, n_fail;
  
  // Data Types
 private:
  bool is_enabled;
  double min_value, max_value;
  std::string cut_name;
  
  // Member functions 
 public:
  void disable(){ is_enabled = false; }
  void enable(){ is_enabled  = true; }
  void set_min( double x ) { min_value = x; }
  void set_max( double x ) { max_value = x; }
  void set_name( std::string n ){ cut_name = n; }
  bool is_on(){ return is_enabled; }
  bool operator<(DataEventCut *otherCut);
  bool operator>(DataEventCut *otherCut);
  
  int number_pass(){ return n_pass; }
  int number_fail(){ return n_fail; }
  int number_total(){ return n_pass+n_fail; }
  
  double min(){ return min_value; }
  double max(){ return max_value; }
  double passFraction(){ return n_pass/(n_pass+n_fail); }
  
  std::string name(){ return cut_name; }
  
  virtual bool applies(h22Event, int); /** Pass in the event, and the particle index of interest */
  virtual bool passes(h22Event, int); /** Pass in the event, and the particle index of interest */

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

  bool passes(h22Event, int);

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

  bool passes(h22Event, int);

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

  bool passes(h22Event, int);

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
  double am, as, bm, bs, cm, cs, dm, ds, nsigma, min, max;
  bool applies(h22Event, int);
  bool passes(h22Event, int);
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
  double am, as, bm, bs, cm, cs, dm, ds, nsigma, min, max;
  bool applies(h22Event, int);
  bool passes(h22Event, int);
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

  bool passes(h22Event, int);

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
  bool passes(h22Event, int);

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
  bool passes(h22Event, int);

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
  bool passes(h22Event, int);

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
  bool passes(h22Event, int);

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
  bool passes(h22Event, int);

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

  bool passes(h22Event, int);

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
  
  bool passes(h22Event, int);

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

  bool passes(h22Event, int);

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

  bool passes(h22Event, int);

};
/////////////////////////////////////////////////////////////////////
/*

  Delta Beta Cut 

 */
/////////////////////////////////////////////////////////////////////

class DataEventCut_DeltaBetaCut : public DataEventCut 
{
 public:
  DataEventCut_DeltaBetaCut();
  ~DataEventCut_DeltaBetaCut();

  bool passes(h22Event event, int electronIndex, int hadronIndex, int PID);
};

/////////////////////////////////////////////////////////////////////
/*

  Delta Z-Vertex Cut 

 */
/////////////////////////////////////////////////////////////////////

class DataEventCut_DeltaZVertexCut : public DataEventCut{
 public:
  DataEventCut_DeltaZVertexCut();
  ~DataEventCut_DeltaZVertexCut();

  bool passes(h22Event event, int index); 
};

#endif
