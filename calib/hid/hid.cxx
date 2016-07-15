////////////////////////////////////////
/*
 David Riser, University of Connecticut
 
 July 13, 2016
 
 hid.cxx -> Write out Hadron ID Params.
 
 */
////////////////////////////////////////

// c++ includes
#include <iostream>
using namespace std;

// my includes
#include "CommonTools.h"
#include "Corrections.h"
#include "h22Event.h"
#include "h22Option.h"
#include "h22Reader.h"
#include "ParticleFilter.h"

// root includes
#include "TCanvas.h"
#include "TH1.h"
#include "TH2.h"

int main(int argc, char * argv[])
{
    
    // Setup Options
    h22Options opts;
    opts.set(argc,argv);
    int GSIM     = opts.args["MC"].arg;
    long int nev = opts.args["N"].arg;
    string eparfile = opts.args["EPARS"].args;
    
    // Setup Reader
    h22Reader * fReader = new h22Reader(GSIM);
    for (auto it=opts.ifiles.begin(); it<opts.ifiles.end(); it++) { fReader->AddFile(*it); }
    fReader->Init();
    
    nev = smallest(nev, fReader->GetEntries());
    
    // Setting important constants
    int runno = fReader->runno();
    ParticleFilter filter(eparfile);
    filter.set_info(GSIM, runno);
    Corrections corr;
    
    // Histograms
    TH2F * h2_p_b_pos[6];
    TH2F * h2_p_b_neg[6];
    TH2F * h2_p_b_neu[6];
    TH2F * h2_p_db_prot[6];
    TH2F * h2_p_db_pim[6];
    TH2F * h2_p_db_pip[6];
    
    
    for (int s=0; s<6; s++)
    {
        h2_p_b_pos[s]   = new TH2F(Form("h2_p_b_pos_%d",s),Form(" Beta vs. Momentum (q>0) Sector %d ",s+1),100,0.5,5,100,0.2,1.2);
        h2_p_b_neg[s]   = new TH2F(Form("h2_p_b_neg_%d",s),Form(" Beta vs. Momentum (q<0) Sector %d ",s+1),100,0.5,5,100,0.2,1.2);
        h2_p_b_neu[s]   = new TH2F(Form("h2_p_b_neu_%d",s),Form(" Beta vs. Momentum (q=0) Sector %d ",s+1),100,0.5,5,100,0.2,1.2);
        h2_p_db_prot[s] = new TH2F(Form("h2_p_db_prot_%d",s),Form(" #Delta Beta vs. Momentum (Proton) Sector %d ",s+1),100,0.5,5,100,-0.6,0.2);
        h2_p_db_pip[s]  = new TH2F(Form("h2_p_db_pip_%d",s),Form(" #Delta Beta vs. Momentum (#pi^+) Sector %d ",s+1),100,0.5,5,100,-0.2,0.6);
        h2_p_db_pim[s]  = new TH2F(Form("h2_p_db_pim_%d",s),Form(" #Delta Beta vs. Momentum (#pi^-) Sector %d ",s+1),100,0.5,5,100,-0.2,0.2);
    }
    
    int electrons = 0;
    
    for (int iev=0; iev<nev; iev++)
    {
        fReader->GetEntry(iev);
        h22Event event = fReader->GetEvent();
        
        // keeping track of run number and making sure particle filter knows about it as well.
        if ( runno != fReader->runno() ){ runno = fReader->runno(); filter.set_info(GSIM, runno); }
        
        // Looking for electron
        int e_index = filter.getByPID(event,11);
        
        // Load up hadrons if we've electron.
        if (e_index > -123)
        {
            electrons++;
            double start_time = corr.electron_sct(event,0,runno,GSIM) - event.sc_r[0]/speed_of_light;
            for (int ipart=1; ipart<event.gpart; ipart++)
            {
                int sector = event.sc_sect[ipart]-1;
                if (sector > -1)
                {
                    double cbeta = event.sc_r[ipart]/(corr.hadron_sct(event,ipart,runno,GSIM)-start_time);
                    double beta = cbeta/speed_of_light;
                    if (event.q[ipart] < 0) {
                        h2_p_b_neg[sector]  ->Fill(event.p[ipart],beta);
                        h2_p_db_pim[sector] ->Fill(event.p[ipart],(event.p[ipart])/sqrt(event.p[ipart]*event.p[ipart]+pi_mass*pi_mass)-beta);
                    }
                    if (event.q[ipart] > 0) {
                        h2_p_b_pos[sector]   ->Fill(event.p[ipart],beta);
                        h2_p_db_prot[sector] ->Fill(event.p[ipart],(event.p[ipart])/sqrt(event.p[ipart]*event.p[ipart]+proton_mass*proton_mass)-beta);
                        h2_p_db_pip[sector]  ->Fill(event.p[ipart],(event.p[ipart])/sqrt(event.p[ipart]*event.p[ipart]+pi_mass*pi_mass)-beta);
                    }
                    if (event.q[ipart] == 0) { h2_p_b_neu[sector]->Fill(event.p[ipart],beta); }
                }
                if (iev%opts.args["PRINT"].arg == 0) cout << "\r done " << iev << endl;
            }
        }
    }
    
    TCanvas * c1 = new TCanvas("c1","",1200,800);
    c1->Divide(3,2);
    c1->Print("test.pdf[");
    
    for (int s=0; s<6; s++) {c1->cd(s+1); h2_p_b_neg[s]->Draw("colz"); } c1->Print("test.pdf");
    for (int s=0; s<6; s++) {c1->cd(s+1); h2_p_b_pos[s]->Draw("colz"); } c1->Print("test.pdf");
    for (int s=0; s<6; s++) {c1->cd(s+1); h2_p_b_neu[s]->Draw("colz"); } c1->Print("test.pdf");
    for (int s=0; s<6; s++) {c1->cd(s+1); h2_p_db_prot[s]->Draw("colz"); } c1->Print("test.pdf");
    for (int s=0; s<6; s++) {c1->cd(s+1); h2_p_db_pip[s]->Draw("colz"); } c1->Print("test.pdf");
    for (int s=0; s<6; s++) {c1->cd(s+1); h2_p_db_pim[s]->Draw("colz"); } c1->Print("test.pdf");
    
    c1->Print("test.pdf]");
    
    cout << " Events: " << nev << " Electrons: " << electrons << endl;
    
}
