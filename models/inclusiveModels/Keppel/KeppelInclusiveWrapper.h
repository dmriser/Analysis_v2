#ifndef keppel_inclusive_wrapper_h
#define keppel_inclusive_wrapper_h

class KeppelInclusiveWrapper{
 public:
  KeppelInclusiveWrapper();
  ~KeppelInclusiveWrapper();

  float GetXS(float beamEnergy, float qq, float w);
  float GetXSByX(float beamEnergy, float qq, float x);
};

#endif
