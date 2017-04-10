////////////////////////////////////////
/*
 David Riser, University of Connecticut

 July 17, 2016

 rho_pid.cxx ->

 check for pip+pim events.

 */
////////////////////////////////////////

// c++ includes
#include <iostream>
using namespace std;

// my includes from h22 library
#include "CommonTools.h"
#include "Corrections.h"
#include "h22Event.h"
#include "h22Option.h"
#include "h22Reader.h"
#include "pars.h"
#include "ParticleFilter.h"

// root includes
#include "TCanvas.h"
#include "TH1.h"
#include "TLorentzVector.h"
#include "TStopwatch.h"

int main(int argc, char * argv[])
{

    // Setup Options
    h22Options opts;
    opts.set(argc,argv);
    int GSIM     = opts.args["MC"].arg;
    long int nev = opts.args["N"].arg;
    string eparfile = opts.args["EPARS"].args;
    string hparfile = opts.args["HPARS"].args;

    hpars hpars; epars epars;
    epars.load(eparfile);
    hpars.load(hparfile);


    // Setup Reader
    h22Reader fReader(GSIM);
    vector<string>::iterator it; 
    for (it=opts.ifiles.begin(); it<opts.ifiles.end(); it++) { fReader.AddFile(*it); }
    fReader.Init();

    // If you ask for more the loop will analyze garbage
    nev = smallest(nev, fReader.GetEntries());

    // Setting important constants
    int runno = fReader.GetRunNumber();

    // Setting up PID and Corrections
    ParticleFilter filter(eparfile);
    filter.set_info(GSIM, runno);
    Corrections corr;

    // Example of physics
    int rho_candidates = 0;
    TH1F * h1_inv_mass = new TH1F("h1_inv_mass"," Invariant Mass of Pion Pair ",100,-0.1,2.5);
    TH1F * h1_de       = new TH1F("h1_de"," Energy Difference of Pion Pair ",100,-2,2);

    // Keep track of time.
    TStopwatch timer;
    timer.Reset();
    timer.Start();

    for (int iev=0; iev<nev; iev++)
    {
        fReader.GetEntry(iev);
        h22Event event = fReader.GetEvent();

        // keeping track of run number and making sure particle filter knows about it as well.
        if ( runno != fReader.GetRunNumber() ){ runno = fReader.GetRunNumber(); filter.set_info(GSIM, runno); }

        // Load up hadrons if we've electron.
        if (filter.has_electron(event))
        {
            // Look for both particles in same event.
            int pip_index  = filter.getByPID(event, 211);
            int pim_index  = filter.getByPID(event,-211);

            if (pim_index > -123 && pip_index > -123)
            {
                rho_candidates++;
                TLorentzVector pip(event.p[pip_index]*event.cx[pip_index],
                                   event.p[pip_index]*event.cy[pip_index],
                                   event.p[pip_index]*event.cz[pip_index],
                                   sqrt(event.p[pip_index]*event.p[pip_index] + pi_mass*pi_mass));

                TLorentzVector pim(event.p[pim_index]*event.cx[pim_index],
                                   event.p[pim_index]*event.cy[pim_index],
                                   event.p[pim_index]*event.cz[pim_index],
                                   sqrt(event.p[pim_index]*event.p[pim_index] + pi_mass*pi_mass));

                double inv_mass = (pip + pim).Mag2();
                double de       = (pip - pim).E();

                h1_inv_mass->Fill(inv_mass);
                h1_de      ->Fill(de);

            }
        }

        // Tell the user
        if ( (int)iev%(int)opts.args["PRINT"].arg == 0) cout << "\r done " << iev << " of " << nev << flush;
    }

    // If you changed anything in the parfiles
    epars.save(eparfile);
    hpars.save(hparfile);

    cout << "\n Found " << rho_candidates << " candidate rho events " << endl;

    double loop_time  = timer.RealTime();
    double event_rate = (double)nev/loop_time;

    cout << " Event loop finished in " << loop_time << " seconds w/ rate " << event_rate << " events/sec " << endl;

    TCanvas * c1 = new TCanvas("c1","",800,400);
    c1->Divide(2,1);
    c1->cd(1);
    h1_inv_mass->Draw();
    c1->cd(2);
    h1_de->Draw();
    c1->Print("rho_pid.pdf");
}
