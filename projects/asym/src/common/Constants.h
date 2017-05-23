#ifndef constants_h
#define constants_h

#include <iostream>

#include "Types.h"

namespace constants {
  const static int NSECT        = 6;
  const static int NMESON       = 4;
  const static int NHEL         = 2;
  const static int MAX_BINS_X   = 8; 
  const static int MAX_BINS_Z   = 8; 
  const static int MAX_BINS_PT  = 8; 
  const static float BEAM_POL   = 0.749;

  struct names{
    const static std::string mesons[NMESON]; 
    const static std::string latex[NMESON]; 
  };

  struct pid {
    const static int pid[NMESON];
  };
}

const std::string constants::names::mesons[NMESON] = {"pm","pp","km","kp"};
const std::string constants::names::latex[NMESON]  = {"#pi^{-}","#pi^{+}","K^{-}","K^{+}"};
const int constants::pid::pid[NMESON] = {-211, 211, -321, 321};

#endif
