//////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  August 12, 2016 

  Common.h -> Define common items for 
  ElasticModel. 


*/
//////////////////////////////////////////////

#ifndef elastic_common_h
#define elastic_common_h

#define proton_mass 0.938
#define pion_mass 0.13957
#define electron_mass 0.000511
#define alpha 0.0072993
#define pi 3.1415926
#define to_degrees 180/3.1415926
#define to_radians 3.1415926/180

// Functions Used by Elastic Model 
double sintn(double x);
double sintp(double x);
double spence(double x);
double betafn(double z);

#endif 
