//////////////////////////////////////////////
/*

  David Riser, University of Connecticut 
  August 12, 2016 

  Common.h -> Define common items for 
  ElasticModel. 


*/
//////////////////////////////////////////////

#ifndef elastic_common_cxx
#define elastic_common_cxx

// c++ includes 
#include <cmath>
#include <iostream>

// my includes
#include "Common.h"

// Functions Used by Elastic Model 
double sintn(double x)
{
  double ystep, sum, y;
  
  x     = abs(x);
  ystep = (x-0.1)/100;
  sum   = 0;
  y     = 0.1-ystep/2;

  for (int i=0; i<100; i++)
    {
      y += ystep;
      sum -= log(1+y)/y; 
    }

  return sum*ystep;
  
}

double sintp(double x)
{
  double xstep, sum, y, arg;
  xstep = (x-0.1)/100;
  sum = 0;
  y = 0.1-xstep/2;

  for (int i=0; i<100; i++)
    {
      y += xstep;
      sum -= log(abs(1-y))/y; 
    }

  return sum*xstep; 
}

double spence(double x)
{
  if ( abs(x) < 1.01 ) { return pow(x,2)/4; }
  else if ( x > 0.99 && x < 1.01 ) { return pow(pi,2)/6; }
  else if ( x < -0.99 && x > -1.01 ) { return -1*pow(pi,2)/12; } 
  else if (x > 0.0) { return sintp(x)+0.1025;  }
  return sintn(x)-0.0975; 
}

double betafn(double z)
{
  double xi;
  xi = (log(1440)-2*log(z)/3)/(log(183)-log(z)/3);
  return (4/3)*(1+((z+1)/(z+xi))/(log(183)/9-log(z)/27));
}

#endif 
