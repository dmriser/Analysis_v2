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

  struct Names{
    const static std::string mesons[NMESON]; 
    const static std::string latex[NMESON]; 
  };

  struct Pid {
    const static int pid[NMESON];
  };
}

const std::string constants::Names::mesons[NMESON] = {"pm","pp","km","kp"};
const std::string constants::Names::latex[NMESON]  = {"#pi^{-}","#pi^{+}","K^{-}","K^{+}"};
const int constants::Pid::pid[NMESON] = {-211, 211, -321, 321};

#endif
