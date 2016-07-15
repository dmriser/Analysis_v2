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
#include "pars.h"

// root includes
#include "TCanvas.h"
#include "TF1.h"
#include "TGraphErrors.h"
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
    string hparfile = opts.args["HPARS"].args;
    
    hpars pars;
    pars.load(hparfile);
    
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
    
    // Bit of Important Parameters for Histograms
    int NPBINS = 50;
    double PMIN = 0.5;
    double PMAX = 2.0;
    double PSTEP = (PMAX-PMIN)/(NPBINS-1);
    const int NSLICES = NPBINS;
    
    // Histograms
    TH1F * h1_dvz[6];
    
    TH2F * h2_p_b_pos[6];
    TH2F * h2_p_b_neg[6];
    TH2F * h2_p_b_neu[6];
    TH2F * h2_p_db_prot[6];
    TH2F * h2_p_db_pim[6];
    TH2F * h2_p_db_pip[6];
    
    TF1 * f_dvz_gauss = new TF1("f_dvz_gauss","gaus",-10,10);
    TF1 * f_db_gauss  = new TF1("f_db_gauss","gaus",-0.05,0.05);
    TF1 * f_db_mean   = new TF1("f_db_mean","[0]",PMIN,PMAX);
    TF1 * f_db_sigma  = new TF1("f_db_sigma","[0]",PMIN,PMAX);

    TH1D * h1_slice_prot[6][NSLICES];
    TH1D * h1_slice_pip[6][NSLICES];
    TH1D * h1_slice_pim[6][NSLICES];
    
    TGraphErrors * g_mean_prot[6];
    TGraphErrors * g_sigma_prot[6];
    TGraphErrors * g_mean_pip[6];
    TGraphErrors * g_sigma_pip[6];
    TGraphErrors * g_mean_pim[6];
    TGraphErrors * g_sigma_pim[6];
    
    double mean[NSLICES];
    double sigma[NSLICES];
    double mean_e[NSLICES];
    double sigma_e[NSLICES];
    double x[NSLICES];
    double dx[NSLICES];
    
    // For passing to TGraph constructor
    for (int b=0; b<NSLICES; b++) {x[b] = (double) b*PSTEP + PMIN; dx[b] = 0.0;}
    
    
    for (int s=0; s<6; s++)
    {
        h1_dvz[s]       = new TH1F(Form("h1_dvz_%d",s),      Form(" Difference between E Z-Vertex #& Candidate Sector %d",s+1),200,-10,10);
        h2_p_b_pos[s]   = new TH2F(Form("h2_p_b_pos_%d",s),  Form(" Beta vs. Momentum (q>0) Sector %d ",s+1),NPBINS,PMIN,PMAX,100,0.2,1.2);
        h2_p_b_neg[s]   = new TH2F(Form("h2_p_b_neg_%d",s),  Form(" Beta vs. Momentum (q<0) Sector %d ",s+1),NPBINS,PMIN,PMAX,100,0.2,1.2);
        h2_p_b_neu[s]   = new TH2F(Form("h2_p_b_neu_%d",s),  Form(" Beta vs. Momentum (q=0) Sector %d ",s+1),NPBINS,PMIN,PMAX,100,0.2,1.2);
        h2_p_db_prot[s] = new TH2F(Form("h2_p_db_prot_%d",s),Form(" #Delta Beta vs. Momentum (Proton) Sector %d ",s+1),NPBINS,PMIN,PMAX,100,-0.6,0.2);
        h2_p_db_pip[s]  = new TH2F(Form("h2_p_db_pip_%d",s), Form(" #Delta Beta vs. Momentum (#pi^+) Sector %d ",s+1),NPBINS,PMIN,PMAX,100,-0.2,0.6);
        h2_p_db_pim[s]  = new TH2F(Form("h2_p_db_pim_%d",s), Form(" #Delta Beta vs. Momentum (#pi^-) Sector %d ",s+1),NPBINS,PMIN,PMAX,100,-0.2,0.2);
    }
    
    int electrons = 0;
    
    for (int iev=0; iev<nev; iev++)
    {
        fReader->GetEntry(iev);
        h22Event event = fReader->GetEvent();
        
        // keeping track of run number and making sure particle filter knows about it as well.
        if ( runno != fReader->runno() ){ runno = fReader->runno(); filter.set_info(GSIM, runno); }
        
        // Load up hadrons if we've electron.
        if (filter.has_electron(event))
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
                    
                    h1_dvz[sector]->Fill(corr.vz(event,0,runno,GSIM)-corr.vz(event,ipart,runno,GSIM));
                }
            }
        }
    }
    
    
    // Fitting Happens Here.
    for (int s=0; s<6; s++)
    {
        h1_dvz[s]->Fit("f_dvz_gauss","RQ");
        pars.DVZM[s] = f_dvz_gauss->GetParameter(1);
        pars.DVZS[s] = f_dvz_gauss->GetParameter(2);
    }
    
    // Do the fitting and slicing for dbeta (proton)
    for (int s=0; s<6; s++)
    {
        for (int b=0; b<NSLICES; b++)
        {
            string name       = Form("h1_slice_prot_%d_%d",s,b);
            h1_slice_prot[s][b]    = new TH1D(name.c_str(),name.c_str(),100,-0.6,0.2);
            h2_p_db_prot[s]   ->ProjectionY(name.c_str(),b+1,b+2);
            h1_slice_prot[s][b]    ->Fit("f_db_gauss","RQ");
            
            mean[b]    = f_db_gauss->GetParameter(1);
            sigma[b]   = f_db_gauss->GetParameter(2);
            mean_e[b]  = f_db_gauss->GetParError(1);
            sigma_e[b] = f_db_gauss->GetParError(2);
        }
        
        g_mean_prot[s]  = new TGraphErrors(NSLICES,x,mean,dx,mean_e);
        g_sigma_prot[s] = new TGraphErrors(NSLICES,x,sigma,dx,sigma_e);
        g_mean_prot[s]  ->Fit("f_db_mean","RQ");
        g_sigma_prot[s] ->Fit("f_db_sigma","RQ");

        // setting params
        pars.PROT_DBM[s] = f_db_mean ->GetParameter(0);
        pars.PROT_DBS[s] = f_db_sigma->GetParameter(0);
    }

    
    // Do the fitting and slicing for dbeta (pip)
    for (int s=0; s<6; s++)
    {
        for (int b=0; b<NSLICES; b++)
        {
            string name           = Form("h1_slice_pip_%d_%d",s,b);
            h1_slice_pip[s][b]    = new TH1D(name.c_str(),name.c_str(),100,-0.6,0.2);
            h2_p_db_pip[s]        ->ProjectionY(name.c_str(),b+1,b+2);
            h1_slice_pip[s][b]    ->Fit("f_db_gauss","RQ");
            
            mean[b]    = f_db_gauss->GetParameter(1);
            sigma[b]   = f_db_gauss->GetParameter(2);
            mean_e[b]  = f_db_gauss->GetParError(1);
            sigma_e[b] = f_db_gauss->GetParError(2);
        }
        
        g_mean_pip[s]  = new TGraphErrors(NSLICES,x,mean,dx,mean_e);
        g_sigma_pip[s] = new TGraphErrors(NSLICES,x,sigma,dx,sigma_e);
        g_mean_pip[s]  ->Fit("f_db_mean","RQ");
        g_sigma_pip[s] ->Fit("f_db_sigma","RQ");

        pars.PIP_DBM[s] = f_db_mean->GetParameter(0);
        pars.PIP_DBS[s] = f_db_sigma->GetParameter(0);
    }
    
    
    // Do the fitting and slicing for dbeta (pim)
    for (int s=0; s<6; s++)
    {
        for (int b=0; b<NSLICES; b++)
        {
            string name           = Form("h1_slice_pim_%d_%d",s,b);
            h1_slice_pim[s][b]    = new TH1D(name.c_str(),name.c_str(),100,-0.6,0.2);
            h2_p_db_pim[s]        ->ProjectionY(name.c_str(),b+1,b+2);
            h1_slice_pim[s][b]    ->Fit("f_db_gauss","RQ");
            
            mean[b]    = f_db_gauss->GetParameter(1);
            sigma[b]   = f_db_gauss->GetParameter(2);
            mean_e[b]  = f_db_gauss->GetParError(1);
            sigma_e[b] = f_db_gauss->GetParError(2);
        }
        
        g_mean_pim[s]  = new TGraphErrors(NSLICES,x,mean,dx,mean_e);
        g_sigma_pim[s] = new TGraphErrors(NSLICES,x,sigma,dx,sigma_e);
        g_mean_pim[s]  ->Fit("f_db_mean","RQ");
        g_sigma_pim[s] ->Fit("f_db_sigma","RQ");
        
        // Setting parfile
        pars.PIM_DBM[s] = f_db_mean ->GetParameter(0);
        pars.PIM_DBS[s] = f_db_sigma->GetParameter(0);

    }
    
      pars.save(hparfile);
    
    TCanvas * c1 = new TCanvas("c1","",1200,800);
    c1->Divide(3,2);
    c1->Print("test.pdf[");
    
    for (int s=0; s<6; s++) {c1->cd(s+1); h2_p_b_neg[s]->Draw("colz"); }   c1->Print("test.pdf");
    for (int s=0; s<6; s++) {c1->cd(s+1); h2_p_b_pos[s]->Draw("colz"); }   c1->Print("test.pdf");
    for (int s=0; s<6; s++) {c1->cd(s+1); h2_p_b_neu[s]->Draw("colz"); }   c1->Print("test.pdf");
    for (int s=0; s<6; s++) {c1->cd(s+1); h2_p_db_prot[s]->Draw("colz"); g_mean_prot[s]->Draw("same"); } c1->Print("test.pdf");
    for (int s=0; s<6; s++) {c1->cd(s+1); h2_p_db_pip[s]->Draw("colz"); g_mean_pip[s]->Draw("same");}  c1->Print("test.pdf");
    for (int s=0; s<6; s++) {c1->cd(s+1); h2_p_db_pim[s]->Draw("colz"); g_mean_pim[s]->Draw("same");}  c1->Print("test.pdf");
    for (int s=0; s<6; s++) {c1->cd(s+1); h1_dvz[s]->Draw("colz"); }       c1->Print("test.pdf");
    
    // Print Slices
    for (int b=0; b<NSLICES; b++)
    {
        for (int s=0; s<6; s++)
        {
            c1->cd(s+1);
            h1_slice_prot[s][b]->Draw();
        }
        c1->Print("test.pdf");
    }
    
    // Print Slices
    for (int b=0; b<NSLICES; b++)
    {
        for (int s=0; s<6; s++)
        {
            c1->cd(s+1);
            h1_slice_pip[s][b]->Draw();
        }
        c1->Print("test.pdf");
    }
    // Print Slices
    for (int b=0; b<NSLICES; b++)
    {
        for (int s=0; s<6; s++)
        {
            c1->cd(s+1);
            h1_slice_pim[s][b]->Draw();
        }
        c1->Print("test.pdf");
    }
    
    c1->Print("test.pdf]");
    
    cout << " Events: " << nev << " Electrons: " << electrons << endl;
    
}
