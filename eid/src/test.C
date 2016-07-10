
// c++ includes
#include <iostream>

// my includes
#include "pars.h"
#include "pars.cxx"
#include "h22Event.h"
#include "h22Reader.h"
#include "h22Option.h"

int main(int argc, char * argv[])
{
    // set command line options
    h22Options opts;
    opts.set(argc, argv);
    
    // data reader
    h22Reader fReader(0);
    
    // load parameters
    epars params;
    params.load("test.dat");
    
    int pass, fail;
    
    for (auto it=opts.ifiles.begin(); it<opts.ifiles.end(); it++) { fReader.AddFile(*it); }
    fReader.Init();
    
    for (int iev=0; iev < opts.ARGS["N"].arg; iev++)
    {
        fReader.GetEntry(ien);
        h22Event event = fReader.GetEvent();
        if ( event.tl3_x[0] > params.dc_left(3, event.tl3_y[0]) && event.tl3_x[0] > params.dc_right(3, event.tl3_y[0]) ) pass++;
        else { fail++; }
    }
    
    params.save("test.dat");
    
    std::cout << " Pass: " << pass << " Fail: " << fail << std::endl;
    
    return 0;
}
