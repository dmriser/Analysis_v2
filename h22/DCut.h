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
#include "DEvent.h"

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

  double min(){ return min_value; }
  double max(){ return max_value; }
  
  std::string name(){ return cut_name; }
  
  virtual bool passes(DEvent, int); /** Pass in the event, and the particle index of interest */

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

  bool passes(DEvent, int);

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

  bool passes(DEvent, int);

};


/////////////////////////////////////////////////////////////////////
/*

  SamplingFractionCut 

 */
/////////////////////////////////////////////////////////////////////

class SampFracCut : public DCut 
{
 public:
  SampFracCut();
  ~SampFracCut();

  bool passes(DEvent, int);
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

  bool passes(DEvent, int);

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
  bool passes(DEvent, int);

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
  bool passes(DEvent, int);

};


#endif
