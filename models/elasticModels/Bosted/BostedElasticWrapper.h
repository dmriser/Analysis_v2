#ifndef BOSTED_ELASTIC_WRAPPER_H
#define BOSTED_ELASTIC_WRAPPER_H

class BostedElasticWrapper{

 public:
  inline BostedElasticWrapper(){}
  inline ~BostedElasticWrapper(){}

  float getRadiatedValue(float beamEnergy, float theta, float targetRadiationLengths, float wCut);
  float getValue(float beamEnergy, float theta);

};

#endif
