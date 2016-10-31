#ifndef brasse_inclusive_wrapper_h
#define brasse_inclusive_wrapper_h

class BrasseInclusiveWrapper{
 public:
  BrasseInclusiveWrapper();
  ~BrasseInclusiveWrapper();

  float GetXS(float beamEnergy, float qq, float w);
  float GetXSByX(float beamEnergy, float qq, float x);
};

#endif
