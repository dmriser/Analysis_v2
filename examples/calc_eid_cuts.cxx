//////////////////////////////////////
/*
 
 David Riser, University of Connecticut
 
 calc_eid_cuts.cxx -> test the usage of libraries
 
    July 9, 2016
 
 */
//////////////////////////////////////

// c++ includes
#include <iostream>
#include <vector>

// my includes
#include "h22Event.h"
#include "h22Option.h"
#include "h22Reader.h"
#include "pars.h"

// root includes
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"
#include "TVector3.h"

int main(int argc, char * argv[])
{
    
    
    // Setup Options
    h22Options opts;
    opts.set(argc,argv);
    
    epars pars;
    pars.load(opts.args["EPARS_FILE"].args);
    
    // Setup Reader
    int GSIM = 0;                             //! Set to false, not initializing MC banks.
    h22Reader * fReader = new h22Reader(GSIM);
    
    for (auto it=opts.ifiles.begin(); it<opts.ifiles.end(); it++) { fReader->AddFile(*it); }
    fReader->Init();
    
    long int nev = opts.args["N"].arg;
    if (nev > fReader->GetEntries()) nev = fReader->GetEntries();
    
    // Histograms
    TH1F * h1_ecu[6];
    TH1F * h1_ecv[6];
    TH1F * h1_ecw[6];
    
    for (int s=0; s<6; s++)
    {
        h1_ecu[s] = new TH1F(Form("h1_ecu_%d",s),Form(" EC U Coord. Sector %d",s+1),400,-400,400);
        h1_ecv[s] = new TH1F(Form("h1_ecv_%d",s),Form(" EC V Coord. Sector %d",s+1),400,-100,800);
        h1_ecw[s] = new TH1F(Form("h1_ecw_%d",s),Form(" EC W Coord. Sector %d",s+1),400,-100,800);

    }
    
    for (int iev=0; iev<nev; iev++)
    {
        fReader->GetEntry(iev);
        h22Event event = fReader->GetEvent();
        
        TVector3 uvw = event.uvw(0);
        int sector = event.ec_sect[0]-1;
        if (sector > -1)
        {
            h1_ecu[sector]->Fill(uvw.X());
            h1_ecv[sector]->Fill(uvw.Y());
            h1_ecw[sector]->Fill(uvw.Z());
        }
        
    }
    
    std::cout << " Done! " << std::endl;
    
    // Writing out
    TCanvas * c1 = new TCanvas("c1", "", 1200, 400);
    c1->Divide(3,1);
    c1->Print("test.pdf[");
    
    for (int s=0; s<6; s++)
    {
        c1->cd(1);
        h1_ecu[s]->Draw();
        c1->cd(2);
        h1_ecv[s]->Draw();
        c1->cd(3);
        h1_ecw[s]->Draw();
        c1->Print("test.pdf");
    }
    
    c1->Print("test.pdf]");
    
    return 0;
}
