#ifndef hadron_id_h
#define hadron_id_h

#include <iostream>
#include <cmath>

#include "h22Event.h"

namespace Nathan {

  // --------------------------------------------------------
  //
  // --------------------------------------------------------
  class HadronIDResult {
  public:
    HadronIDResult(); 
    ~HadronIDResult(); 

    // check the status or ID 
    bool hasProton();
    bool hasPip();
    bool hasPim();

    // stores the results of the identification 
    int protonIndex, pipIndex, pimIndex; 
  };

  // --------------------------------------------------------
  //
  // --------------------------------------------------------
  class BetaPCutLimits {
  public:
    BetaPCutLimits(); 
    ~BetaPCutLimits(); 
    
    // values 
    float min, max, sigma; 
  };

  class BetaPTable {
  public:
    BetaPTable();
    virtual ~BetaPTable();

    // setters 
    void setPath(const std::string path){
      fPath = path; 
    }

    // getter 
    float getMinimumP() const {
      return pMin; 
    }

    float getMaximumP() const {
      return pMax; 
    }

    int getNumberBins() const {
      return fNumberBins; 
    }
    
    int getMomentumBin(float p) const; 

    virtual void loadValues();
    
    BetaPCutLimits & getValues(int type, int sector, int bin); 

  protected:
    std::string fPath; 
    
    // binning 
    const static int fNumberBins = 70; 
    float pMin, pMax;

    // [data or mc][sector][bin] 
    BetaPCutLimits fValues[2][6][fNumberBins];
  };

  // --------------------------------------------------------
  //
  // --------------------------------------------------------
  class PipBetaPTable : public BetaPTable {
  public:
    PipBetaPTable(){
      pMin = 0.2; 
      pMax = 3.75;
    }

    void loadValues();
  };


  // --------------------------------------------------------
  //
  // --------------------------------------------------------
  class PimBetaPTable : public BetaPTable {
  public:
    PimBetaPTable(){
      pMin = 0.2; 
      pMax = 3.25;
    }

    void loadValues();
  };


  // --------------------------------------------------------
  //
  // --------------------------------------------------------
  class PipMethods {
  public:
    PipMethods(PipBetaPTable table); 
    ~PipMethods(); 

    bool MXCut(int strict, float mx);
    bool R1Fid(h22Event &event, int index); 
    bool BetaP(h22Event &event, int index, int expOrSim);

    void SetStrictFid(int strict){
      fStrictFid = strict; 
    }

    void SetStrictBetaP(int strict){
      fStrictBetaP = strict; 
    }

  protected:
    PipBetaPTable fTable; 

    // strictnesses for cuts 
    int fStrictFid; 
    int fStrictBetaP; 

    // for r1 fiducial cut
    const static int NSTRICT = 3; 
    float height[NSTRICT]; 
    float angle[NSTRICT]; 
    float slope[NSTRICT]; 
  };

  // --------------------------------------------------------
  //
  // --------------------------------------------------------
  class PimMethods {
  public:
    PimMethods(PimBetaPTable table); 
    ~PimMethods(); 

  protected:
    PimBetaPTable fTable; 
  };


  // --------------------------------------------------------
  //
  // --------------------------------------------------------
  class HadronID {
  public:
    HadronID(PipBetaPTable pipTable, 
	     PimBetaPTable pimTable);
    ~HadronID();

    void SetRunNumber(const int runno){
      fRunNumber = runno; 
    }
    
    void SetGSIM(const bool gsim){
      fGSIM = gsim; 
    }

    HadronIDResult getIDResult(h22Event & event);

  protected:
    int  fRunNumber; 
    bool fGSIM; 

    PipMethods *fPipMethods; 
    PimMethods *fPimMethods; 
  };

 
}; 

#endif
