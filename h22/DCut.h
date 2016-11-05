/////////////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  August 14, 2016 

  Dcut.h -> Define class structure for DCut class.

  DCut is a base class not intended to be directly 
  used but rather overwrite the virtual method passes().

  For each new cut, a class is created that inherits 
  from DCut and the method is overwritten.  The 
  DSelection class then contains a vector<DCut> that
  stores the cuts for a certain selection criteria. 


*/
/////////////////////////////////////////////////////

#ifndef dcut_h
#define dcut_h

// c++ includes
#include <iostream>

// my includes
#include "h22Event.h"

class DCut
{
 public:
  DCut();
  ~DCut();

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
  
  int number_pass(){ return n_pass; }
  int number_fail(){ return n_fail; }
  int number_total(){ return n_pass+n_fail; }
  
  double min(){ return min_value; }
  double max(){ return max_value; }
  
  std::string name(){ return cut_name; }
  
  virtual bool applies(h22Event, int); /** Pass in the event, and the particle index of interest */
  virtual bool passes(h22Event, int); /** Pass in the event, and the particle index of interest */

};

/////////////////////////////////////////////////////////////////////
/*

  Charge Cut 

 */
/////////////////////////////////////////////////////////////////////

class ChargeCut : public DCut 
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

class TrackQualityCut : public DCut 
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

class MomentumCut : public DCut 
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

class SampFracCut : public DCut 
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

  EC U Cut 

 */
/////////////////////////////////////////////////////////////////////

class ECUCut : public DCut 
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

class ECVCut : public DCut 
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

class ECWCut : public DCut 
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

class ZVertexCut : public DCut 
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

class CCFiducialCut : public DCut 
{
 public:
  CCFiducialCut();
  ~CCFiducialCut();

  double a, b; 
  bool passes(h22Event, int);

};

/////////////////////////////////////////////////////////////////////
/*

  EC Energy Deposition Inner Cut 

 */
/////////////////////////////////////////////////////////////////////

class ECEdepInnerCut : public DCut 
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

class DCR1FiducialCut : public DCut 
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

class DCR3FiducialCut : public DCut 
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

class NPheCut : public DCut 
{
 public:
  NPheCut();
  ~NPheCut();

  bool passes(h22Event, int);

};
/////////////////////////////////////////////////////////////////////
/*

  Delta Beta Cut 

 */
/////////////////////////////////////////////////////////////////////

class DCut_DeltaBetaCut : public DCut 
{
 public:
  DCut_DeltaBetaCut();
  ~DCut_DeltaBetaCut();

  bool passes(h22Event event, int index, int pid);
};

/////////////////////////////////////////////////////////////////////
/*

  Delta Z-Vertex Cut 

 */
/////////////////////////////////////////////////////////////////////

class DCut_DeltaZVertexCut : public DCut{
 public:
  DCut_DeltaZVertexCut();
  ~DCut_DeltaZVertexCut();

  bool passes(h22Event event, int index); 
};

#endif
