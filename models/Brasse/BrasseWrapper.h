#ifndef brasse_wrapper_h
#define brasse_wrapper_h

class BrasseInclusiveModel{
 public:
  inline BrasseInclusiveModel();
  inline ~BrasseInclusiveModel();

  double getValue(double beamEnergy, double qq, double w);
};

#endif
